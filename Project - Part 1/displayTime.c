#include "commands.h"

void displayTime(char* arguments)
{
    time_t rawTime;
    struct tm* timeInfo;

    time(&rawTime);
    timeInfo = localtime (&rawTime);

    if(strcmp(arguments,"help") == 0)
    {
        blue();
        printf("display-time ");
        reset();
        printf("gives the time in 24hr format by default.\nUse the argument '12' to display in 12hr format.\n");
    }
    else if(strcmp(arguments,"12") == 0 && timeInfo->tm_hour > 12)
    {
        timeInfo->tm_hour = timeInfo->tm_hour - 12;
        printf("The current time is: %d:%d:%d\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    }
    else
    {
        printf("The current time is: %d:%d:%d\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    }

}