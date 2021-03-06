// the definition of IOCTL for LED

#ifndef __IOCTLREALLED_H__
#define __IOCTLREALLED_H__

#define LED_IOCOMMAND_ON	0x80
#define LED_IOCOMMAND_OFF	0x81

#define LED_IOCTL_MAGIC		'l'

#define LED_IOCTL_NR_MIN	LED_IOCOMMAND_ON
#define LED_IOCTL_NR_MAX	LED_IOCOMMAND_OFF

#define LED_IOCTL_ON	_IO(LED_IOCTL_MAGIC, LED_IOCOMMAND_ON)
#define LED_IOCTL_OFF	_IO(LED_IOCTL_MAGIC, LED_IOCOMMAND_OFF)

#endif

