/*
  演示注册设备号和添加设备对象
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

#define FIRSTCHARDEVDRIVER_MAJOR     240
#define FIRSTCHARDEVDRIVER_MINOR     0
#define FIRSTCHARDEVDRIVER_COUNT     1
#define FIRSTCHARDEVDRIVER_NAME      "firstchardevdriver"

static struct cdev firstchardevdriver_cdev;
static struct file_operations firstchardevdriver_fops={.owner=THIS_MODULE,};

static int __init firstchardevdriver_init(void)
{
	int ret=0;
	dev_t dev=0;
	printk(KERN_ALERT "Entry firstchardevdriver_init()\n");
	
	//rigister the device number
	dev=MKDEV(FIRSTCHARDEVDRIVER_MAJOR, FIRSTCHARDEVDRIVER_MINOR);
	ret=register_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT, FIRSTCHARDEVDRIVER_NAME);
	if(0>ret){
		printk(KERN_ALERT "FirstCharDevDriver:Can't register the char device :Device Major=%d !\n", FIRSTCHARDEVDRIVER_MAJOR);
		ret=-1;
		goto failure_register_chrdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver:Success to register char device :Devic Major=%d!\n", FIRSTCHARDEVDRIVER_MAJOR);
	
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
	dev=MKDEV(FIRSTCHARDEVDRIVER_MAJOR, FIRSTCHARDEVDRIVER_MINOR);
	
	cdev_del(&firstchardevdriver_cdev);
	unregister_chrdev_region(dev, FIRSTCHARDEVDRIVER_COUNT);
	
}

module_init(firstchardevdriver_init);
module_exit(firstchardevdriver_exit);

MODULE_AUTHOR("WinLin");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("FirstCharDevDriver");
MODULE_DESCRIPTION("FirstCharDevDriver");

