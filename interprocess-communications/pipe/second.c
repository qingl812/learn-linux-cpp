/***
 * @brief 命名管道 mkfifo
 * @include sys/stat.h
 * @ref int mkfifo(const char* pathname, mode_t mode);
 * @return 成功，0；失败，-1
 * @details
 * https://www.cnblogs.com/xianghang123/archive/2012/03/31/2427731.html
 *
 * @note
 * gcc interprocess-communications/pipe/second.c -o out/a.out && out/a.out
 * echo Q >> /tmp/my_fifo
 * rm /tmp/my_fifo
 */

#include <errno.h>    // errno
#include <fcntl.h>    // O_CREAT
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // read

#define FIFO "/tmp/my_fifo"

// 本程序从一个FIFO读数据，并把读到的数据打印到标准输出
// 如果读到字符“Q”，则退出
int main(int argc, char** argv) {
    char buf_r[100];
    int fd;
    int nread;
    if ((mkfifo(FIFO, O_CREAT) < 0) && (errno != EEXIST)) {
        printf("不能创建FIFO\n");
        exit(1);
    }

    printf("准备读取数据\n");
    fd = open(FIFO, O_RDONLY, 0);
    if (fd == -1) {
        perror("打开FIFO");
        exit(1);
    }

    while (1) {
        if ((nread = read(fd, buf_r, 100)) == -1) {
            if (errno == EAGAIN)
                printf("没有数据\n");
        }

        //假设取到Q的时候退出
        if (buf_r[0] == 'Q')
            break;

        buf_r[nread] = 0;
        printf("从FIFO读取的数据为：%s\n", buf_r);
        sleep(1);
    }
}