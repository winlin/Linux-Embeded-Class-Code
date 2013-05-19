#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

extern int exsexp_mult(int, int);
extern int exsexp_divd(int, int);

static int __init exsimp_init(void)
{
	int multval;

	printk(KERN_ALERT "Entry exsimp_init !\n");

	multval = exsexp_mult( 5, 8 );
	printk(KERN_ALERT "multval = %d \n", multval);

	return 0;
}

static void __exit exsimp_exit(void)
{
	int divdval;

	printk(KERN_ALERT "Entry exsimp_exit !\n");

	divdval = exsexp_divd( 100, 5 );
	printk(KERN_ALERT "divdval = %d \n", divdval);
}

module_init(exsimp_init);
module_exit(exsimp_exit);

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Kernel Module Export_Symbol demo");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("ExportSymbolDemo");

