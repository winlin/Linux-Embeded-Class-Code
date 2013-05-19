#include <linux/init.h>
#include <linux/module.h>

static int __init helloworld_init(void)
{
	printk(KERN_ALERT "init: THE BAD WORLD \n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	printk(KERN_ALERT "exit: THE BAD WORLD \n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
