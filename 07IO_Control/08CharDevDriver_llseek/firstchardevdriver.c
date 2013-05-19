/*
 调用class_create() decive_create()来自动
 创建设备类和设备节点
 整合设备,优化一下程序结构
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

#define MAX_DATA_LENGTH       10

static unsigned int firstchardevdriver_major=FIRSTCHARDEVDRIVER_MAJOR;
static unsigned int firstchardevdriver_minor=FIRSTCHARDEVDRIVER_MINOR;

struct firstchardevdriver_cdev{
	struct device *dev_device;
	struct cdev m_cdev;
	u32 led;

	u8 data[MAX_DATA_LENGTH];
	u32 data_len;
};

struct firstchardevdriver_cdev *firstchardevdriver_cdevp=NULL;
static struct class *dev_class=NULL;

static int firstchardevdriver_display_data(struct file* filp, struct firstchardevdriver_cdev *pcdevp)
{
	int i;
	printk(KERN_ALERT "Entry disply_data()\n");
	printk(KERN_ALERT "Current data length:%d\n",pcdevp->data_len);
	printk(KERN_ALERT "Current data f_pos:%d\n",(int)(filp->f_pos));
	
	for(i=0;i<MAX_DATA_LENGTH;++i){
		printk(KERN_ALERT "Data[%d]=%d\t", i, pcdevp->data[i]);
		if(0==(i+1)%5){
			printk(KERN_ALERT "\n");
		}
	}
	printk("\n");
	return 0;
	
}

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
	u32 pos=*f_pos;
	u32 cnt=count;

	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver()_read\n");
	
	if(pos >= pcdevp->data_len) {
		printk(KERN_ALERT "File pointer at the end of file \n");
		return -EINVAL;
	}
	if(cnt >(pcdevp->data_len - pos)) {
		cnt=pcdevp->data_len-pos;
	}
	if(copy_to_user(buf, &(pcdevp->data[pos]), cnt)) {
		printk(KERN_ALERT "Faile to read data to user space\n");
		return -EINVAL;
	}

	*f_pos+=cnt;
	
	return cnt;
}

static ssize_t firstchardevdriver_write(struct file *filp,const char __user *buf, size_t count, loff_t *f_pos)
{
	struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	u32 pos=*f_pos;
	u32 cnt=count;
	
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_write()\n");
	
	if(MAX_DATA_LENGTH <= pcdevp->data_len) {
		printk(KERN_ALERT "The buffer is full \n");
		return -EINVAL;
	}
	if((MAX_DATA_LENGTH-pcdevp->data_len) < cnt) {
		cnt=MAX_DATA_LENGTH-pcdevp->data_len;
	}
	if(copy_from_user(&(pcdevp->data[pos]), buf, cnt)) {
		printk(KERN_ALERT "Write data to User Space failed \n");
		return -EINVAL;
	}
	pcdevp->data_len+=cnt;
	*f_pos+=cnt;
	firstchardevdriver_display_data(filp, pcdevp);
	
	return cnt;
}

static loff_t firstchardevdriver_llseek(struct file *filp, loff_t off, int whence)
{
	struct firstchardevdriver_cdev *pcdevp=filp->private_data;
	loff_t newpos;
	printk(KERN_ALERT "Entry firstchardevdriver_llseek() !\n");
	
	printk(KERN_ALERT "Old file poniter:%d\n",(int)(filp->f_pos));
	
	switch(whence) {
	case SEEK_SET:
		newpos=off;
		break;
	case SEEK_CUR:
		newpos=filp->f_pos;
		break;
	case SEEK_END:
		newpos=pcdevp->data_len+off;
		break;
	default:
		return -EINVAL;
	}

	if(0 > newpos||newpos > pcdevp->data_len) {
		return -EINVAL;
	}
	filp->f_pos=newpos;
	printk(KERN_ALERT "New file pointer:%d\n",(int)(filp->f_pos));
	
	firstchardevdriver_display_data(filp, pcdevp);
	
	return newpos;
}

static int firstchardevdriver_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	//struct firstchardevdriver_cdev *pcdevp=filp->private_data;
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
	.llseek=firstchardevdriver_llseek,
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
	
	//allocate memory for private struct including struct cdev 
	firstchardevdriver_cdevp=kmalloc(sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT, GFP_KERNEL);
	memset(firstchardevdriver_cdevp, 0, sizeof(struct firstchardevdriver_cdev)*FIRSTCHARDEVDRIVER_COUNT);
	
	//rigister device class
	dev_class=class_create(THIS_MODULE, FIRSTCHARDEVDRIVER_CLASS_NAME);
	if(IS_ERR(dev_class)){
		printk(KERN_ALERT "FirstCharDevDriver: Fail to create one device class:%s\n", FIRSTCHARDEVDRIVER_CLASS_NAME);
		ret=-1;
		goto failure_create_class;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Sucess to create one device class:%s\n", FIRSTCHARDEVDRIVER_CLASS_NAME);
	
	//register cdev objects
	if(!firstchardevdriver_cdevp){
		printk(KERN_ALERT "FirstCharDevDriver:No enough memory\n");
		ret=-ENOMEM;
		goto failure_alloc_cdev;
	}

	ret=firstchardevdriver_cdev_add(firstchardevdriver_cdevp, firstchardevdriver_minor);
	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver: Failure to add cdev to Kernel, return value=%d\n", ret);
		goto failure_add_cdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Success to add a cdev to Kernel and one device files\n");
	
	return ret;
	
failure_add_cdev:
	device_destroy(dev_class, MKDEV(firstchardevdriver_major,firstchardevdriver_minor));
         /* Attention to chang the dev_t variable */
	cdev_del(&(firstchardevdriver_cdevp->m_cdev));
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

	device_destroy(dev_class, MKDEV(firstchardevdriver_major,firstchardevdriver_minor));
	cdev_del(&((firstchardevdriver_cdevp+i)->m_cdev));
	
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

