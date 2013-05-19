/*
  注册设备号和设备对象，并且使用脚本文件创建
  设备文件，并声明了f_operations对应的函数
*/
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define FIRSTCHARDEVDRIVER_MAJOR     0
#define FIRSTCHARDEVDRIVER_MINOR     0
#define FIRSTCHARDEVDRIVER_COUNT     1
#define FIRSTCHARDEVDRIVER_NAME      "firstchardevdriver"


static unsigned int firstchardevdriver_major=FIRSTCHARDEVDRIVER_MAJOR;

static struct cdev firstchardevdriver_cdev;

static int firstchardevdriver_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_open()\n");
	return 0;
}

static ssize_t firstchardevdriver_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver()_read\n");
	return 0;
}

static ssize_t firstchardevdriver_write(struct file *filp,const char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_write()\n");
	return 0;
}

static int firstchardevdriver_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ALERT "FirstCharDevDriver: Entry firstchardevdriver_ioctl()\n");
	return 0;
}

static int firstchardevdriver_release(struct inode *inode, struct file *filp)
{
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

static int __init firstchardevdriver_init(void)
{
	int ret=0;
	dev_t dev=0;
	printk(KERN_ALERT "Entry firstchardevdriver_init()\n");
	
	if(firstchardevdriver_major){
		//rigister the device number
		dev=MKDEV(firstchardevdriver_major, FIRSTCHARDEVDRIVER_MINOR);
		ret=register_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT, FIRSTCHARDEVDRIVER_NAME);
	}
	else{
		ret=alloc_chrdev_region(&dev, FIRSTCHARDEVDRIVER_MINOR, FIRSTCHARDEVDRIVER_COUNT, "firstchardevdriver");
		firstchardevdriver_major=MAJOR(dev);
	}

	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver:Can't register the char device :Device Major=%d !\n", firstchardevdriver_major);
		ret=-1;
		goto failure_register_chrdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver:Success to register char device :Devic Major=%d!\n", firstchardevdriver_major);
	
	//register cdev object
	memset(&firstchardevdriver_cdev, 0, sizeof(struct cdev));
	cdev_init(&firstchardevdriver_cdev, &firstchardevdriver_fops);
	ret=cdev_add(&firstchardevdriver_cdev, dev, FIRSTCHARDEVDRIVER_COUNT);
	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver: Failure to add cdev to Kernel, return value=%d\n", ret);
		goto failure_add_cdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Success to add cdev to Kernel \n");
	return ret;
	
failure_add_cdev:
	unregister_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT);
failure_register_chrdev:
	return ret;
}

static void __exit firstchardevdriver_exit(void)
{
	dev_t dev=0;
	printk(KERN_ALERT "Entry firstchardevdriver_exit()\n");
	dev=MKDEV(firstchardevdriver_major, FIRSTCHARDEVDRIVER_MINOR);
	
	cdev_del(&firstchardevdriver_cdev);
	unregister_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT);
	
}

module_init(firstchardevdriver_init);
module_exit(firstchardevdriver_exit);

MODULE_AUTHOR("WinLin");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("FirstCharDevDriver");
MODULE_DESCRIPTION("FirstCharDevDriver");

