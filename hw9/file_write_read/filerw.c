#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buf[1];
    int fd = open("output.txt", O_CREAT | O_WRONLY);
    write(fd, "String for file", 16);
    close(fd);
    fd = open("output.txt", O_RDONLY);
    for (int i = -1; i >= -16; i--)
    {
        lseek(fd, i, SEEK_END);
        read(fd, buf, 1);
        printf("%s", buf);
    }
    close(fd);
    printf("\n");
}