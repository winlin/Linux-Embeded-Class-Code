#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>

#include <linux/types.h>
#include <linux/errno.h>

#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>

MODULE_LICENSE("Dual BSD/GPL");

#define VIRTUALBLOCKDEVICE_MAJOR	0
#define VIRTUALBLOCKDEVICE_MINOR	0
#define VIRTUALBLOCKDEVICE_NAME		"virtualblockdevice"

static u32 virtualblockdevice_major = VIRTUALBLOCKDEVICE_MAJOR;
static u32 virtualblockdevice_minor = VIRTUALBLOCKDEVICE_MINOR;

static struct gendisk *virtualblockdevice_disk;
static struct request_queue *virtualblockdevice_queue;

#define VIRTUALBLOCKDEVICE_DISK_CAPACITY	( 8 * 1024 * 1024 )
static unsigned char virtualblockdevice_data[ VIRTUALBLOCKDEVICE_DISK_CAPACITY ];

static void virtualblockdevice_endbio(struct bio *bio, int errno)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 24 )
	bio_endio( bio, 0, errno );
#else
	bio_endio( bio, errno );
#endif
}

static int virtualblockdevice_make_request(struct request_queue *q, struct bio *bio)
{
	int i;
	struct bio_vec *biovec;
	void *vbd_mem;

	printk(KERN_ALERT "VirtualBlockDevice: Entry virtualblockdevice_make_request !\n");

	if( ( ( bio->bi_sector << 9 ) + bio->bi_size ) > VIRTUALBLOCKDEVICE_DISK_CAPACITY ) {
		printk(KERN_ALERT "VirtualBlockDevice: Failure to make request: start sector: = %llu\t count bytes: = %lu \n", (unsigned long long)bio->bi_sector, (unsigned long)bio->bi_size);
		virtualblockdevice_endbio( bio, -EIO );
		return 0;
	}

	vbd_mem = virtualblockdevice_data + (bio->bi_sector << 9);

	bio_for_each_segment( biovec, bio, i ) {
		void *biovec_mem;

		switch( bio_rw( bio ) ) {
		case READ:
			biovec_mem = kmap( biovec->bv_page ) + biovec->bv_offset;
			memcpy( biovec_mem, vbd_mem, biovec->bv_len );
			kunmap( biovec->bv_page );
			break;
		case WRITE:
			biovec_mem = kmap( biovec->bv_page ) + biovec->bv_offset;
			memcpy( vbd_mem, biovec_mem, biovec->bv_len );
			kunmap( biovec->bv_page );
			break;
		default:
			printk(KERN_ALERT "VirtualBlockDevice: unknown value from bio_rw: %lu !\n", bio_rw( bio ));
			virtualblockdevice_endbio( bio, -EIO );
			return 0;
		}

		vbd_mem += biovec->bv_len;
	}

	virtualblockdevice_endbio( bio, 0 );

	return 0;
}

#if 0
static void virtualblockdevice_do_request(struct request_queue *q)
{
	struct request *req;

	printk(KERN_ALERT "VirtualBlockDevice: Entry virtualblockdevice_do_request !\n");

	while( NULL != ( req = elv_next_request( q ) ) ) {
		if( ( ( req->sector + req->current_nr_sectors ) << 9 ) > VIRTUALBLOCKDEVICE_DISK_CAPACITY ) {
			printk(KERN_ALERT "VirtualBlockDevice: bad request: start sector: = %llu\t sector count: = %lu \n", (unsigned long long) req->sector, (unsigned long)req->current_nr_sectors);
			end_request( req, 0 );
			continue;
		}
		printk(KERN_ALERT "VirtualBlockDevice: request: start sector: = %llu\t sector count: = %lu \n", (unsigned long long) req->sector, (unsigned long)req->current_nr_sectors);

		switch( rq_data_dir( req ) ) {
		case READ:
			memcpy( req->buffer, (virtualblockdevice_data + (req->sector << 9)), (req->current_nr_sectors << 9) );
			end_request( req, 1 );
			break;
		case WRITE:
			memcpy( (virtualblockdevice_data + (req->sector << 9)), req->buffer, (req->current_nr_sectors << 9) );
			end_request( req, 1 );
			break;
		default:
			printk(KERN_ALERT "VirtualBlockDevice: Unknown data direction !\n");
			break;
		}
	}
}
#endif

static int virtualblockdevice_getgeo(struct block_device *dev, struct hd_geometry *geo)
{
	printk(KERN_ALERT "VirtualBlockDevice: Entry virtualblockdevice_getgeo !\n");

	if( VIRTUALBLOCKDEVICE_DISK_CAPACITY < ( 16 * 1024 * 1024 ) ) {
		geo->heads = 1;
		geo->sectors = 1;
	}
	else if( VIRTUALBLOCKDEVICE_DISK_CAPACITY < ( 512 * 1024 * 1024 ) ) {
		geo->heads = 1;
		geo->sectors = 32;
	}
	else if( VIRTUALBLOCKDEVICE_DISK_CAPACITY < ( 16ULL * 1024 * 1024 * 1024 ) ) {
		geo->heads = 32;
		geo->sectors = 32;
	}
	else {
		geo->heads = 255;
		geo->sectors = 63;
	}

	geo->cylinders = ( VIRTUALBLOCKDEVICE_DISK_CAPACITY >> 9 ) / geo->heads / geo->sectors;

	return 0;
}

static struct block_device_operations virtualblockdevice_fops = {
	.owner		= THIS_MODULE,
	.getgeo		= virtualblockdevice_getgeo,
};

static int __init virtualblockdevice_init(void)
{
	int ret;

	printk(KERN_ALERT "VirtualBlockDevice: Entry virtualblockdevice_init !\n");

	ret = register_blkdev( virtualblockdevice_major, VIRTUALBLOCKDEVICE_NAME );
	if( 0 > ret ) {
		printk(KERN_ALERT "VirtualBlockDevice: Failure to register block device: virtualblockdevice ! Major: %d\tErrno: %d !\n", virtualblockdevice_major, ret);
		goto failure_register_blkdev;
	}
	virtualblockdevice_major = ret;
	printk(KERN_ALERT "VirtualBlockDevice: Success to register block device: virtualblockdevice ! Major: %d !\n", virtualblockdevice_major);

	// get request_queue
	virtualblockdevice_queue = blk_alloc_queue( GFP_KERNEL );
	if( !virtualblockdevice_queue ) {
		printk(KERN_ALERT "VirtualBlockDevice: Failure to alloc request_queue !\n");
		ret = -ENOMEM;
		goto failure_alloc_queue;
	}
	printk(KERN_ALERT "VirtualBlockDevice: Success to alloc request_queue !\n");
	blk_queue_make_request( virtualblockdevice_queue, virtualblockdevice_make_request );

	// get gendisk
	virtualblockdevice_disk = alloc_disk( 1 );
	if( !virtualblockdevice_disk ) {
		printk(KERN_ALERT "VirtualBlockDevice: Failure to allocate gendisk !\n");
		ret = -ENOMEM;
		goto failure_alloc_disk;
	}
	printk(KERN_ALERT "VirtualBlockDevice: Success to allocate gendisk !\n");

	// initialize gendisk
	strcpy( virtualblockdevice_disk->disk_name, VIRTUALBLOCKDEVICE_NAME );
	virtualblockdevice_disk->major = virtualblockdevice_major;
	virtualblockdevice_disk->first_minor = virtualblockdevice_minor;
	virtualblockdevice_disk->fops = &virtualblockdevice_fops;
	virtualblockdevice_disk->queue = virtualblockdevice_queue;
	set_capacity( virtualblockdevice_disk, ( VIRTUALBLOCKDEVICE_DISK_CAPACITY >> 9 ) );

	// add gendisk to kernel
	add_disk( virtualblockdevice_disk );

	return 0;

failure_alloc_disk:
	blk_cleanup_queue( virtualblockdevice_queue );
failure_alloc_queue:
	unregister_blkdev( virtualblockdevice_major, VIRTUALBLOCKDEVICE_NAME );
failure_register_blkdev:
	return ret;
}

static void __exit virtualblockdevice_exit(void)
{
	printk(KERN_ALERT "VirtualBlockDevice: Entry virtualblockdevice_exit !\n");

	del_gendisk( virtualblockdevice_disk );
	put_disk( virtualblockdevice_disk );

	blk_cleanup_queue( virtualblockdevice_queue );

	unregister_blkdev( virtualblockdevice_major, VIRTUALBLOCKDEVICE_NAME);
}

module_init( virtualblockdevice_init );
module_exit( virtualblockdevice_exit );

MODULE_AUTHOR("Tarena");
MODULE_DESCRIPTION("Virtual block device driver demo");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("VirtualBlockDevice");

