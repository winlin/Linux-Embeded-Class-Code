/* 定义一个BUS总线对象，并添加两个设备
   定义总线类型
   定义设备类型
   定义设备
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

ssize_t dmd_bus_name_show(struct bus_type *bus, char *buf);
ssize_t dmd_bus_name_store(struct bus_type *bus, const char *buf, size_t count);
ssize_t dmd_bus_modalias_show(struct bus_type *bus, char *buf);
ssize_t dmd_bus_modalias_store(struct bus_type *bus, const char *buf, size_t count);

static BUS_ATTR(bus_name, 0600, dmd_bus_name_show, dmd_bus_name_store);
static BUS_ATTR(bus_modalias, 0600, dmd_bus_modalias_show, dmd_bus_modalias_store);

static struct bus_attribute *devicemodeldemo_bus_attrs[]={
	&bus_attr_bus_name,
	&bus_attr_bus_modalias,
	NULL,
};

ssize_t dmd_bus_name_show(struct bus_type *bus, char *buf)
{
	printk(KERN_ALERT "Entry dmd_bus_name_show() \n");

	return sprintf(buf, "Bus Name Show:%s !\n", devicemodeldemo_bus_attrs[0]->attr.name);
}

ssize_t dmd_bus_name_store(struct bus_type *bus, const char *buf, size_t count)
{
	char sbuf[100];
	printk(KERN_ALERT "Entry dmd_bus_name_store() \n");
	sscanf(buf, "%s\n", sbuf);
	printk(KERN_ALERT "Bus Name Store:%s\n", sbuf);

	return count;
}

ssize_t dmd_bus_modalias_show(struct bus_type *bus, char *buf)
{
	printk(KERN_ALERT "Entry dmd_bus_modalias_show() \n");
	
	return sprintf(buf, "Bus Modalias Show:%s !\n", devicemodeldemo_bus_attrs[1]->attr.name);
}

ssize_t dmd_bus_modalias_store(struct bus_type *bus, const char *buf, size_t count)
{
	char sbuf[100];
	printk(KERN_ALERT "Entry dmd_bus_modalais_store() \n");
	sscanf(buf, "%s\n", sbuf);
	printk(KERN_ALERT "Bus Modalias Store:%s\n", sbuf);
	
	return count;
}

// this function will note be call as for we don't define the device driver
static int devicemodeldemo_bus_match(struct device *dev, struct device_driver *drv)
{
	printk(KERN_ALERT "Entry devicemodeldemo_bus_match()\n");
	return 0;
}

//kernel will call this uevent() to set the envrionment before send to user space
static int devicemodeldemo_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	printk(KERN_ALERT "Entry devicemodeldemo_bus_uevent()\n");
	return 0;
}

static void devicemodeldemo_dev_release(struct device *dev)
{
	printk(KERN_ALERT "Entry devicemodeldemo_dev_release()\n");
}

static struct bus_type devicemodeldemo_bus_type = {
	.name = "dmd_bus",
	.match = devicemodeldemo_bus_match,
	.uevent = devicemodeldemo_bus_uevent,
};

static struct device_type devicemodeldemo_dev_type = {
	.name = "dmd_bus_dev",
	.release = devicemodeldemo_dev_release,
};
	
static struct device devicemodeldemo_dev={
	.parent = NULL,   //&platform_bus
	.bus = &devicemodeldemo_bus_type,
	.type = &devicemodeldemo_dev_type,
	.bus_id = "devmoddev:0001",
};
	
static int __init devicemodelbus_init(void)
{
	int ret;
	printk(KERN_ALERT "Entry devicemodelbus_init()\n");
	//register the BUS
	ret=bus_register(&devicemodeldemo_bus_type);
	if (ret < 0) {
		printk(KERN_ALERT "Fail to register the bus %s\n",devicemodeldemo_bus_type.name);
		return -EFAULT;
	}
	printk(KERN_ALERT "Success to register the bus %s\n",devicemodeldemo_bus_type.name);

	//create the BUS FILE
	bus_create_file(&devicemodeldemo_bus_type, &bus_attr_bus_name);
	bus_create_file(&devicemodeldemo_bus_type, &bus_attr_bus_modalias);

	ret = device_register(&devicemodeldemo_dev);
	if (0 > ret) {
		printk(KERN_ALERT "Fail to register device:%s\n", devicemodeldemo_dev.type->name);
		goto failure_device_register;
	}
	printk(KERN_ALERT "Success to register device:%s\n", devicemodeldemo_dev.type->name);
	return ret;

failure_device_register:
	bus_remove_file(&devicemodeldemo_bus_type, &bus_attr_bus_name);
	bus_remove_file(&devicemodeldemo_bus_type, &bus_attr_bus_modalias);

	bus_unregister(&devicemodeldemo_bus_type);
	return ret;
}

static void __exit devicemodelbus_exit(void)
{
	printk(KERN_ALERT "Entry devicemodlebus_exit()");

	device_unregister(&devicemodeldemo_dev);
	
	bus_remove_file(&devicemodeldemo_bus_type, &bus_attr_bus_name);
	bus_remove_file(&devicemodeldemo_bus_type, &bus_attr_bus_modalias);

	bus_unregister(&devicemodeldemo_bus_type);
	
}

module_init(devicemodelbus_init);
module_exit(devicemodelbus_exit);

MODULE_AUTHOR("Winlin");
MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION("Create a bus and its attributes");
MODULE_ALIAS("DeviceModelBus");
