#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <sem.h>
#include <stdio.h>

#define DEFAULT_LOCK_PRIO 20


int hproc_sem (char*msg, int sem)
{
     int i;
     wait(sem);
     for (i = 0; i < 100000; i++);
     sleep(1);
     signal(sem);
     kprintf("%s: process ending\n", msg);
     return 0;
}
int hproc_lock (char*msg, int sem, int uselck)
{
     int i;
     lock(sem, WRITE, DEFAULT_LOCK_PRIO);
     for (i = 0; i < 100000; i++);
     sleep(1);
     releaseall(1, sem);
     kprintf("%s: process ending\n", msg);
     return 0;
}
int lproc_sem (char*msg, int sem)
{
     int i;
     int start;
     wait(sem);
     sleep(1);
     for (i = 0; i < 100000; i++);
     sleep(1);
     signal(sem);
     kprintf("%s: process ending\n", msg);
     return 0;
}
int lproc_lock (char*msg, int sem)
{
     int i;
     int start;
     lock(sem, READ, DEFAULT_LOCK_PRIO);
     sleep(1);
     for (i = 0; i < 100000; i++);
     sleep(1);
     releaseall(1, sem);
     kprintf("%s: process ending\n", msg);
     return 0;
}
int mproc(char *msg, int sem)
{
     int i;
     int start;
     start = ctr1000;
     while (ctr1000 - start < 5000) 
     {
	  for (i = 0; i < 999999; i++);
	       kprintf("working");
     }
     kprintf("%s: process ending\n", msg);
     return 0;
}


void semph_test ()
{
     int sem, hi, med, low;
     sem = screate(1);
     if (sem == SYSERR)
	  kprintf("Error: screate() failed\n");
     low = create(lproc_sem, 2000, 10, "low", 2, "low", sem);
     med = create(mproc, 2000, 20, "med", 2, "med", sem);
     hi = create(hproc_sem, 2000, 30, "high", 2, "high", sem);



     kprintf("-- start low. sem granted (prio 30)\n");
     resume(low);
     sleep(1);

     kprintf("-- start hi sem not granted (prio 50)\n");
     resume(hi);
     kprintf("-- start med runs before hi (prio 40)\n");
     resume(med);
     
}

void lock_test()
{
     int lck,hi, med, low;
     lck = lcreate();
     if (lck == SYSERR)
	  kprintf("Error: lcreate() failed\n");
     low = create(lproc_lock, 2000, 10, "proclow", 2, "proclow", lck);
     med = create(mproc, 2000, 20, "procmed", 2, "procmed", lck);
     hi = create(hproc_lock, 2000, 30, "prochigh", 2, "prochigh", lck);
     
    

     kprintf("-- start low priority proclow. lock granted (prio 30)\n");
     resume(low);
     sleep(1);
     kprintf("-- start hi priority proclow. lock granted after proclow finishes (prio 50)\n");
     resume(hi);
     kprintf("-- start med priority procmed. runs after prochi finishes (prio 40)\n");
     resume(med);
}
