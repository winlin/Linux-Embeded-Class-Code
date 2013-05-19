/*
 调用class_create() decive_create()来自动
 创建设备类和设备节点
 整合设备,优化一下程序结构
 这个程序可以创建多个设备文件
 */
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define FIRSTCHARDEVDRIVER_MAJOR     0
#define FIRSTCHARDEVDRIVER_MINOR     0
#define FIRSTCHARDEVDRIVER_COUNT     1
#define FIRSTCHARDEVDRIVER_NAME      "firstchardevdriver"
#define FIRSTCHARDEVDRIVER_CLASS_NAME "firstchardevdriver_class"


static unsigned int firstchardevdriver_major=FIRSTCHARDEVDRIVER_MAJOR;
static unsigned int firstchardevdriver_minor=FIRSTCHARDEVDRIVER_MINOR;

struct firstchardevdriver_cdev{
	struct device *dev_device;
	struct cdev m_cdev;
	u32 led;
};

struct firstchardevdriver_cdev *firstchardevdriver_cdevp=NULL;
static struct class *dev_class=NULL;

static int firstchardevdriver_open(struct inode *inode, struct file *filp)
{
	struct firstchardevdriver_cdev *pcdevp=NULL;
	
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_open()\n");

	pcdevp=container_of(inode->i_cdev, struct firstchardevdriver_cdev,m_cdev);
	pcdevp->led=MINOR(inode->i_rdev);
	filp->private_data=pcdevp;
	
	return 0;
}

static ssize_t firstchardevdriver_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	char kbuf[50]="Data in kernel Space!";
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver()_read\n");
	
	if(4==count){
		if(put_user(pcdevp->led,(unsigned long __user*)buf)){
			goto write_to_user_error;
		}
		printk(KERN_INFO "Kernel write intege into User space:0x%08x\n",pcdevp->led);
	}
	else{
		if(copy_to_user(buf, kbuf,count)){
			goto write_to_user_error;
		}
		printk(KERN_ALERT "Kernel write string into User space :%s\n",buf);
	}
	
	return count;
write_to_user_error:
	return 0;
}

static ssize_t firstchardevdriver_write(struct file *filp,const char __user *buf, size_t count, loff_t *f_pos)
{
	struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	char kbuf[50]={0};
	
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_write()\n");
	if(4==count){
		if(get_user(pcdevp->led, (unsigned long __user *)buf)){
			goto read_from_user_error;
		}
		printk(KERN_INFO "Read an integer from user space to kernel space :0x%08x\n",pcdevp->led);
	}
	else{
		if(copy_from_user( kbuf, buf, count)){
			goto read_from_user_error;
		}
		printk(KERN_INFO "Read a string from user space to kernel space: %s\n",kbuf);
	}

	return count;
read_from_user_error:
	return 0;
}

static int firstchardevdriver_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_ioctl()\n");
	
	return 0;
}

static int firstchardevdriver_release(struct inode *inode, struct file *filp)
{
	//struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_release()\n");
	return 0;
}

static struct file_operations firstchardevdriver_fops={
	.owner=THIS_MODULE,
	.open=firstchardevdriver_open,
	.read=firstchardevdriver_read,
	.write=firstchardevdriver_write,
	.ioctl=firstchardevdriver_ioctl,
	.release=firstchardevdriver_release,
};
static int firstchardevdriver_cdev_add(struct firstchardevdriver_cdev *pcdevp, int index)
{
	int ret=0;
	dev_t dev=0;
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_cdev_add()\n");
	
	dev=MKDEV(firstchardevdriver_major, firstchardevdriver_minor+index);
	cdev_init(&(pcdevp->m_cdev), &firstchardevdriver_fops);
	ret=cdev_add(&(pcdevp->m_cdev), dev, 1);
	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver: Fail to add a device to Kernel!Major:%d\tMinor:%d\tError:%d\n", MAJOR(dev), MINOR(dev), ret);
	}
	printk(KERN_ALERT "FirstCharDevDriver:Success to add a device to Kernel!Major:%d\tMinor:%d\n", MAJOR(dev), MINOR(dev));
	
	pcdevp->dev_device=device_create(dev_class, NULL, dev, NULL, "firstchardevdriver%d",MINOR(dev));
	printk(KERN_ALERT "FirstCharDevDriver: Success to add a device file:/dev/firstchatdevdriver%d\n",MINOR(dev));
	
	return ret;
}

static int __init firstchardevdriver_init(void)
{
	int ret=0;
	dev_t dev=0;
	int i=0;
	int j=0;
	
	printk(KERN_ALERT "Entry firstchardevdriver_init()\n");
	
	if(firstchardevdriver_major){
		//rigister the device number
		dev=MKDEV(firstchardevdriver_major, firstchardevdriver_minor);
		ret=register_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT, FIRSTCHARDEVDRIVER_NAME);
	}
	else{
		ret=alloc_chrdev_region(&dev, firstchardevdriver_minor, FIRSTCHARDEVDRIVER_COUNT, "firstchardevdriver");
		firstchardevdriver_major=MAJOR(dev);
	}

	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver:Can't register the char device :Device Major=%d !\n", firstchardevdriver_major);
		ret=-1;
		goto failure_register_chrdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver:Success to register char device :Devic Major=%d!\n", firstchardevdriver_major);
	
	//rigister device class
	dev_class=class_create(THIS_MODULE, FIRSTCHARDEVDRIVER_CLASS_NAME);
	if(IS_ERR(dev_class)){
		printk(KERN_ALERT "FirstCharDevDriver: Fail to create one device class:%s\n", FIRSTCHARDEVDRIVER_CLASS_NAME);
		ret=-1;
		goto failure_create_class;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Sucess to create one device class:%s\n", FIRSTCHARDEVDRIVER_CLASS_NAME);
	
	//register cdev objects
        //allocate memory for private struct including struct cdev 
	firstchardevdriver_cdevp=kmalloc(sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT, GFP_KERNEL);
	if(!firstchardevdriver_cdevp){
		printk(KERN_ALERT "FirstCharDevDriver:No enough memory\n");
		ret=-ENOMEM;
		goto failure_alloc_cdev;
	}
	memset(firstchardevdriver_cdevp, 0, sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT);

	for(i=0;i<FIRSTCHARDEVDRIVER_COUNT;++i){
		ret=firstchardevdriver_cdev_add((firstchardevdriver_cdevp+i), i);
		if(0>ret){
			printk(KERN_ALERT "FirstCharDevDriver: Failure to add cdev to Kernel, return value=%d\n", ret);
			goto failure_add_cdev;
		}
		printk(KERN_ALERT "FirstCharDevDriver: Success to add a cdev to Kernel and one device files\n");
	}

	return ret;
	
failure_add_cdev:
	for(j=i-1;j>=0;--j){
		device_destroy(dev_class, MKDEV(firstchardevdriver_major,(firstchardevdriver_minor+j)));   /* Attention to chang the dev_t variable */
		cdev_del(&((firstchardevdriver_cdevp+i)->m_cdev));	
	}
	class_destroy(dev_class);
failure_create_class:
	kfree(firstchardevdriver_cdevp);
failure_alloc_cdev:
	unregister_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT);
failure_register_chrdev:
	return ret;
}

static void __exit firstchardevdriver_exit(void)
{
	dev_t dev=0;
	unsigned int i=0;
	printk(KERN_ALERT "Entry firstchardevdriver_exit()\n");
	dev=MKDEV(firstchardevdriver_major, firstchardevdriver_minor);
	
	device_destroy(dev_class, dev);
	
	for(i=0;i<FIRSTCHARDEVDRIVER_COUNT;++i){
		device_destroy(dev_class, MKDEV(firstchardevdriver_major,(firstchardevdriver_minor+i)));
		cdev_del(&((firstchardevdriver_cdevp+i)->m_cdev));
	}
	class_destroy(dev_class);
	kfree(firstchardevdriver_cdevp);
	
	unregister_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT);
	
}

module_init(firstchardevdriver_init);
module_exit(firstchardevdriver_exit);

MODULE_AUTHOR("WinLin");
MODULE_VERSION("0.0.2");
MODULE_ALIAS("FirstCharDevDriver");
MODULE_DESCRIPTION("FirstCharDevDriver");

