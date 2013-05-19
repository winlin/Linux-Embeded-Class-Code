#include <linux/init.h>
#include <linux/module.h>

static int __init helloworld_init(void)
{
	printk(KERN_ALERT "My first Hello world kernel module!\n");

	return 0;
}

static void __exit helloworld_exit(void)
{
	printk(KERN_ALERT "Bye, cruel world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

