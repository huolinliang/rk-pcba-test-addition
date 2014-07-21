/*
 * author:leung
 * light_sensor test in pcba mode
 */

#define LOG_TAG "light_sensor"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <pthread.h>
#include "language.h"
#include "test_case.h"
#include "light_sensor_test.h"

#define LS_PATH   "/sys/class/input"
#define LS_NAME   "lightsensor-level"


void* light_sensor_test(void *argv)
{
    int ret;
    int result;
    int fd;
    int count;
    int i;
    char buffer[20];
    DIR *dir;
    char path[PATH_MAX];
    struct dirent *ptr;
    struct testcase_info *tc_info = (struct testcase_info*)argv;

    // get print line
    if(tc_info->y <= 0)
        tc_info->y  = get_cur_print_y();

    dir = opendir(LS_PATH);
    while((ptr = readdir(dir)) != NULL) {
        // we ignore "." and ".."
        const char *subdir = ptr->d_name;
        if(subdir[0] == '.' && (subdir[1] == 0 || (subdir[1] == '.' && subdir[2] == 0))) {
            continue;
        }

        // find name file in each subdir
        snprintf(path, sizeof(path), "%s/%s/device/name", LS_PATH, subdir);
        printf("path=%s\n", path);
        fd = open(path, O_RDONLY);
        if (fd < 0){
            printf("open %s failed\n", path);
            continue;
        }

        count = read(fd, buffer, sizeof(buffer));
        if(count > 0) {
            while(count > 0 && buffer[count-1] == '\n')
                count--;
            buffer[count] = '\0';
        }
        else {
            buffer[0] = '\0';
        }

        if(strcmp(buffer,LS_NAME) == 0) {
            printf("pcba light_sensor success\n");
            ui_print_xy_rgba(0,tc_info->y,0,255,0,255,"%s:[%s]\n",PCBA_LIGHT_SENSOR,"通过");
            break;
        }
        else {
            printf("read lightsensor_name=%s", buffer);
            ui_print_xy_rgba(0,tc_info->y,255,255,0,255,"%s:[%s]\n",PCBA_LIGHT_SENSOR,"检测中..");
        }
        sleep(2);
        close(fd);
    }
    return argv;
}



