/* 演示怎样手动在/sys/ 下面注册一个kobject设备对象 */
#include<linux/init.h>
#include<linux/module.h>

#include<linux/kobject.h>
#include<linux/sysfs.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct kobject dmdkobj;

static ssize_t devicemodeldemo_attr_show(struct kobject *kobj,struct attribute *attr,char *buf)
{
	printk(KERN_ALERT "DeviceModelDemo: Entry devicemodeldemo_attr_show !\n");

	return sprintf(buf,"DeviceModelDemo: Attribute Name Show: %s !\n",attr->name);
}

static ssize_t devicemodeldemo_attr_store(struct kobject *kobj,struct attribute *attr,const char *buf,  ssize_t count)
{
	char sbuf[100];

	printk(KERN_ALERT "DevicdModelDemo: Entry devicemodeldemo_attr_store !\n");

	sscanf(buf,"%s\n",sbuf);
	printk(KERN_ALERT "DeviceModelDemo: Attribute Name Store: %s !\n",sbuf);

	return count;
}

static void devicemodeldemo_release(struct kobject *kobj)
{
	printk(KERN_ALERT "DeviceModelDemo: Entry devicemodeldemo_release !\n");
	/*当设备的引用计数减少到0的时候，系统会自动调用该设备的 release() 函数，并且会自动删除 /sys/ 下面
	  的设备文件，所以，在release里面不用调用kobject_del() */
}

static struct sysfs_ops devicemodeldemo_sysfs_ops = {
	.show = devicemodeldemo_attr_show,
	.store = devicemodeldemo_attr_store,
};

static struct attribute devicemodeldemo_attr_name = {
	.name = "devicemodeldemo_name",
	.mode = 0644,
};

static struct attribute *devicemodeldemo_default_attrs[] = {
	&devicemodeldemo_attr_name,
	NULL,
};

static struct kobj_type dmdkobj_type = {
	.release = devicemodeldemo_release,
	.sysfs_ops = &devicemodeldemo_sysfs_ops,
	.default_attrs = devicemodeldemo_default_attrs,
};

static int __init devicemodeldemo_init(void)
{
	printk(KERN_ALERT "DeviceModelDemo: Entry devicemodedemo_init !\n");
	//first clear the kobject object
	memset(&dmdkobj,0,sizeof(struct kobject));
	kobject_init_and_add(&dmdkobj,&dmdkobj_type,NULL,"devicemodeldemo");

	return 0;
}

static void __exit devicemodeldemo_exit(void)
{
	printk(KERN_ALERT "DeviceModelDemo: Entry devicemodeldemo_exit !\n");

	kobject_put(&dmdkobj);

	printk(KERN_ALERT "DeviceModelDemo: Exit devicemodeldemo_exit !\n");
}

module_init(devicemodeldemo_init);
module_exit(devicemodeldemo_exit);

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Create a kobject and add it into kernel ");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("DeviceModelDemo");
