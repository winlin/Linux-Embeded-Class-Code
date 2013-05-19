#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");

#define EMPLOYEE_NUMBEROF    5

static struct list_head employee_list;

struct employee{
	char name[20];
	int number;
	int salary;
	struct list_head list;
};

static struct employee *employeep = NULL;
static struct employee *employeep_tmp = NULL;
static struct list_head *pos = NULL;
static struct list_head *n = NULL;

static int __init listdemo_init(void)
{
	int i;
	printk(KERN_ALERT "Entry the listdemo_init\n");
	
	//initlize list head
	INIT_LIST_HEAD( &employee_list );
	//allocate memory for employee array
	employeep = kmalloc( sizeof( struct employee ) * EMPLOYEE_NUMBEROF, GFP_KERNEL );
	memset( employeep, 0, sizeof( struct employee )* EMPLOYEE_NUMBEROF );
	
	//fill data
	for(i=0; i < EMPLOYEE_NUMBEROF; i++){
		sprintf( employeep[ i ].name, "Employee%d", i);
		employeep[ i ].number = 1001 + i;
		employeep[ i ].salary = 50000 + i * 50000;
		
		list_add( &(employeep[ i ].list), &employee_list ); 
		//list_add_tail( &(employeep[ i ].list), &employee_list);
	}

	//use list_for_each print all members
	list_for_each_safe( pos, n, &employee_list ){
		employeep_tmp = list_entry( pos, struct employee, list );
		printk(KERN_ALERT "Employee Name:%s\t Number:%d\t Salary:%d\n",
		       employeep_tmp->name, employeep_tmp->number, employeep_tmp->salary );
	}
	
	return 0;
}

static void __exit listdemo_exit(void)
{
	int i;
	printk(KERN_ALERT "Entry listdemo_exit\n");
	
	//delete list members
	//for( i=0; i < EMPLOYEE_NUMBEROF ; ++i ){
	//	list_del( &(employeep[i].list) );
	//}
	pos=n=NULL;
	i=0;
	/* 
	   We call list_for_each_safe() just to demo the use of for_each();
	   If we want to free the memory ,we only need to call kfree(employeep).
	 */
	list_for_each_safe(pos, n, &employee_list){
		printk(KERN_ALERT "i=%d \n", ++i);
		list_del(pos);
	}
	printk(KERN_ALERT "list_del() over\n");
	//free memory
	kfree( employeep );
}

module_init( listdemo_init );
module_exit( listdemo_exit );

MODULE_ALIAS( "List Demo" );
MODULE_AUTHOR( "Winlin" );
MODULE_DESCRIPTION( "List usage Demo");
MODULE_VERSION( "0.0.1" );

