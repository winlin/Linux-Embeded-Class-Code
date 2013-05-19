// filename: leddmio.h
//           some definitions of IOCTL used by LED Dot Matrix device driver and test applications
//           Author: Oliver
//           Date: 2011-01-02
//           Rivision:
//                     2011-01-02----original draft by Oliver

#ifndef __LEDDMIO_H__
#define __LEDDMIO_H__

#define LEDDM_LIGHT_ON                  0
#define LEDDM_LIGHT_OFF                 1

#define LEDDM_SIGNAL_LOW                0
#define LEDDM_SIGNAL_HIGH               1

#define LEDDM_UPDATE_NORMAL             0
#define LEDDM_UPDATE_CLEAR              1

#define CHAR_COLOR_CLEAR                0
#define CHAR_COLOR_RED                  1
#define CHAR_COLOR_GREEN                2
#define CHAR_COLOR_ORANGE               3

#define LED_DM_WIDTH                    64
#define LED_DM_HEIGHT                   32

#define LEDDM_COMMAND_STOP              0x80
#define LEDDM_COMMAND_START             0x81
#define LEDDM_COMMAND_PAUSE             0x82
#define LEDDM_COMMAND_RESUME            0x83
#define LEDDM_COMMAND_SPEEDUP           0x84
#define LEDDM_COMMAND_SPEEDDOWN         0x85
#define LEDDM_COMMAND_SETDATAWIDTH      0x86
#define LEDDM_COMMAND_SETDATAHEIGHT     0x87
#define LEDDM_COMMAND_SETDATA           0x88

#define LEDDM_IOCTL_MAGIC               'l'
#define LEDDM_IOCTL_MINNR               LEDDM_COMMAND_STOP
#define LEDDM_IOCTL_MAXNR               LEDDM_COMMAND_SETDATA

#define LEDDM_IOCTL_STOP                _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_STOP)
#define LEDDM_IOCTL_START               _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_START)
#define LEDDM_IOCTL_PAUSE               _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_PAUSE)
#define LEDDM_IOCTL_RESUME              _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_RESUME)
#define LEDDM_IOCTL_SPEEDUP             _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_SPEEDUP)
#define LEDDM_IOCTL_SPEEDDOWN           _IO(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_SPEEDDOWN)
#define LEDDM_IOCTL_SETDATAWIDTH        _IOW(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_SETDATAWIDTH, int)
#define LEDDM_IOCTL_SETDATAHEIGHT       _IOW(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_SETDATAHEIGHT, int)
#define LEDDM_IOCTL_SETDATA             _IOW(LEDDM_IOCTL_MAGIC, LEDDM_COMMAND_SETDATA, unsigned char *)

#endif
