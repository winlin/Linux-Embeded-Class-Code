#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>

MODULE_LICENSE("Dual BSD/GPL");

#define VIRTUALBLOCKDEVICE_MAJOR      0
#define VIRTUALBLOCKDEVICE_MINOR      0
#define VIRTUALBLOCKDEVICE_NAME       "virtualblockdevice"

static u32 virtualblockdevice_major = VIRTUALBLOCKDEVICE_MAJOR;
static u32 virtualblockdevice_minor = VIRTUALBLOCKDEVICE_MINOR;

static struct block_device_operations virtualblockdevice_fops = {
	.owner = THIS_MODULE,
};

static int __init virtualblockdevice_init(void)
{
	int ret;
	printk(KERN_ALERT "Entry virtualblockdevice_init()\n");

	ret = register_blkdev(virtualblockdevice_major, VIRTUALBLOCKDEVICE_NAME);
	if (0 > ret) {
		printk(KERN_ALERT "Fail to register block device: Major=%d\n",virtualblockdevice_major);
		goto failure_register_blkdev;
	}
	virtualblockdevice_major = ret;
	
	printk(KERN_ALERT "Success to register block device: Major=%d\n",virtualblockdevice_major);

	return 0;

failure_register_blkdev:
	return ret;
}

static void __exit virtualblockdevice_exit(void)
{
	printk(KERN_ALERT "Entry virtualblockdevice_exit()\n");
	unregister_blkdev(virtualblockdevice_major, VIRTUALBLOCKDEVICE_NAME);
}

module_init(virtualblockdevice_init);
module_exit(virtualblockdevice_exit);

MODULE_AUTHOR("Winlin");
MODULE_DESCRIPTION("VirtualBlockDevice");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("VirtualBlockDevice");
