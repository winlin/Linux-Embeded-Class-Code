#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

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

static struct bus_type devicemodeldemo_bus_type={
	.name = "dmd_bus",
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

	return 0;
	
}

static void __exit devicemodelbus_exit(void)
{
	printk(KERN_ALERT "Entry devicemodlebus_exit()");
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
