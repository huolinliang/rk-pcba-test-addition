#define LOG_TAG "Backlight"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <pthread.h>
#include "language.h"
#include "test_case.h"
#include "backlight_test.h"

#define BACKLIGHT_PATH    "/sys/class/backlight/rk28_bl/brightness"
#define HIGH_VALUE        "180"
#define LOW_VALUE         "60"

void* backlight_test(void *argv)
{
    int ret;
    int result;
    int fd;
    int value;
    char buffer[3];
	struct testcase_info *tc_info = (struct testcase_info*)argv;
	ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"%s:[%s]\n",PCBA_BACKLIGHT,"自动调节模式");
    for(;;)
    {
    fd = open(BACKLIGHT_PATH, O_RDWR);
    if (fd < 0){
        printf("open %s failed\n", BACKLIGHT_PATH);
	    ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"%s:[%s]\n",PCBA_BACKLIGHT,"调节失败");
		tc_info->result = -1;
        return argv;
    }
    sleep(4);

	//ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"%s:[%s]\n",PCBA_BACKLIGHT,"低亮度");
    value = write(fd, LOW_VALUE, sizeof(LOW_VALUE));
    sleep(4);
	//ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"%s:[%s]\n",PCBA_BACKLIGHT,"高亮度");
    value = write(fd, HIGH_VALUE, sizeof(HIGH_VALUE));
    close(fd);
    }
    return argv;
}



