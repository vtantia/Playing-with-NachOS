// stats.h 
//	Routines for managing statistics about Nachos performance.
//
// DO NOT CHANGE -- these stats are maintained by the machine emulation.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "utility.h"
#include "stats.h"

//----------------------------------------------------------------------
// Statistics::Statistics
// 	Initialize performance metrics to zero, at system startup.
//----------------------------------------------------------------------

Statistics::Statistics()
{
    totalTicks = idleTicks = systemTicks = userTicks = 0;
    numDiskReads = numDiskWrites = 0;
    numConsoleCharsRead = numConsoleCharsWritten = 0;
    numPageFaults = numPacketsSent = numPacketsRecvd = 0;
    busyTicks = totalRunTime = totalWaitTime = cpuUtilization = avgWaitTime = 0;
    maxBurst = numBursts = maxCompletion = avgCompletion = variance = 0;
    minBurst = minCompletion = 1000000000;
    threadCount = 0;
    for(int i=0; i<MAX_THREAD_COUNT; i++) {
        completionTimes[i] = 0;
    }
}

//----------------------------------------------------------------------
// Statistics::Print
// 	Print performance metrics, when we've finished everything
//	at system shutdown.
//----------------------------------------------------------------------

void
Statistics::Print()
{
    printf("Ticks: total %d, idle %d, system %d, user %d, busy, %d\n", totalTicks, 
	idleTicks, systemTicks, userTicks, busyTicks);
    printf("Disk I/O: reads %d, writes %d\n", numDiskReads, numDiskWrites);
    printf("Console I/O: reads %d, writes %d\n", numConsoleCharsRead, 
	numConsoleCharsWritten);
    printf("Paging: faults %d\n", numPageFaults);
    printf("Network I/O: packets received %d, sent %d\n", numPacketsRecvd, 
	numPacketsSent);

    printf("Total time: running %d, wait %d\n", totalRunTime, totalWaitTime);
    printf("CPU Utilization: %lf\n", cpuUtilization);
    printf("Burst lengths: max %d, min %d, average %lf, count\(non-zero\) %d\n", maxBurst, minBurst, (double)totalRunTime / numBursts, numBursts);
    printf("Average waiting time: %lf\n", (double)totalWaitTime / (threadCount-1));
    printf("Thread completion times: max %d, min %d, average %lf, variance %lf\n", maxCompletion, minCompletion, avgCompletion, variance);
}

void
Statistics::generateFinalStats()
{
    busyTicks = totalTicks - idleTicks;
    cpuUtilization = (double) totalRunTime / totalTicks;

    for(int i=1; i<threadCount; i++) 
    {
        //maxCompletion = std::max(maxCompletion, completionTimes[i]);
        //minCompletion = std::min(minCompletion, completionTimes[i]);
        maxCompletion = (maxCompletion > completionTimes[i]) ? maxCompletion: completionTimes[i]; //std::max(stats->maxBurst, burstLength);
        minCompletion = (minCompletion < completionTimes[i]) ? minCompletion: completionTimes[i]; //std::min(stats->minBurst, burstLength);
        if (completionTimes[i] == 0)
            printf("%d ..............", i);
        avgCompletion += completionTimes[i];
    }
    avgCompletion /= (threadCount-1);

    for(int i=1; i<threadCount; i++)
    {
        variance += (completionTimes[i] - avgCompletion)*(completionTimes[i] - avgCompletion);
    }
    variance /= (threadCount-1);
}
