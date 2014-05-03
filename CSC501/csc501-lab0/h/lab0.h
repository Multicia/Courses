
void test(void);
long zfunction(long);
long zfunction_s(long);

#ifndef NPROC                           /* set the number of processes  */
#define NPROC           50              /*  allowed if not already done */
#endif

extern long ctr1000;
int activated;
struct data
{
	int freq;
	int time;
};

struct data Info[NPROC][27];

#define FREEMEM 0
#define CHPRIO 1
#define GETPID 2
#define GETPRIO 3
#define GETTIME 4
#define KILL 5
#define RECEIVE 6
#define RECVCLR 7
#define RECVTIM 8
#define RESUME 9
#define SCOUNT 10
#define SDELETE 11
#define SEND 12
#define SETDEV 13
#define SETNOK 14
#define SCREATE 15
#define SIGNAL 16
#define SIGNALN 17
#define SLEEP 18
#define SLEEP10 19
#define SLEEP100 20
#define SLEEP1000 21
#define SRESET 22
#define STACKTRACE 23
#define SUSPEND 24
#define UNSLEEP 25
#define WAIT 26
