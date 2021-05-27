#include "dirtest.h"
#include <dirent.h>
#include <string.h>
//#include <stdio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "cmsis_os.h"
#include <stdlib.h>

// size_t file_test(char* path) {
//     //FILE *file;
//     int fd;
//     ssize_t count = 0;
//     ssize_t bytes_read;
//     char buf[512];

//     /*file = fopen(path, "r");

//     if (!file) {
//         return 0;
//     }*/

//     fd = open(path, O_RDONLY);

//     if (fd < 0) {
//         return 0;
//     }

//     /*do {
//         //bytes_read = fread(buf, 1, sizeof(buf), file);
//         bytes_read = read(fd, buf, sizeof(buf));
//         if (bytes_read > 0) {
//             count += bytes_read;
//         }
//     } while (bytes_read > 0);*/

//     //fclose(file);
//     close(fd);

//     return count;
// }

void dummy() {
    DIR *dir;
    dir = opendir("/");
    if (dir == NULL) {
        return;
    }
    closedir(dir);
}
void dir_test() {
    DIR *dir;
    struct dirent *entry;

    /*FILE *file = fopen(media_print_SFN_path, "r");
    if (file != NULL) {
        char buffer[512];
        size_t s_read = fread(buffer, sizeof(buffer[0]), sizeof(buffer) / sizeof(buffer[0]), file);
        if (s_read > 0) {
            SERIAL_ECHOLN("OK");
        }
        fclose(file);
    }*/

    /*osDelay(1000);
    entry = malloc(1024);
    osDelay(1000);
    memset(entry, 0, 1024);
    osDelay(1000);
    free(entry);
    osDelay(1000);*/
    dir = opendir("/");
    if (dir == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) {
            //entry += file_test(entry->d_name);
            break;
        }
    }
    closedir(dir);

    /*int fd = open(media_print_SFN_path, O_RDONLY);
    if (fd >= 0) {
        char buffer[512];
        memset(buffer, 0, sizeof(buffer));
        int s_read = read(fd, buffer, sizeof(buffer));
        if (s_read > 0) {
            SERIAL_ECHOLN("OK");
        }
        close(fd);
    }*/
}
