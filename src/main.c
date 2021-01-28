#include <unistd.h>

extern void PSP_Main(int argc, char *argv[]);


/******************************************************************************
 **  Function:  main
 **
 **  Purpose:
 **    This is the main entry point.
 **
 **  Arguments:
 **    Command line arguments
 **
 **  Return:
 **    Zero
 **
 **  Notes:
 **
 */
int main(int argc, char *argv[])
{
    PSP_Main(argc, argv);

    /*
     ** Let the main thread sleep
     */
    for (;;)
    {
        /*
         ** Even though this sleep call is for 1 second,
         ** the sigalarm timer for the 1hz will keep waking
         ** it up. Keep that in mind when setting the timer down
         ** to something very small.
         */
        sleep(1);
    }

    return 0;
}
