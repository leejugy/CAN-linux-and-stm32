#include "define.h"
#include "can_ctl.h"

int main()
{
    start_thread_can(CAN0_INTERFACE);
    while(1)
    {
        sleep(10);
    }
}