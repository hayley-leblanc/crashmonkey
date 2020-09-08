#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cassert>

#include "../../../../nova-tester/ioctl.h"
#include "nova.h"

// this stuff is copied directly from crashmonkey/code/user_tools/src/workload.cpp 
// Super ugly defines to do compile-time string concatonation X times...
#define REP0(x)
#define REP1(x)     x
#define REP2(x)     REP1(x) x
#define REP3(x)     REP2(x) x
#define REP4(x)     REP3(x) x
#define REP5(x)     REP4(x) x
#define REP6(x)     REP5(x) x
#define REP7(x)     REP6(x) x
#define REP8(x)     REP7(x) x
#define REP9(x)     REP8(x) x
#define REP10(x)    REP9(x) x

#define REP(hundreds, tens, ones, x) \
  REP##hundreds(REP10(REP10(x))) \
  REP##tens(REP10(x)) \
  REP##ones(x)

static const unsigned int kTestDataSize = 4096;
// 4K of data plus one terminating byte.
static constexpr char kTestDataBlock[kTestDataSize + 1] =
  REP(1, 2, 8, "abcdefghijklmnopqrstuvwxyz123456");

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
}

// this is copied verbatim from CrashMonkey user_tools workload.cpp, we just can't use 
// it where it was originally defined because of all the object stuff
int novaWriteData(int fd, unsigned int offset, unsigned int size) {
    // Offset into a data block to start working at.
    const unsigned int rounded_offset =
    (offset + (kTestDataSize - 1)) & (~(kTestDataSize - 1));
    // Round down size to 4k for number of full pages to write.

    const unsigned int aligned_size = (size >= kTestDataSize) ?
    (size - (rounded_offset - offset)) & ~(kTestDataSize - 1) :
    0;
    unsigned int num_written = 0;

    // The start of the write range is not aligned with our data blocks.
    // Therefore, we should write out part of a data block for this segment,
    // with the first character in the data block aligning with the data block
    // boundary.
    if (rounded_offset != offset) {
    // We should never write more than kTestDataSize of unaligned data at the
    // start.
    const unsigned int to_write = (size < rounded_offset - offset) ?
        size : rounded_offset - offset;
    while (num_written < to_write){
        const unsigned int mod_offset =
        (num_written + offset) & (kTestDataSize - 1);
        assert(mod_offset < kTestDataSize);

        int res = pwrite(fd, kTestDataBlock + mod_offset, to_write - num_written,
            offset + num_written);
        if (res < 0) {
        return res;
        }
        num_written += res;
    }
    }

    // Write out the required number of full pages for this request. The first
    // byte will be aligned with kTestDataSize.
    unsigned int aligned_written = 0;
    while (aligned_written < aligned_size) {
    const unsigned int mod_offset = (aligned_written & (kTestDataSize - 1));
    // Write up to a full page of data at a time.
    int res = pwrite(fd, kTestDataBlock + mod_offset,
        kTestDataSize - mod_offset, offset + num_written);
    if (res < 0) {
        return res;
    }
    num_written += res;
    aligned_written += res;
    } 

    if (num_written == size) {
    return 0;
    }

    // Write out the last partial page of data. The first byte will be aligned
    // with kTestDataSize.
    unsigned int end_written = 0;
    while (num_written < size) {
    assert(end_written < kTestDataSize);
    const unsigned int mod_offset = (end_written & (kTestDataSize - 1));
    int res = pwrite(fd, kTestDataBlock + mod_offset,
        size - num_written, offset + num_written);
    if (res < 0) {
        return res;
    }
    num_written += res;
    end_written += res;
    } 

    return 0;
}