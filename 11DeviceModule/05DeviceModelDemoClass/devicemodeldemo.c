#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DEVICEMODELDEMO_MAJOR      240

static struct class *devicemodeldemo_class = NULL;
static struct device *devicemodeldemo_device = NULL;

static int devicemodeldemo_open(struct inode* inode, struct file *filp)
{
	printk(KERN_ALERT "Entry devicemodeldemo_open()\n");
	return 0;
}

static ssize_t devicemodeldemo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_ALERT "Entry devicemodeldemo_read()\n");
	return 0;
}

static int devicemodeldemo_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "Entry devicemodeldemo_release()\n");
	return 0;
}

static struct file_operations devicemodeldemo_fops = {
	.owner = THIS_MODULE,
	.open = devicemodeldemo_open,
	.read = devicemodeldemo_read,
	.release = devicemodeldemo_release,
};


static ssize_t devicemodeldemo_class_name_show(struct class *class, char *buf)
{
	printk(KERN_ALERT "Entry devicemodeldemo_class_name_show()\n");
	return 0;
}
static ssize_t devicemodeldemo_class_name_store(struct class *class, const char *buf, size_t count)
{
	printk(KERN_ALERT "Entry devicemodeldemo_class_name_store()\n");
	return count;
}

static ssize_t devicemodeldemo_device_name_show(struct device *device, struct device_attribute *attr, char *buf)
{
	printk(KERN_ALERT "Entry devicemodeldemo_device_name_show()\n");
	return 0;
}

static ssize_t devicemodeldemo_device_name_store(struct device *device, struct device_attribute *attr, const char *buf, size_t count)
{
	printk(KERN_ALERT "Entry devicemodeldemo_device_name_store()\n");
	return count;
}

static CLASS_ATTR(class_name, 0644, devicemodeldemo_class_name_show, devicemodeldemo_class_name_store);
static DEVICE_ATTR(device_name, 0644, devicemodeldemo_device_name_show, devicemodeldemo_device_name_store);
static int __init devicemodeldemo_init(void)
{
	int ret;
	printk(KERN_ALERT "Entry devicemodeldemo_init()\n");

	// register a char device
	ret = register_chrdev(DEVICEMODELDEMO_MAJOR, "devicemodeldemo_device", &devicemodeldemo_fops);
	if (0 > ret) {
		printk(KERN_ALERT "Fail to register char device:devicemodeldemo_device ! Major:%d\n",DEVICEMODELDEMO_MAJOR);
		goto failure_register_chrdev;
	}
	printk(KERN_ALERT "Success to register char device:devicemodeldemo_device ! Major:%d\n",DEVICEMODELDEMO_MAJOR);

	// register a device class
	devicemodeldemo_class = class_create(THIS_MODULE, "devicemodeldemo_class");
	if (IS_ERR(devicemodeldemo_class)) {
		printk(KERN_ALERT "Fail to register device class:devicemodeldemo_class !\n");
		goto failure_create_class;
	}
	printk(KERN_ALERT "Success to register device class:devicemodeldemo_class !\n");

	// create a char device
	devicemodeldemo_device = device_create(devicemodeldemo_class, NULL, MKDEV(DEVICEMODELDEMO_MAJOR, 0), NULL, "devicemodeldemo_device");
	if (IS_ERR(devicemodeldemo_device)) {
		printk(KERN_ALERT "Fail to create char device :devicemodeldemo_device !\n");
		goto failure_create_device;
	}
	printk(KERN_ALERT "Success to create char device :devicemodeldemo_device !\n");

	// create files
	class_create_file(devicemodeldemo_class, &class_attr_class_name);
	device_create_file(devicemodeldemo_device, &dev_attr_device_name);
	
	return ret;

failure_create_device:
	class_destroy(devicemodeldemo_class);
failure_create_class:
	unregister_chrdev(DEVICEMODELDEMO_MAJOR, "devicemodeldemo_device");
failure_register_chrdev:
	return ret;
	
}

static void __exit devicemodeldemo_exit(void)
{
	printk(KERN_ALERT "Entry devicemodeldemo_exit()\n");

	// delete files
	class_remove_file(devicemodeldemo_class, &class_attr_class_name);
	device_remove_file(devicemodeldemo_device, &dev_attr_device_name);

	device_destroy(devicemodeldemo_class, MKDEV(DEVICEMODELDEMO_MAJOR,0));
	class_destroy(devicemodeldemo_class);

	unregister_chrdev(DEVICEMODELDEMO_MAJOR, "devicemodeldemo_device");
}

module_init(devicemodeldemo_init);
module_exit(devicemodeldemo_exit);

MODULE_AUTHOR("Winlin");
MODULE_DESCRIPTION("Create a Device Class and add a Char-Device into the Class");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("DeviceClassDemo");
