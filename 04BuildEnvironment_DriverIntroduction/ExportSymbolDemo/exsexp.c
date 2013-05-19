/*
   Define two functional methods .
   Export them for called by other modules
*/
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

int exsexp_mult(int x,int y)
{
	printk(KERN_ALERT "Entry exsexp_mult!\n");
	return (x*y);
}

int exsexp_divd(int x,int y)
{
	printk(KERN_ALERT "Entry exsexp_divd!\n");
	return (x/y);
}

EXPORT_SYMBOL(exsexp_mult);
EXPORT_SYMBOL(exsexp_divd);
