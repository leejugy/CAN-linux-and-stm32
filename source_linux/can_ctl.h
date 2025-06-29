#ifndef __CAN_CTL__
#define __CAN_CTL__

#include "define.h"
#include <linux/can.h>
#include <linux/can/isotp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <poll.h>

#define CAN0_INTERFACE "can0"
#define CAN_MAX_SIZE 4095
#define INTERFACE_MAX_NAME 128

typedef struct
{
    char can_interface[128];
    uint8_t can_send[CAN_MAX_SIZE];
    uint8_t can_recv[CAN_MAX_SIZE];
    struct pollfd can_poll_fd;
    size_t can_recv_size;
    size_t can_send_size;
    sem_t sem;
}can_ctl_t;

void start_thread_can(char *interface);
int can_send(can_ctl_t *can_ctl);
#endif