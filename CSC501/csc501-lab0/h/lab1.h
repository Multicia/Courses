

#ifndef NPROC                           /* set the number of processes  */
#define NPROC           50              /*  allowed if not already done */
#endif
#define RANDOMSCHED 1
#define LINUXSCHED 2
int used_scheduler;
unsigned long execution_time[NPROC];
