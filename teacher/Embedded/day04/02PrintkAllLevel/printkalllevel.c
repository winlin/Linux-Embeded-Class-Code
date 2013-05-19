#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init printkalllevel_init(void)
{
	printk(KERN_EMERG "Printk Levle 0 !\n");
	printk(KERN_ALERT "Printk Levle 1 !\n");
	printk(KERN_CRIT "Printk Levle 2 !\n");
	printk(KERN_ERR "Printk Levle 3 !\n");
	printk(KERN_WARNING "Printk Levle 4 !\n");
	printk(KERN_NOTICE "Printk Levle 5 !\n");
	printk(KERN_INFO "Printk Levle 6 !\n");
	printk(KERN_DEBUG "Printk Levle 7 !\n");

	return 0;
}

static void __exit printkalllevel_exit(void)
{
	printk(KERN_EMERG "Printk Levle 0 !\n");
	printk(KERN_ALERT "Printk Levle 1 !\n");
	printk(KERN_CRIT "Printk Levle 2 !\n");
	printk(KERN_ERR "Printk Levle 3 !\n");
	printk(KERN_WARNING "Printk Levle 4 !\n");
	printk(KERN_NOTICE "Printk Levle 5 !\n");
	printk(KERN_INFO "Printk Levle 6 !\n");
	printk(KERN_DEBUG "Printk Levle 7 !\n");
}

module_init(printkalllevel_init);
module_exit(printkalllevel_exit);

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Kernel Module to print all printk levle");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("PrintkAllLevel");

