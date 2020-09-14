#include "include.h"
#include "fsl_ftm.h"
int main(void)
{
    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    NVIC_SetPriorityGrouping(2);

    system_run();
    
    return 0;
}
