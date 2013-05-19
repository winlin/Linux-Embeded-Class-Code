#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define BARK_TIME	5 // time interval
#define BARK_TIMES	5 // bark times

static struct timer_list bigdog_timer;
static int bark_count = 0;

static void bigdog_timer_handler(unsigned long data)
{
	printk(KERN_ALERT "Entry bigdog_timer_handler !\n");

	if( bark_count < BARK_TIMES ) {
		printk(KERN_ALERT "Bigdog barking times: %d \n", ++bark_count);

		bigdog_timer.expires = jiffies + ( BARK_TIME * HZ );
		add_timer( &bigdog_timer );
	}
}

static int __init bigdog_timer_init(void)
{
	printk(KERN_ALERT "Entry bigdog_timer_init !\n");

	// initialize timer_list
	init_timer( &bigdog_timer );
	bigdog_timer.expires = jiffies + ( BARK_TIME * HZ );
	bigdog_timer.function = bigdog_timer_handler;

	// add timer_list to kernel
	add_timer( &bigdog_timer );

	return 0;
}

static void __exit bigdog_timer_exit(void)
{
	printk(KERN_ALERT "Entry bigdog_timer_exit !\n");

	del_timer( &bigdog_timer );
}

module_init( bigdog_timer_init );
module_exit( bigdog_timer_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Kernel Timer List Demo");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("KernelTimer");

