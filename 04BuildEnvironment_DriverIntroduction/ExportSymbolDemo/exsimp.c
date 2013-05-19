/*
  Call the exsexp_mult() and exsexp_divd()
  in the exsexp.ko module file 
 */
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

extern int exsexp_mult(int,int);
extern int exsexp_divd(int,int);

static int __init exsimp_init(void)
{
	int mulval;
	printk(KERN_ALERT "Entry exsimp_init!\n");
	mulval=exsexp_mult(5,8);
	printk(KERN_ALERT "mulval=%d\n",mulval);
	return 0;
}

static void __exit exsimp_exit(void)
{
	int divdval;
	printk(KERN_ALERT "Entry exsimp_exit!\n");
	divdval=exsexp_divd(100,5);
	printk(KERN_ALERT "divdval=%d\n",divdval);
}

module_init(exsimp_init);
module_exit(exsimp_exit);

MODULE_AUTHOR("Winlin");
MODULE_DESCRIPTION("Kernel export symbol demo");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("Export Symble");

