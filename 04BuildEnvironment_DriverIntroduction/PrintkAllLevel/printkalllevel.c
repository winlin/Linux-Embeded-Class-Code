#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init printkalllevel_init(void)
{
	printk(KERN_EMERG "Printk Level 0 !\n");
	printk(KERN_ALERT "Printk Level 1 !\n");
	printk(KERN_CRIT "Printk Level 2 !\n");
	printk(KERN_ERR "Printk Level 3 !\n");
	printk(KERN_WARNING "Printk Level 4 !\n");
	printk(KERN_NOTICE "Printk Level 5 !\n");
	printk(KERN_INFO "Printk Level 6 !\n");
	printk(KERN_DEBUG "Printk Level 7 !\n");
	return 0;
}

static void __exit printkalllevel_exit(void)
{
	
	printk(KERN_EMERG "Printk Level 0 !\n");
	printk(KERN_ALERT "Printk Level 1 !\n");
	printk(KERN_CRIT "Printk Level 2 !\n");
	printk(KERN_ERR "Printk Level 3 !\n");
	printk(KERN_WARNING "Printk Level 4 !\n");
	printk(KERN_NOTICE "Printk Level 5 !\n");
	printk(KERN_INFO "Printk Level 6 !\n");
	printk(KERN_DEBUG "Printk Level 7 !\n");
}

module_init(printkalllevel_init);
module_exit(printkalllevel_exit);

MODULE_AUTHOR("Winlin");
MODULE_DESCRIPTION("Kernel Module to print all printk level");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("PrintkAllLevel");

/* cat > /proc/sys/kernel/print 回车 再输入8 再按 Ctrl+D 来修改printk的默认等级 */
