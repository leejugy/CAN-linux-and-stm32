#include "can_ctl.h"

static void can_init(can_ctl_t *can_ctl)
{
    char linux_cmd[LINUX_CMD_LEN] = {0, };
    struct ifreq ifr = {0, };
    struct sockaddr_can can_addr = {0, };

    sprintf(linux_cmd, "ip link set %s down", can_ctl->can_interface);
    if(system(linux_cmd) < 0)
    {
        printr("fail down %s : %s", can_ctl->can_interface, strerror(errno));
    }

    sprintf(linux_cmd, "ip link set %s up type can bitrate 1000000 sample-point 0.75", can_ctl->can_interface);
    if(system(linux_cmd) < 0)
    {
        printr("fail up %s : %s", can_ctl->can_interface, strerror(errno));
        exit(0);
    }
    
    can_ctl->can_poll_fd.fd = socket(AF_CAN, SOCK_DGRAM, CAN_ISOTP);

    if(can_ctl->can_poll_fd.fd < 0)
    {
        printr("fail to open socket : %s", strerror(errno));
        exit(-1);
    }

    struct can_isotp_fc_options fcopts = 
    {
        .bs = 8,         
        .stmin = 0,     
        .wftmax = 1
    };
    if(setsockopt(can_ctl->can_poll_fd.fd, SOL_CAN_ISOTP, CAN_ISOTP_RECV_FC, &fcopts, sizeof(fcopts)) < 0)
    {
        printr("fail to set %s socket option", can_ctl->can_interface);
        exit(-2);
    }

    strcpy(ifr.ifr_ifrn.ifrn_name, can_ctl->can_interface);
    if(ioctl(can_ctl->can_poll_fd.fd, SIOCGIFINDEX, &ifr) < 0)
    {
        printr("fail to read interface can : %s", strerror(errno));
        exit(-3);
    }

    can_addr.can_family = AF_CAN;
    can_addr.can_addr.tp.rx_id = 0x03;
    can_addr.can_addr.tp.tx_id = 0x02;
    can_addr.can_ifindex = ifr.ifr_ifru.ifru_ivalue;
    
    if(bind(can_ctl->can_poll_fd.fd, (struct sockaddr *) &can_addr, sizeof(can_addr)) < 0)
    {
        printr("fail to bind can %d : %s", can_addr.can_ifindex, strerror(errno));
        exit(-4);
    }

    if(sem_init(&can_ctl->sem, 0, 1) < 0)
    {
        printr("fail to init semaphore");
        exit(-5);
    }

    can_ctl->can_poll_fd.events = POLLERR | POLLIN | POLLOUT;
}

int can_send(can_ctl_t *can_ctl)
{
    int ret = 0;
    ret = poll(&can_ctl->can_poll_fd, sizeof_pollfd(can_ctl->can_poll_fd.fd), 10);
    if(ret < 0)
    {
        printr("pollfd return -1 : %s", strerror(errno));
        return -1;
    }

    if(can_ctl->can_poll_fd.revents & POLLOUT)
    {
        sem_wait(&can_ctl->sem);
        ret = write(can_ctl->can_poll_fd.fd, can_ctl->can_send, can_ctl->can_send_size);        
        if(ret < 0)
        {   
            printr("fail to write %s", can_ctl->can_interface);
        }
        sem_post(&can_ctl->sem);
        return ret;
    }
    else if(can_ctl->can_poll_fd.revents & POLLERR)
    {
        printr("pollfd return POLLERR");
        return -1;
    }
    else if(can_ctl->can_poll_fd.revents & POLLIN)
    {
        return -1;
    }
    else
    {
        printr("send unkown poll status : %d", can_ctl->can_poll_fd.revents);
        return -1;
    }
}

static int can_recv(can_ctl_t *can_ctl)
{
    int ret = 0;
    ret = poll(&can_ctl->can_poll_fd, sizeof_pollfd(can_ctl->can_poll_fd.fd), 10);
    if(ret < 0)
    {
        printr("pollfd return -1 : %s", strerror(errno));
        return -1;
    }

    if(can_ctl->can_poll_fd.revents & POLLIN)
    {
        sem_wait(&can_ctl->sem);
        ret = read(can_ctl->can_poll_fd.fd, can_ctl->can_recv, can_ctl->can_recv_size);        
        if(ret < 0)
        {   
            printr("fail to read %s", can_ctl->can_interface);
        }
        sem_post(&can_ctl->sem);
        return ret;
    }
    else if(can_ctl->can_poll_fd.revents & POLLERR)
    {
        printr("pollfd return POLLERR");
        return -1;
    }
    else if(can_ctl->can_poll_fd.revents & POLLOUT)
    {
        return -1;
    }
    else
    {
        printr("recv unkown poll status : %d", can_ctl->can_poll_fd.revents);
        return -1;
    }
}

static void thread_can_recv(void *arg)
{
    can_ctl_t *can_ctl = arg;
    int ret = 0;
    while(1)
    {
        memset(can_ctl->can_recv, 0, sizeof(can_ctl->can_recv));
        ret = can_recv(can_ctl);
        if(ret > 0)
        {
            printb("recv : %s", can_ctl->can_recv);
        }

        usleep(100 * 1000);
    }
}

static void thread_can_send(void *arg)
{
    can_ctl_t *can_ctl = arg;
    int try = 0;
    while(1)
    {
        memset(can_ctl->can_send, 0, sizeof(can_ctl->can_send));
        sprintf(can_ctl->can_send, "can send by linux try : %d", try++);
        can_ctl->can_send_size = strlen(can_ctl->can_send);
        try %= 100;
        can_send(can_ctl);

        usleep(1000 * 1000);
    }
}

can_ctl_t can1_ctl = {0, };

void start_thread_can(char *interface)
{
    pthread_t tid = 0;

    strcpy(can1_ctl.can_interface, interface);
    can1_ctl.can_recv_size = CAN_MAX_SIZE;
    can_init(&can1_ctl);

    if(pthread_create(&tid, NULL, (void *)&thread_can_send, &can1_ctl) < 0)
    {
        printr("fail to create thread send : %s", strerror(errno));
        exit(1);
    }

    if(pthread_detach(tid) < 0)
    {
        printr("fail to detach thread send : %s", strerror(errno));
        exit(1);
    }

    if(pthread_create(&tid, NULL, (void *)&thread_can_recv, &can1_ctl) < 0)
    {
        printr("fail to create thread recv : %s", strerror(errno));
        exit(1);
    }

    if(pthread_detach(tid) < 0)
    {
        printr("fail to detach thread recv : %s", strerror(errno));
        exit(1);
    }
}