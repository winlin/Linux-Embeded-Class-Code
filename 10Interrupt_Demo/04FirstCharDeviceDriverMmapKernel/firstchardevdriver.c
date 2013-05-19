/*通过实现设备的mmap()操作,来把两个LED等对应的寄存器
  映射到应用程序的虚拟空间*/
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include<linux/device.h>

#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include<linux/mm.h>

MODULE_LICENSE("Dual BSD/GPL");

#define FIRSTCHARDEVDRIVER_MAJOR 0
#define FIRSTCHARDEVDRIVER_MINOR 0
#define FIRSTCHARDEVDRIVER_COUNT 1
#define FIRSTCHARDEVDRIVER_NAME "firstchardevdriver"
#define FIRSTCHARDEVDRIVER_CLASS_NAME "firstchardevdriver_class"

#define IOMEM_GPIO_BASE		0x7F008000
#define IOMEM_GPIO_SIZE		0x00001000   // 4KB a memory page size

static u32 firstchardevdriver_major=FIRSTCHARDEVDRIVER_MAJOR;
static u32 firstchardevdriver_minor=FIRSTCHARDEVDRIVER_MINOR;

struct  firstchardevdriver_cdev{
	struct device *dev_device;
	struct cdev cdev;

	u8 led;
};
struct firstchardevdriver_cdev *firstchardevdriver_cdevp = NULL;
static struct class *dev_class = NULL;
static struct device *dev_device = NULL;

static int firstchardevdriver_open(struct inode *inode,struct file *filp)
{
	struct firstchardevdriver_cdev *pcdevp = NULL;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_open !\n");

	pcdevp = container_of(inode->i_cdev,struct firstchardevdriver_cdev,cdev);
	pcdevp->led = MINOR(inode->i_rdev);
	filp->private_data = pcdevp;

	return 0;
}

static ssize_t firstchardevdriver_read(struct file *filp,char __user *buf,size_t count,loff_t *f_ops)
{
	struct firstchardevdriver_cdev *pcdevp = filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_read !\n");

	printk(KERN_ALERT "FirstCharDevDriver: LED: %d !\n",pcdevp->led);
	return 0;
}

static ssize_t firstchardevdriver_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_ops)
{
	struct firstchardevdriver_cdev *pcdevp = filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_write !\n");
	printk(KERN_ALERT "FirstCharDevDriver: LED: %d !\n",pcdevp->led);
	return 0;
}

static int firstchardevdriver_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
{
	struct firstchardevdriver_cdev *pcdevp = filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_ioctl !\n");
	printk(KERN_ALERT "FirstCharDevDriver: LED: %d !\n",pcdevp->led);
	return 0;
}

// When user call mmap() the VFS will allocate a VMA to the device
static int firstchardevdriver_mmap(struct file *filp,struct vm_area_struct *vma)
{
	unsigned long vmsize = vma->vm_end - vma->vm_start;
	
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_mmap !\n");

	printk(KERN_ALERT "FirstCharDevDriver: vmsize = 0x%lx !\n",vmsize);

	// SET THE FLAGS
	vma->vm_flags |= VM_IO | VM_RESERVED;
	
	// Allocate a page 
	if(remap_pfn_range(vma,vma->vm_start,(IOMEM_GPIO_BASE >> PAGE_SHIFT),IOMEM_GPIO_SIZE,vma->vm_page_prot)){
		printk(KERN_ALERT "FirstCharDevDriver: Mmap error for GPIO !\n");
		return -EAGAIN;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Mmap success for GPIO !\n");

	return 0;
}

static int firstchardevdriver_release(struct inode *inode,struct file *filp)
{
	struct firstchardevdriver_cdev *pcdevp = filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_release !\n");
	printk(KERN_ALERT "FirstCharDevDriver: LED: %d !\n",pcdevp->led);
	return 0;
}

static struct file_operations firstchardevdriver_fops={
	.owner=THIS_MODULE,
	.open=firstchardevdriver_open,
	.read=firstchardevdriver_read,
	.write=firstchardevdriver_write,
	.ioctl=firstchardevdriver_ioctl,
	.mmap = firstchardevdriver_mmap,
	.release=firstchardevdriver_release,
};

static int firstchardevdriver_cdev_add(struct firstchardevdriver_cdev *pcdevp,int index)
{
	int ret = 0;
	dev_t dev = 0;

	printk(KERN_INFO "FirstCharDevDriver: Entry firstchardevdriver_cdev_add !\n");
	dev = MKDEV(firstchardevdriver_major,firstchardevdriver_minor + index);
	cdev_init( &(pcdevp->cdev), &firstchardevdriver_fops );
	ret = cdev_add( &(pcdevp->cdev),dev,1 );
	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver: Failure to add cdev to Kernel ! Major: %d\tMinor: %d\tErrorno: %d\n",MAJOR(dev),MINOR(dev),ret);
		return ret;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Success to add cdev to Kernel ! Major: %d\tMinor: %d\n",MAJOR(dev),MINOR(dev));
	pcdevp->dev_device = device_create(dev_class,NULL,dev,NULL,"firstchardevdriver%d",MINOR(dev));
	printk(KERN_INFO "FirstCharDevDriver: Success to create device file: /dev/firstchardevdriver%d !\n",MINOR(dev));
	return ret;
}

static int __init firstchardevdriver_init(void)
{
	int ret=0;
	dev_t dev=0;
	int i;
	int j;


	printk(KERN_ALERT "Entry firstchardevdriver_init !\n");
	
	// allocate memory for private struct including struct cdev
	firstchardevdriver_cdevp = kmalloc(sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT,GFP_KERNEL);
	if(!firstchardevdriver_cdevp){
		printk(KERN_ALERT "FirstCharDevDriver: No enough memory !\n");
		ret = -ENOMEM;
		goto failure_alloc_cdev;
	}
	memset(firstchardevdriver_cdevp,0,(sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT));

	//register device class 
	dev_class = class_create(THIS_MODULE,FIRSTCHARDEVDRIVER_CLASS_NAME);
	if(IS_ERR(dev_class)){
		printk(KERN_INFO "FirstCharDevDriver: Cann't create one device class: %s !\n",FIRSTCHARDEVDRIVER_CLASS_NAME);
		ret = -1;
		goto failure_create_class;
	}
	printk(KERN_INFO "FirstCharDevDriver: Success to create one device class: %s !\n",FIRSTCHARDEVDRIVER_CLASS_NAME);

	//register  device number
	if(firstchardevdriver_major){
		dev=MKDEV(firstchardevdriver_major,firstchardevdriver_minor);
		ret=register_chrdev_region(dev,FIRSTCHARDEVDRIVER_COUNT,FIRSTCHARDEVDRIVER_NAME);
	}else{
		ret=alloc_chrdev_region(&dev,firstchardevdriver_minor,FIRSTCHARDEVDRIVER_COUNT,FIRSTCHARDEVDRIVER_NAME);
		firstchardevdriver_major=MAJOR(dev);
	}
	if(ret<0){
		printk(KERN_ALERT "FirstCharDevDriver: Cannot register char device:Device Major=%d! \n",firstchardevdriver_major);
		ret=-1;
		goto failure_register_chrdev;
	}

	printk(KERN_ALERT "FirstCharDevDriver: Success to register char device: Device Major=%d! \n",firstchardevdriver_major);

	//register cdev driver and create device node
	/*memset(&firstchardevdriver_cdev,0,sizeof(struct cdev));
	cdev_init(&firstchardevdriver_cdev,&firstchardevdriver_fops);
	ret=cdev_add(&firstchardevdriver_cdev,dev,FIRSTCHARDEVDRIVER_COUNT);*/
	for(i = 0; i<FIRSTCHARDEVDRIVER_COUNT; i++){
		ret = firstchardevdriver_cdev_add(&(firstchardevdriver_cdevp[ i ]),i);
		if(ret<0){
		/*printk(KERN_ALERT "FirstCharDevDriver: Failure to add cdev to Kernel,return value=%d !\n",ret);*/
			goto failure_add_cdev;
		}
	}
	printk(KERN_ALERT "FirstCharDevDriver: Success to add cdev to Kernel and Create all device nodes !\n");

	// register device 
	/*dev_device = device_create(dev_class,NULL,dev,NULL,"firstchardevdriver%d",MINOR(dev));
	printk(KERN_INFO "FirstCharDevDriver: Success to create device file: /dev/firstchardevdriver%d !\n",MINOR(dev));*/

	return ret;
	
failure_add_cdev:
	for(j=i-1;j>=0;j--){
		device_destroy(dev_class,MKDEV(firstchardevdriver_major,(firstchardevdriver_major + j)));
		cdev_del(&(firstchardevdriver_cdevp[j].cdev));
	}
	class_destroy(dev_class);
failure_create_class:
	kfree(firstchardevdriver_cdevp);
failure_alloc_cdev:
	unregister_chrdev_region(dev,FIRSTCHARDEVDRIVER_COUNT);
failure_register_chrdev:
	return ret;
}


static void __exit firstchardevdriver_exit(void)
{
	int i;
	dev_t dev=0;
	printk(KERN_ALERT "Entry firstchardevdriver_exit !\n");
	dev=MKDEV(firstchardevdriver_major,firstchardevdriver_minor);

	for(i = 0; i < FIRSTCHARDEVDRIVER_COUNT; i++)
	{
		device_destroy(dev_class,MKDEV(firstchardevdriver_major,(firstchardevdriver_minor + i)));

		cdev_del(&(firstchardevdriver_cdevp[ i ].cdev));
	}

	class_destroy(dev_class);

	kfree(firstchardevdriver_cdevp);

	unregister_chrdev_region(dev,FIRSTCHARDEVDRIVER_COUNT);	
}

module_init(firstchardevdriver_init);
module_exit(firstchardevdriver_exit);

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("My first char device driver");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("FirstCharDevDriver");


