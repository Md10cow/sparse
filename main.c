#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

#define DEFAULT_BUFFER_SIZE 4096

static bool is_zeroed(const char *buf, size_t size) {
    for (size_t i = 0; i < size; i++)
        if (buf[i] != '\0') return false;
    return true;
}

int main(int argc, char *argv[]) {
    size_t buf_size = DEFAULT_BUFFER_SIZE;
    int opt;
    while ((opt = getopt(argc, argv, "b:")) != -1)
        if (opt == 'b') buf_size = atoi(optarg);

    int src_fd = (optind < argc - 1) ? open(argv[optind], O_RDONLY) : STDIN_FILENO;
    int dst_fd = (optind < argc - 1) ? open(argv[optind+1], O_CREAT|O_WRONLY|O_TRUNC, 0666) 
                                   : open(argv[optind], O_CREAT|O_WRONLY|O_TRUNC, 0666);

    char buf[buf_size];
    off_t current_pos = 0;
    bool in_zero_sequence = false;

    while (1) {
        ssize_t bytes = read(src_fd, buf, buf_size);
        if (bytes <= 0) break;

        if (is_zeroed(buf, bytes)) {
            if (!in_zero_sequence) {
                lseek(dst_fd, current_pos, SEEK_SET);
                in_zero_sequence = true;
            }
        } else {
            if (in_zero_sequence) {
                in_zero_sequence = false;
            }
            lseek(dst_fd, current_pos, SEEK_SET);
            write(dst_fd, buf, bytes);
        }
        current_pos += bytes;
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}
