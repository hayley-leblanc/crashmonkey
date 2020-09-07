#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "../../../../nova-tester/ioctl.h"
#include "novaCheckpoint.h"

int novaCheckpoint() {
    int val = 0;
    int fd;

    fd = open("/dev/ioctl_dummy", 0);
    if (fd < 0) {
        perror("open");
        return fd;
    }

    val = ioctl(fd, LOGGER_CHECKPOINT, NULL);
    if (val < 0) {
        perror("LOGGER_CHECKPOINT");
        close(fd);
        return val;
    }

    return 0;

    return 0;
}