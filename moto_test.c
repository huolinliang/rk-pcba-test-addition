#define LOG_TAG "Moto"

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
#include "moto_test.h"

#define MOTO_GPIO_NUM        "284"
#define SYS_GPIO_PATH    "/sys/class/gpio/export"
#define MOTO_GPIO_PATH    "/sys/class/gpio/gpio284/direction"
#define HIGH_VALUE        "high"
#define LOW_VALUE         "low"

void* moto_test(void *argv)
{
    int ret;
    int result;
    int fd, fdd;
    int value;
    char buffer[3];
    struct testcase_info *tc_info = (struct testcase_info*)argv;
    ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"%s:[%s]\n",PCBA_MOTO,"自动模式");

    if(tc_info->y <= 0)
		tc_info->y  = get_cur_print_y();

    fdd = open(SYS_GPIO_PATH, O_WRONLY);
    if (fdd < 0)
        printf("open %s failed\n", SYS_GPIO_PATH);
    else
    ret = write(fdd, MOTO_GPIO_NUM, sizeof(MOTO_GPIO_NUM));

    for(;;)
    {
        fd = open(MOTO_GPIO_PATH, O_RDWR);
        if (fd < 0){
            printf("open %s failed\n", MOTO_GPIO_PATH);
            ui_print_xy_rgba(0,tc_info->y,255,0,0,255,"%s:[%s]\n",PCBA_MOTO,"启动失败");
            tc_info->result = -1;
            return argv;
        }

        value = write(fd, HIGH_VALUE, sizeof(HIGH_VALUE));
        sleep(2);
        value = write(fd, LOW_VALUE, sizeof(LOW_VALUE));
        sleep(2);
        close(fd);
    }
    return argv;
}



