// filename: leddm.c
//           test program to test LED Dot Matrix device driver
//           Author: Oliver
//           Date: 2011-01-02
//           Rivision:
//                     2011-01-02----original draft by Oliver

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/ioctl.h>

#include "leddmio.h"

#define MAX_DISP_TEXT_LEN        128
#define CUR_CHAR_WIDTH           32
#define CUR_CHAR_HEIGHT          32

static unsigned char ssdt[MAX_DISP_TEXT_LEN];     //User's words
static unsigned char sslen;                       //User's words' length
static int cur_char_color;                        //Display words' color
static unsigned char chardmdata[CUR_CHAR_WIDTH / 8 * CUR_CHAR_HEIGHT];
static unsigned char leddmdata[CUR_CHAR_HEIGHT * (MAX_DISP_TEXT_LEN * CUR_CHAR_WIDTH)];
static unsigned char *pleddmdata = NULL;
static int cur_dm_width = 0;
static int cur_dm_height = 0;
static int cur_dm_pos = 0;

char leddm_showmenu(void)
{
    char ret;

    printf("LED Dot Matrix Screen Control demonstration:\n\n");
    printf("        1. Stop Display\n");
    printf("        2. Start Display\n");
    printf("        3. Pause Display\n");
    printf("        4. Resume Display\n");
    printf("        5. Scroll Spedd Up\n");
    printf("        6. Scroll Spedd Down\n");
    printf("        7. Update Display content from disptext.txt\n");
    printf("        8. Exit\n\n");
    printf("    Please select functions(1-7):\n");

    while(1)
    {
        ret = getchar();
        if((ret >= '1') && (ret <= '8'))
        {
            break;
        }
    }

    return ret;
}
/* Get Use's words */
int leddm_get_disp_text(void)
{
    int ft, rdcnt;

    sslen = 0;
    for(rdcnt = 0; rdcnt < MAX_DISP_TEXT_LEN; rdcnt++)
    {
        ssdt[rdcnt] = 0;
    }

    ft = open("disptext.txt", O_RDONLY);
    if(ft < 0)
    {
        printf("Failure to open display text file: disptext.txt !\r\n");
        return ft;
    }

    sslen = lseek(ft, 0, SEEK_END);
    printf("disptext.txt file length: %d \n", sslen);
    lseek(ft, 0, SEEK_SET);
    rdcnt = read(ft, ssdt, sslen);
    printf("read: %d bytes from disptext.txt\n", rdcnt);
    close(ft);

    return rdcnt;
}
/* Get one chinese word's Dot Matrix */
int leddm_get_char_dot_matrix(unsigned char ch, unsigned char cl)
{
    int ret = -1;
    int fk;
    unsigned long offs;
    int rdcnt;

    fk = open("HZK32", O_RDONLY);
    if(fk < 0)
    {
        printf("Failure to open HZK file: HZK32 !\r\n");
        return fk;
    }

    rdcnt = CUR_CHAR_WIDTH / 8 * CUR_CHAR_HEIGHT;
    offs = ((ch - 0xa1) * 94 + (cl - 0xa1)) * rdcnt;

    lseek(fk, offs, SEEK_SET);
    ret = read(fk, chardmdata, rdcnt);
    if(ret != rdcnt)
    {
        ret = -1;
    }

    close(fk);
    return ret;
}
/* Copy one chinese word Dot Matrix into a Dot Matrix Array */
void leddm_get_display_dot_matrix(void)
{
    int i, j, k;
    unsigned char mask;
    unsigned char uch;

    for(i = 0; i < CUR_CHAR_HEIGHT; i++)
    {
        for(j = 0; j < (CUR_CHAR_WIDTH / 8); j++)
        {
            for(k = 0; k < 8; k++)
            {
                mask = 0x80;
                mask = mask >> k;
                uch = chardmdata[i * (CUR_CHAR_WIDTH / 8) + j];
                if(uch & mask)
                {
                    leddmdata[(i * (MAX_DISP_TEXT_LEN * CUR_CHAR_WIDTH)) + (cur_dm_pos + j * 8 + k)] = (unsigned char) cur_char_color;
                }
                else
                {
                    leddmdata[(i * (MAX_DISP_TEXT_LEN * CUR_CHAR_WIDTH)) + (cur_dm_pos + j * 8 + k)] = CHAR_COLOR_CLEAR;
                }
            }
        }
    }

    cur_dm_pos += CUR_CHAR_WIDTH;

    return;
}

/* Parse the user's words: get a word info from ZHK.TXT and copy
   it into leddmdata[] Dot Matrix Array */
int leddm_get_parse_disp_text(void)
{
    int i;
    unsigned char ch;
    unsigned char cl;

    cur_char_color = CHAR_COLOR_RED;
    for(i = 0; i < sslen;)
    {
        ch = ssdt[i++];
        if((ch == 0x0d) || (ch == 0x0a))
        {
            break;
        }
        if(i < sslen)
        {
            cl =ssdt[i++];
        }
        if(ch == '&')
        {
            switch(cl)
            {
            case 'R':
            case 'r':
                cur_char_color = CHAR_COLOR_RED;
                break;
            case 'G':
            case 'g':
                cur_char_color = CHAR_COLOR_GREEN;
                break;
            case 'O':
            case 'o':
                cur_char_color = CHAR_COLOR_ORANGE;
                break;
            default:
		    i--;    // ch=cl ,move to next char 
                break;
            }
        }
        else if(ch >= 0xa1)    // is a chinese word ?
        {
            if(leddm_get_char_dot_matrix(ch, cl) < 0)
            {
                return -1;
            }
            leddm_get_display_dot_matrix();
        }
        else
        {
		i--;       // ch=cl , move to next char
        }
    }

    return sslen;
}

/* Modify the leddmdata[] Dot Matrix becasue in one line (MAX_DISP_TEXT_LEN*CUR_CHAR_WIDTH)
   there are blank space */
void leddm_make_post(void)
{
    unsigned char unch;
    int i, j;

    cur_dm_width = cur_dm_pos;
    cur_dm_height = CUR_CHAR_HEIGHT;
    for(i = 1; i < cur_dm_height; i++)
    {
        for(j = 0; j < cur_dm_width; j++)
        {
            unch = leddmdata[(i * (MAX_DISP_TEXT_LEN * CUR_CHAR_WIDTH)) + j];
            leddmdata[i * cur_dm_width + j] = unch;
        }
    }
}

/* Main Function to get user's word and convert into Dot Matrix
   then copy them into leddmdata[] Dot Matrix Array */
int leddm_get_dm_data(void)
{
    int ret = 0;
    int slen, rdcnt;

    cur_dm_pos = 0;

    ret = leddm_get_disp_text();
    if(ret < 0)
    {
        return ret;
    }
    ret = leddm_get_parse_disp_text();
    leddm_make_post();

    return ret;
}

int main(int argc, char **argv)
{
    int fd;
    char ret;

    fd = open("/dev/leddm0", 0);
    if (fd < 0)
    {
        fd = open("/dev/leddm", 0);
    }
    if (fd < 0)
    {
        printf("Failure to open device: leddm !\r\n");
        exit(1);
    }

    while(1)
    {
        printf("\033[2J");
        printf("\033[0;0H");
        ret = leddm_showmenu();
        switch(ret)
        {
        case '1':
            ioctl(fd, LEDDM_IOCTL_STOP);
            break;
        case '2':
            ioctl(fd, LEDDM_IOCTL_START);
            break;
        case '3':
            ioctl(fd, LEDDM_IOCTL_PAUSE);
            break;
        case '4':
            ioctl(fd, LEDDM_IOCTL_RESUME);
            break;
        case '5':
            ioctl(fd, LEDDM_IOCTL_SPEEDUP);
            break;
        case '6':
            ioctl(fd, LEDDM_IOCTL_SPEEDDOWN);
            break;
        case '7':
            if(leddm_get_dm_data() > 0)
            {
                ioctl(fd, LEDDM_IOCTL_SETDATAWIDTH, cur_dm_width);
                ioctl(fd, LEDDM_IOCTL_SETDATAHEIGHT, cur_dm_height);
                pleddmdata = leddmdata;
                ioctl(fd, LEDDM_IOCTL_SETDATA, pleddmdata);
            }
            break;
        case '8':
            goto outmain;
            break;
        default:
            break;
        }
    }

outmain:
    close(fd);

    return 0;
}
