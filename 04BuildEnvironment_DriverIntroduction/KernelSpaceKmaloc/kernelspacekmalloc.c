#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static char* buf1 = NULL;
static char* buf2 = NULL;

static int __init kernelspacekmalloc_init(void)
{
	printk(KERN_ALERT "Entry kernelspacekmalloc_init \n");
	buf1=(char*)kmalloc( 100, GFP_KERNEL );
	if( NULL == buf1 ){
		printk(KERN_ALERT "kmalloc failed \n");
		goto failure_to_kmalloc;
	}
	memset( buf1, 0, 100 );
	strcpy( buf1, "Success to allocate memory from kernel 100 B \n" );
	printk( KERN_ALERT "Buffer buf1: %s\n", buf1 );

	buf2=(char*)get_zeroed_page( GFP_KERNEL );
	if( NULL == buf2 ){
		printk(KERN_ALERT "kmalloc failed \n");
		goto failure_get_zeroed_page;
	}
	strcpy( buf2, "Success to allocate Page memory from Kernel \n");
	printk( KERN_ALERT "Buffer buf2: %s\n", buf2 );

	return 0;

failure_get_zeroed_page:
	kfree( buf1 );
	buf1 = NULL;
	return -1;
	
failure_to_kmalloc:
	return -1;

}

static void __exit kernelspacekmalloc_exit(void)
{
	printk( KERN_ALERT "Entry kernelspacekmaloc_eixt() \n");
	kfree( buf1 );
	buf1 = NULL;
	free_page( (unsigned long) buf2 );
	buf2 = NULL;
}

module_init(kernelspacekmalloc_init);
module_exit(kernelspacekmalloc_exit);

MODULE_VERSION("0.0.1");
MODULE_AUTHOR("Winlin");
MODULE_ALIAS("KernelSpaceKmalloc");
MODULE_DESCRIPTION("Kernel Spcae Kmalloc  Demo");

