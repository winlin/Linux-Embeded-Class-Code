#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("Dual BSD/GPL");

static short mpshort = 1;
static int mpint = 500;
static long mplong = 9999;
static char* mpstring = "mpstring";

module_param(mpshort, short, S_IRWXU);
MODULE_PARM_DESC(mpshort, "One short integer");
module_param(mpint, int, S_IRUSR);
MODULE_PARM_DESC(mpint,"One integer");
module_param(mplong, long, S_IRWXU);
MODULE_PARM_DESC(mplong,"One long integer");
module_param(mpstring, charp, S_IRWXU);
MODULE_PARM_DESC(mpstring,"One charp");


static int __init modparam_init(void)
{
	printk(KERN_ALERT "Entry modparam_init \n");
	printk(KERN_ALERT "mpshort = %d\n",mpshort);
	printk(KERN_ALERT "mpint = %d \n",mpint);
	printk(KERN_ALERT "mpling = %ld \n",mplong);
	printk(KERN_ALERT "mpstring = %s\n",mpstring);
	
	return 0;
}

static void __exit modparam_exit(void)
{
	printk(KERN_ALERT "Entry modparam_exit \n");
	printk(KERN_ALERT "mpshort = %d\n",mpshort);
	printk(KERN_ALERT "mpint = %d \n",mpint);
	printk(KERN_ALERT "mpling = %ld \n",mplong);
	printk(KERN_ALERT "mpstring = %s\n",mpstring);	
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_VERSION("0.0.1");
MODULE_AUTHOR("Winlin");
MODULE_ALIAS("ModParam");
MODULE_DESCRIPTION("Kernel Modules Parameters Demo");

/* 
   insmod lib/modules/2.6.28.6/kernel/drivers/modparam mpint=1234 mshort=11
   mpstring="hello world" mplong=12341234

   We only can set the initlization rights to the parameters files in initlization;
   but once the parameters files exit,we can change the content of the files.
*/
