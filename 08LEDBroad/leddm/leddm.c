// filename: leddm.c
//           LED Dot Matrix device driver
//           Author: Oliver
//           Date: 2011-01-02
//           Rivision:
//                     2011-01-02----original draft by Oliver

#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/fcntl.h>
#include <linux/mutex.h>
#include <linux/device.h>

#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include <asm/gpio.h>
#include <plat/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <plat/gpio-bank-c.h>
#include <plat/gpio-bank-f.h>

#include "leddm.h"
#include "leddmio.h"
#include "CharDM.h"

MODULE_LICENSE("Dual BSD/GPL");

static u32 leddm_major = LEDDM_MAJOR;
static u32 leddm_minor = LEDDM_MINOR;

static u32 leddm_port_table[] =
{
    LEDDM_PORT_A,
    LEDDM_PORT_B,
    LEDDM_PORT_C,
    LEDDM_PORT_D,
    LEDDM_PORT_E,
    LEDDM_PORT_R1,
    LEDDM_PORT_G1,
    LEDDM_PORT_R2,
    LEDDM_PORT_G2,
    LEDDM_PORT_LAT,
    LEDDM_PORT_CLK,
    LEDDM_ARRAY_END,
};

struct leddm_cdev
{
    struct class *leddm_cls;
    struct device *leddm_dev;
    struct cdev cdev;

    struct mutex mut;
    struct timer_list timer_update_display;
    struct timer_list timer_update_data;

    int current_jiffies_update_data;
    int is_update_display_on;
    int is_update_data_on;
    int is_del_timer;
	
    u8 display_buf0[ LED_DM_HEIGHT * LED_DM_WIDTH ];
    u8 display_buf1[ LED_DM_HEIGHT * LED_DM_WIDTH ];
    u8 display_buf_toggle;
    u8 *pdata_display;
    u8 *pdata_data;
    int display_speed;
	
    u8 *pdata_buf;
    u32 data_buf_width;
    u32 data_buf_height;
    u32 data_buf_pos;
};
static struct leddm_cdev *leddm_cdevp = NULL;

static void leddm_timer_handler_update_display(unsigned long data);
static void leddm_timer_handler_update_data(unsigned long data);

void leddm_driver_port_init(void)
{
    int i = 0;

    while( LEDDM_ARRAY_END != ( leddm_port_table[i] ) )
    {
        gpio_direction_output( leddm_port_table[i], LEDDM_SIGNAL_LOW );
        s3c_gpio_setpull( leddm_port_table[i], S3C_GPIO_PULL_NONE );
        if( LEDDM_PORT_E >= leddm_port_table[i] )
        {
            gpio_set_value( leddm_port_table[i], LEDDM_SIGNAL_HIGH );
        }
        else
        {
            gpio_set_value( leddm_port_table[i], LEDDM_SIGNAL_LOW );
        }
        i++;
    }
}

void leddm_update_display_send_line( int line, int flag ) 
{ 
    unsigned int i;
    unsigned char uch1;
    unsigned char uch2;

    for( i = 0; i < LED_DM_WIDTH; i++ )
    {
        if( LEDDM_UPDATE_NORMAL == flag )
        {
            uch1 = *( leddm_cdevp->pdata_display + line * LED_DM_WIDTH + i );
            uch2 = *( leddm_cdevp->pdata_display + ( line + 16 ) * LED_DM_WIDTH + i );
        }
        else if( LEDDM_UPDATE_CLEAR == flag )
        {
            uch1 = CHAR_COLOR_CLEAR;
            uch2 = CHAR_COLOR_CLEAR;
        }
        else
        {
            uch1 = CHAR_COLOR_ORANGE;
            uch2 = CHAR_COLOR_ORANGE;
        }

        switch(uch1)
        {
        case CHAR_COLOR_CLEAR:
            gpio_set_value( LEDDM_PORT_R1, LEDDM_SIGNAL_HIGH );
            gpio_set_value( LEDDM_PORT_G1, LEDDM_SIGNAL_HIGH );
            break;
        case CHAR_COLOR_RED:
	case CHAR_COLOR_GREEN:
            gpio_set_value( LEDDM_PORT_R1, LEDDM_SIGNAL_LOW );
            gpio_set_value( LEDDM_PORT_G1, LEDDM_SIGNAL_HIGH );
            break;
	    /*case CHAR_COLOR_GREEN:
            gpio_set_value( LEDDM_PORT_R1, LEDDM_SIGNAL_HIGH );
            gpio_set_value( LEDDM_PORT_G1, LEDDM_SIGNAL_LOW );
            break;*/
        case CHAR_COLOR_ORANGE:
            gpio_set_value( LEDDM_PORT_R1, LEDDM_SIGNAL_LOW );
            gpio_set_value( LEDDM_PORT_G1, LEDDM_SIGNAL_LOW );
            break;
        default:
            break;
        }
        switch(uch2)
        {
        case CHAR_COLOR_CLEAR:
            gpio_set_value( LEDDM_PORT_R2, LEDDM_SIGNAL_HIGH );
            gpio_set_value( LEDDM_PORT_G2, LEDDM_SIGNAL_HIGH );
            break;
        case CHAR_COLOR_RED:
	case CHAR_COLOR_GREEN:
            gpio_set_value( LEDDM_PORT_R2, LEDDM_SIGNAL_LOW );
            gpio_set_value( LEDDM_PORT_G2, LEDDM_SIGNAL_HIGH );
            break;
	    /*case CHAR_COLOR_GREEN:
            gpio_set_value( LEDDM_PORT_R2, LEDDM_SIGNAL_HIGH );
            gpio_set_value( LEDDM_PORT_G2, LEDDM_SIGNAL_LOW );
            break;*/
        case CHAR_COLOR_ORANGE:
            gpio_set_value( LEDDM_PORT_R2, LEDDM_SIGNAL_LOW );
            gpio_set_value( LEDDM_PORT_G2, LEDDM_SIGNAL_LOW );
            break;
        default:
            break;
        }
        gpio_set_value( LEDDM_PORT_CLK, LEDDM_SIGNAL_LOW );
        gpio_set_value( LEDDM_PORT_CLK, LEDDM_SIGNAL_HIGH );
    }  
} 

void leddm_update_display_screen(int flag) 
{
    int i;

    mutex_lock( &( leddm_cdevp->mut ) );

    gpio_set_value( LEDDM_PORT_A, LEDDM_SIGNAL_HIGH );
    gpio_set_value( LEDDM_PORT_B, LEDDM_SIGNAL_HIGH );
    gpio_set_value( LEDDM_PORT_C, LEDDM_SIGNAL_HIGH );
    gpio_set_value( LEDDM_PORT_D, LEDDM_SIGNAL_HIGH );
    gpio_set_value( LEDDM_PORT_E, LEDDM_SIGNAL_HIGH );

    for( i = 0; i < 17; i++ )
    {
        gpio_set_value( LEDDM_PORT_E, LEDDM_SIGNAL_LOW );

        if( 16 == i )
        {
            leddm_update_display_send_line( 15 , flag );
        }
        else
        {
            leddm_update_display_send_line( i , flag );
        }

        gpio_set_value( LEDDM_PORT_A, (i & 0x01));
        gpio_set_value( LEDDM_PORT_B, ((i & 0x02) >> 1));
        gpio_set_value( LEDDM_PORT_C, ((i & 0x04) >> 2));
        gpio_set_value( LEDDM_PORT_D, ((i & 0x08) >> 3));

        gpio_set_value( LEDDM_PORT_LAT, LEDDM_SIGNAL_LOW );
        gpio_set_value( LEDDM_PORT_LAT, LEDDM_SIGNAL_HIGH );

        gpio_set_value( LEDDM_PORT_E, LEDDM_SIGNAL_HIGH );
    }

    mutex_unlock( &( leddm_cdevp->mut ) );
}

void leddm_display_buffer_toggle(void)
{
    mutex_lock( &( leddm_cdevp->mut ) );

    if( FALSE == leddm_cdevp->display_buf_toggle )
    {
        leddm_cdevp->display_buf_toggle = TRUE;
        leddm_cdevp->pdata_display = leddm_cdevp->display_buf1;
        leddm_cdevp->pdata_data = leddm_cdevp->display_buf0;
    }
    else
    {
        leddm_cdevp->display_buf_toggle = FALSE;
        leddm_cdevp->pdata_display = leddm_cdevp->display_buf0;
        leddm_cdevp->pdata_data = leddm_cdevp->display_buf1;
    }

    mutex_unlock( &( leddm_cdevp->mut ) );
}

void leddm_update_data(void)
{
    int i, j;

    for( j = 0; j < LED_DM_HEIGHT; j++ )
    {
        for( i = 0; i < ( LED_DM_WIDTH - 1 ); i++ )
        {
            *( leddm_cdevp->pdata_data + j * LED_DM_WIDTH + i ) = \
                *( leddm_cdevp->pdata_display + j * LED_DM_WIDTH + i + 1 );
        }
        if( leddm_cdevp->data_buf_width > leddm_cdevp->data_buf_pos )
        {
            *( leddm_cdevp->pdata_data + j * LED_DM_WIDTH + i ) = \
                *( leddm_cdevp->pdata_buf + j * leddm_cdevp->data_buf_width + \
                leddm_cdevp->data_buf_pos );
        }
        else
        {
            *( leddm_cdevp->pdata_data + j * LED_DM_WIDTH + i ) = CHAR_COLOR_CLEAR;
        }
    }
    if( leddm_cdevp->data_buf_pos >= ( leddm_cdevp->data_buf_width + LED_DM_WIDTH ) )
    {
        leddm_cdevp->data_buf_pos = 0;
    }
    else
    {
        leddm_cdevp->data_buf_pos ++;
    }
 
    leddm_display_buffer_toggle();
}

void leddm_driver_init(void)
{
    leddm_driver_port_init();
 
    leddm_cdevp->display_buf_toggle = FALSE;
    leddm_cdevp->pdata_display = leddm_cdevp->display_buf0;
    leddm_cdevp->pdata_data = leddm_cdevp->display_buf1;

    leddm_cdevp->pdata_buf =(u8*) CharDM;
    leddm_cdevp->data_buf_width = CharDM_Width;
    leddm_cdevp->data_buf_height = CharDM_Height;
    leddm_cdevp->data_buf_pos = 0;

    mutex_init( &( leddm_cdevp->mut ) );

    leddm_cdevp->display_speed= LEDDM_JIFFIES_UPDATE_DISPLAY;
    
    init_timer( &( leddm_cdevp->timer_update_display ) );
    leddm_cdevp->timer_update_display.function = leddm_timer_handler_update_display;
    leddm_cdevp->timer_update_display.expires = jiffies + ( HZ / leddm_cdevp->display_speed);
    add_timer( &( leddm_cdevp->timer_update_display ) );
    leddm_cdevp->is_update_display_on = TRUE;

    init_timer( &( leddm_cdevp->timer_update_data ) );
    leddm_cdevp->timer_update_data.function = leddm_timer_handler_update_data;
    leddm_cdevp->timer_update_data.expires = jiffies + ( HZ / leddm_cdevp->display_speed );
    add_timer( &( leddm_cdevp->timer_update_data ) );
    leddm_cdevp->is_update_data_on = TRUE;

    leddm_cdevp->is_del_timer=0;
}

static void leddm_timer_handler_update_display(unsigned long data)
{
    if( !( leddm_cdevp->is_update_display_on ) )
    {
        goto reconfig_timer_update_display;
    }

    leddm_update_display_screen( LEDDM_UPDATE_NORMAL );

reconfig_timer_update_display:
    leddm_cdevp->timer_update_display.expires = jiffies + HZ / leddm_cdevp->display_speed;
    mod_timer( &( leddm_cdevp->timer_update_display ), leddm_cdevp->timer_update_display.expires );
}

static void leddm_timer_handler_update_data(unsigned long data)
{
    if( !( leddm_cdevp->is_update_data_on ) )
    {
        goto reconfig_timer_update_data;
    }

    leddm_update_data();

reconfig_timer_update_data:
    leddm_cdevp->timer_update_data.expires = jiffies + ( HZ / leddm_cdevp->display_speed);
    mod_timer( &( leddm_cdevp->timer_update_data ), leddm_cdevp->timer_update_data.expires );
}

static int leddm_open(struct inode *inode, struct file *filp)
{
    leddm_info( "Entry leddm_open !\n" );

    filp->private_data = leddm_cdevp;

    return nonseekable_open( inode, filp );
}

static int leddm_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct leddm_cdev *pcdevp = filp->private_data;
    void __user *parg=(void __user *)arg;
    int ret=-EFAULT;
    
    leddm_info( "Entry leddm_ioctl !\n" );
    
    if(LEDDM_IOCTL_MAGIC!=_IOC_TYPE(cmd)){
	    printk(KERN_ALERT "ioctl command mismatch\n");
	    return ret;
    }
    if(LEDDM_IOCTL_MINNR>_IOC_NR(cmd)||LEDDM_IOCTL_MAXNR<_IOC_NR(cmd)){
	    printk(KERN_ALERT "commnd number mismatch");
	    return ret;
    }
    if(_IOC_READ&_IOC_DIR(cmd)){
	    if(!access_ok(VERIFY_WRITE, parg, _IOC_SIZE(cmd))){
		    printk(KERN_ALERT "Cant't access the user memory!\n");
		    return ret;
	    }
    }

    //handle ioctl command
    switch(cmd){
    case LEDDM_IOCTL_STOP:
	    del_timer(&pcdevp->timer_update_display);
	    del_timer(&pcdevp->timer_update_data);
	    pcdevp->is_del_timer=1;
	    break;
    case LEDDM_IOCTL_START:
	    if(pcdevp->is_del_timer){
		    add_timer(&pcdevp->timer_update_display);
		    add_timer(&pcdevp->timer_update_data);
		    pcdevp->is_del_timer=0;
	    }
	    break;
    case LEDDM_IOCTL_PAUSE:
	    pcdevp->is_update_data_on=FALSE;
	    break;
    case LEDDM_IOCTL_RESUME:
	    pcdevp->is_update_data_on=TRUE;
	    break;
    case LEDDM_IOCTL_SPEEDUP:
	    pcdevp->display_speed+=10;
	    if(pcdevp->display_speed>HZ)
		    pcdevp->display_speed=HZ-5;
	    break;
    case LEDDM_IOCTL_SPEEDDOWN:
	    pcdevp->display_speed-=5;
	    if(pcdevp->display_speed<20)
		    pcdevp->display_speed=20;
	    break;
    case LEDDM_IOCTL_SETDATAHEIGHT:
	    pcdevp->data_buf_height=arg;
	    break;
    case LEDDM_IOCTL_SETDATAWIDTH:
	    pcdevp->data_buf_width=arg;
	    break;
    case LEDDM_IOCTL_SETDATA:
	    mutex_lock( &( pcdevp->mut ) );
	    if(pcdevp->pdata_buf!=(u8*)CharDM && pcdevp->pdata_buf!=NULL)
		    kfree(pcdevp->pdata_buf);
	    
	    pcdevp->pdata_buf=kmalloc(pcdevp->data_buf_width*pcdevp->data_buf_height,GFP_KERNEL);
	    if(0>copy_from_user(pcdevp->pdata_buf, parg, pcdevp->data_buf_width*pcdevp->data_buf_height)){
		    printk(KERN_ALERT "Copy form user space failed\n");
		    return ret;
	    }
	    mutex_unlock( &( pcdevp->mut ) );
    default:
	    break;
    }
    return 0;
}

static int leddm_release(struct inode *inode, struct file *filp)
{
    struct leddm_cdev *pcdevp = filp->private_data;
    
    leddm_info( "Entry leddm_release !\n" );

    return 0;
}

static struct file_operations leddm_fops = 
{
    .owner  = THIS_MODULE,
    .open   = leddm_open,
    .llseek = no_llseek,
    .ioctl  = leddm_ioctl,
    .release= leddm_release,
};

static int leddm_setup_cdev(struct leddm_cdev *pcdevp, int index)
{
    int ret = 0;
    dev_t dev = MKDEV( leddm_major, leddm_minor );

    cdev_init( &( pcdevp->cdev ), &leddm_fops );
    ret = cdev_add( &( pcdevp->cdev ), dev, 1 );
    if( 0 > ret )
    {
        leddm_dbg( "Failure to add cdev Major: %d\tMinor: %d\tErrorno: %d\n", \
                     leddm_major, leddm_minor, ret);
    }
    else
    {
        leddm_info( "Success to add cdev Major: %d\tMinor: %d\n", \
                     leddm_major, leddm_minor );
    }

    return ret;
}

static int __init leddm_init(void)
{
    int ret = 0;
    dev_t dev = 0;

    leddm_info( "Entry leddm_init !\n" );

    if( leddm_major )
    {
        dev = MKDEV( leddm_major, leddm_minor );
        ret = register_chrdev_region( dev, LEDDM_COUNT, LEDDM_NAME );
    }
    else
    {
        ret = alloc_chrdev_region( &dev, leddm_minor, LEDDM_COUNT, LEDDM_NAME );
        leddm_major = MAJOR( dev );
    }
    if( 0 > ret )
    {
        leddm_dbg( "Cann't get major: %d !\n", leddm_major );
        return ret;
    }
    leddm_info( "Success to get major: %d !\n", leddm_major );

    leddm_cdevp = kmalloc( sizeof( struct leddm_cdev ), GFP_KERNEL );
    if( !leddm_cdevp )
    {
        ret = -ENOMEM;
        goto fail_kmalloc;
    }
    memset( leddm_cdevp, 0, sizeof( struct leddm_cdev ) );

    leddm_driver_init();

    ret = leddm_setup_cdev( leddm_cdevp, 0 );
    if( 0 == ret )
    {
        leddm_cdevp->leddm_cls = class_create( THIS_MODULE, LEDDM_CLASSNAME );
        if( IS_ERR( leddm_cdevp->leddm_cls ) )
        {
            leddm_dbg( "Cann't create device class !\n" );
            ret = -EPERM;
            goto fail_class_create;
        }
        leddm_info( "Success to create device class !\n" );
        leddm_cdevp->leddm_dev = device_create( leddm_cdevp->leddm_cls, NULL, dev, NULL, LEDDM_NAME );
        leddm_info( "Success to create device inode !\n" );
        return ret;
    }

fail_class_create:
    kfree( leddm_cdevp );

fail_kmalloc:
    unregister_chrdev_region( dev, LEDDM_COUNT );

    return ret;
}

static void __exit leddm_exit(void)
{
    leddm_info( "Entry leddm_exit !\n" );

    device_destroy( leddm_cdevp->leddm_cls, MKDEV( leddm_major, leddm_minor ) );
    class_destroy( leddm_cdevp->leddm_cls );

    cdev_del( &( leddm_cdevp->cdev ) );

    del_timer( &( leddm_cdevp->timer_update_data ) );
    del_timer( &( leddm_cdevp->timer_update_display ) );
    if(leddm_cdevp->pdata_buf!=(u8*)CharDM && leddm_cdevp->pdata_buf!=NULL)
	    kfree(leddm_cdevp->pdata_buf);
   
    kfree( leddm_cdevp );

    unregister_chrdev_region( MKDEV( leddm_major, leddm_minor ), LEDDM_COUNT );
}

module_init(leddm_init);
module_exit(leddm_exit);

MODULE_AUTHOR("Oliver");
MODULE_DESCRIPTION("LED Dot Matrix Experiment");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("LED Dot Matrix Device Driver");
