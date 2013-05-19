#include <linux/init.h>
#include <linux/module.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("Dual BSD/GPL");

#define VIRTUALNETDEVICE_CARD_NAME	"vndcard"

static struct net_device *virtualnetdevice_card;

static int virtualnetdevice_open(struct net_device *dev)
{
	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_open !\n");

	return 0;
}

static int virtualnetdevice_stop(struct net_device *dev)
{
	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_stop !\n");

	return 0;
}

static int virtualnetdevice_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_hard_start_xmit !\n");

	dev_kfree_skb( skb );

	return 0;
}

static int virtualnetdevice_card_init(struct net_device *dev)
{
	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_card_init !\n");

	ether_setup( dev );

	strcpy( dev->name, VIRTUALNETDEVICE_CARD_NAME );
	dev->open = virtualnetdevice_open;
	dev->stop = virtualnetdevice_stop;
	dev->hard_start_xmit = virtualnetdevice_hard_start_xmit;
	
	return 0;
}

static int __init virtualnetdevice_init(void)
{
	int ret;

	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_init !\n");

	virtualnetdevice_card = alloc_etherdev( 0 );
	virtualnetdevice_card->init = virtualnetdevice_card_init;

	ret = register_netdev( virtualnetdevice_card );
	if( 0 > ret ) {
		printk(KERN_ALERT "VirtualNetDevice: Cann't register net device: virtualnetdevice_card ! Errno: %d !\n", ret);
		return ret;
	}
	printk(KERN_ALERT "VirtualNetDevice: Success to register net device: virtualnetdevice_card !\n");

	return 0;
}

static void __exit virtualnetdevice_exit(void)
{
	printk(KERN_ALERT "VirtualNetDevice: Entry virtualnetdevice_exit !\n");

	unregister_netdev( virtualnetdevice_card );
}

module_init( virtualnetdevice_init );
module_exit( virtualnetdevice_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Virtual Network Card Driver");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("VirtualNetDevice");

