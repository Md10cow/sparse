#include <unistd.h>
#include <fcntl.h>

#define FILE_SIZE (4 * 1024 * 1024)

int main(void) {
    int fd = open("A", O_TRUNC | O_CREAT | O_WRONLY, 0666);
    write(fd, "1", 1);
    char zero = 0;
    for (int i = 0; i < 9999; i++)
        write(fd, &zero, 1);
    write(fd, "1", 1);
    for (int i = 10001; i < FILE_SIZE; i++)
        write(fd, &zero, 1);
    write(fd, "1", 1);
    close(fd);
    return 0;
}
