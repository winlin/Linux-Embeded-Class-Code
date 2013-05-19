// filename: leddm.h
//           some definitions used by LED Dot Matrix device driver
//           Author: Oliver
//           Date: 2011-01-02
//           Rivision:
//                     2011-01-02----original draft by Oliver

#ifndef __LEDDM_H__
#define __LEDDM_H__

#ifdef LEDDMDBG
#define leddm_dbg( fmt, arg... ) printk(KERN_ALERT "LEDDM: " fmt, ##arg)
#define leddm_info( fmt, arg... ) printk(KERN_INFO "LEDDM: " fmt, ##arg)
#else
#define leddm_dbg( fmt, arg... )
#define leddm_info( fmt, arg... )
#endif

#ifndef TRUE
#define TRUE                            1
#define FALSE                           0
#endif

#define LEDDM_ARRAY_END                 0x55aaaa55

#define LEDDM_MAJOR                     0
#define LEDDM_MINOR                     0
#define LEDDM_COUNT                     1
#define LEDDM_NAME                      "leddm"
#define LEDDM_CLASSNAME                 "leddm_class"

#define LEDDM_JIFFIES_UPDATE_DISPLAY    50
#define LEDDM_JIFFIES_UPDATE_DATA       50

#define LEDDM_PORT_A                    S3C64XX_GPL(0)
#define LEDDM_PORT_B                    S3C64XX_GPL(1)
#define LEDDM_PORT_C                    S3C64XX_GPL(2)
#define LEDDM_PORT_D                    S3C64XX_GPL(3)
#define LEDDM_PORT_E                    S3C64XX_GPL(4)
#define LEDDM_PORT_R1                   S3C64XX_GPK(8)
#define LEDDM_PORT_G1                   S3C64XX_GPK(9)
#define LEDDM_PORT_R2                   S3C64XX_GPK(10)
#define LEDDM_PORT_G2                   S3C64XX_GPK(11)
#define LEDDM_PORT_LAT                  S3C64XX_GPK(12)
#define LEDDM_PORT_CLK                  S3C64XX_GPK(13)

#endif
