#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("Dual BSD/GPL");

static short mpshort = 1;
static int mpint = 500;
static long mplong = 9999;
static char *mpstring = "mpstring";

module_param(mpshort, short, S_IRWXU);
MODULE_PARM_DESC(mpshort, "One short integer");
module_param(mpint, int, S_IRUSR);
MODULE_PARM_DESC(mpint, "One integer");
module_param(mplong, long, S_IRWXU);
MODULE_PARM_DESC(mpshort, "One long integer");
module_param(mpstring, charp, 0);
MODULE_PARM_DESC(mpstring, "One character string");

static int __init modparam_init(void)
{
	printk(KERN_ALERT "Entry modparam_init!\n");

	printk(KERN_ALERT "mpshort = %d \n", mpshort);
	printk(KERN_ALERT "mpint = %d \n", mpint);
	printk(KERN_ALERT "mplong = %ld \n", mplong);
	printk(KERN_ALERT "mpstring = %s \n", mpstring);

	return 0;
}

static void __exit modparam_exit(void)
{
	printk(KERN_ALERT "mpshort = %d \n", mpshort);
	printk(KERN_ALERT "mpint = %d \n", mpint);
	printk(KERN_ALERT "mplong = %ld \n", mplong);
	printk(KERN_ALERT "mpstring = %s \n", mpstring);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Kernel Module Parameters Demo");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("ModParam");

