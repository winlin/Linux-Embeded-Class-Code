#include <linux/init.h>
#include <linux/module.h>

#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");

#define EMPLOYEE_NUMBEROF	5

static struct list_head employee_list;

struct employee {
	char name[20];
	int number;
	int salary;
	struct list_head list;
};

static struct employee *employeep;
static struct employee *employeep_tmp;
static struct list_head *pos;

static int __init listdemo_init(void)
{
	int i;

	printk(KERN_ALERT "Entry listdemo_init !\n");

	// initialize list head
	INIT_LIST_HEAD( &employee_list );

	// allocate memory for employee array
	employeep = kmalloc( sizeof( struct employee ) * EMPLOYEE_NUMBEROF, GFP_KERNEL );
	memset( employeep, 0, sizeof( struct employee ) * EMPLOYEE_NUMBEROF );

	// fill data
	for( i = 0; i < EMPLOYEE_NUMBEROF; i++ ) {
		sprintf( employeep[ i ].name, "Employee%d", i+1 );
		employeep[ i ].number = 1001 + i;
		employeep[ i ].salary = 50000 + 5000 * i;
		list_add(  &(employeep[ i ].list), &employee_list );
		//list_add_tail( &(employeep[ i ].list), &employee_list );
	}

	// use list_for_each print all members
	list_for_each( pos, &employee_list ) {
		employeep_tmp = list_entry( pos, struct employee, list );
		printk(KERN_ALERT "Employ Name: %s\tNumber: %d\tSalary: %d \n", employeep_tmp->name, employeep_tmp->number, employeep_tmp->salary);
	}

	return 0;
}

static void __exit listdemo_exit(void)
{
	int i;

	printk(KERN_ALERT "Entry listdemo_exit !\n");

	// delete list members
	for( i = 0; i < EMPLOYEE_NUMBEROF; i++ ) {
		list_del( &(employeep[ i ].list) );
	}

	// free memory
	kfree( employeep );
}

module_init( listdemo_init );
module_exit( listdemo_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Demo for struct list_head");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("ListDemo");

