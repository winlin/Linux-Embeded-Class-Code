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

#define FIRSTCHARDEVDRIVER_MAJOR	240
#define FIRSTCHARDEVDRIVER_MINOR	0
#define FIRSTCHARDEVDRIVER_COUNT	1
#define FIRSTCHARDEVDRIVER_NAME		"firstchardevdriver"

static int __init firstchardevdriver_init(void)
{
	int ret = 0;
	dev_t dev = 0;

	printk(KERN_ALERT "Entry firstchardevdriver_init !\n");

	// register device number
	dev = MKDEV( FIRSTCHARDEVDRIVER_MAJOR, FIRSTCHARDEVDRIVER_MINOR );
	ret = register_chrdev_region( dev, FIRSTCHARDEVDRIVER_COUNT, FIRSTCHARDEVDRIVER_NAME );
	if( 0 > ret ) {
		printk(KERN_ALERT "FirstCharDevDriver: Cann't register char device: Device Major = %d !\n", FIRSTCHARDEVDRIVER_MAJOR );
		ret = -1;
		goto failure_register_chrdev;
	}
	printk(KERN_ALERT "FirstCharDevDriver: Success to register char device: Device Major = %d !\n", FIRSTCHARDEVDRIVER_MAJOR );

	return ret;

failure_register_chrdev:
	return ret;
}	

static void __exit firstchardevdriver_exit(void)
{
	dev_t dev = 0;

	printk(KERN_ALERT "Entry firstchardevdriver_exit !\n");

	dev = MKDEV( FIRSTCHARDEVDRIVER_MAJOR, FIRSTCHARDEVDRIVER_MINOR );

	unregister_chrdev_region( dev, FIRSTCHARDEVDRIVER_COUNT );
}

module_init( firstchardevdriver_init );
module_exit( firstchardevdriver_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("My first char device driver");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("FirstCharDevDriver");

