/***
 * @brief linux c pipe
 * @include unistd.h
 * @ref int pipe(int pipefd[2]);
 *
 * @details
 * pipe() 创建一个管道，一个可用于进程间通信的单向数据通道。
 * 数组 pipefd 用于返回两个引用管道末端的文件描述符。
 * pipefd[0] 指的是管道的读取端。 pipefd[1] 指的是管道的写端。
 * 写入管道写入端的数据由内核缓冲，直到从管道的读取端读取。
 * 如果 flags 为 0，则 pipe2() 与 pipe() 相同。
 *
 * @return
 * 成功时，返回零。 出错时，返回 -1，并适当设置 errno。
 *
 * @details
 * https://linux.die.net/man/2/pipe
 *
 * @note
 * gcc interprocess-communications/pipe/first.c -o out/a.out && out/a.out 123
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t cpid;
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    char buf;
    if (cpid == 0) {      /* Child reads from pipe */
        close(pipefd[1]); /* Close unused write end */
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    } else {              /* Parent writes argv[1] to pipe */
        close(pipefd[0]); /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]); /* Reader will see EOF */
        wait(NULL);       /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}