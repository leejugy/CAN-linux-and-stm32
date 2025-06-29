#ifndef __DEFINE__
#define __DEFINE__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define LINUX_CMD_LEN 1024
#define sizeof_pollfd(fd) sizeof(fd)/sizeof(int)
#define printr(fmt, ...) printf("[\x1b[31mERR\x1b[0m]" fmt "\n", ##__VA_ARGS__)
#define printg(fmt, ...) printf("[\x1b[32mSUCCESS\x1b[0m]" fmt "\n", ##__VA_ARGS__)
#define printy(fmt, ...) printf("[\x1b[33mWARNING\x1b[0m]" fmt "\n", ##__VA_ARGS__)
#define printb(fmt, ...) printf("[\x1b[34mDEBUG\x1b[0m]" fmt "\n", ##__VA_ARGS__)

#endif