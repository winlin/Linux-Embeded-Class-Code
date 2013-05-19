#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *buf1 = NULL;
static char *buf2 = NULL;

static int __init kernelspacekmalloc_init(void)
{
	printk(KERN_ALERT "Entry kernelspacekmalloc_init !\n");

	buf1 = (char *)kmalloc( 100, GFP_KERNEL );
	if( NULL == buf1 ) {
		printk(KERN_ALERT "Cann't allocate memory using kmalloc !\n");
		goto failure_to_kmalloc;
	}
	memset( buf1, 0, 100 );
	strcpy( buf1, "Success to allocate memory from kernel: 100 bytes !\n");
	printk(KERN_ALERT "Buffer buf1: %s \n", buf1);

	buf2 = (char *)get_zeroed_page( GFP_KERNEL );
	if( NULL == buf2 ) {
		printk(KERN_ALERT "Cann't get a zeroed page using get_zeroed_page !\n");
		goto failure_get_zeroed_page;
	}
	strcpy( buf2, "Success to allocate Page memory from kernel !\n");
	printk(KERN_ALERT "Buffer buf2: %s \n", buf2);

	return 0;

failure_get_zeroed_page:
	kfree( buf1 );
	buf1 = NULL;
failure_to_kmalloc:
	return -1;
}

static void __exit kernelspacekmalloc_exit(void)
{
	printk(KERN_ALERT "Entry kernelspacekmalloc_exit !\n");

	kfree( buf1 );
	buf1 = NULL;
	free_page( (unsigned long) buf2 );
	buf2 = NULL;
}

module_init( kernelspacekmalloc_init );
module_exit( kernelspacekmalloc_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Using kmalloc and get_zeroed_page to allocate memory in kernel space");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("KernelSpaceKMalloc");

