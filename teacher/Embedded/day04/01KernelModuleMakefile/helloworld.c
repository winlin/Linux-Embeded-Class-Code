#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

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

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("My Second Kernel Module");
MODULE_VERSION("0.0.2");
MODULE_ALIAS("SecondHelloWorld");

