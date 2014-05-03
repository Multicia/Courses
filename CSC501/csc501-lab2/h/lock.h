#ifndef NLOCKS
#define NLOCKS            50      /* number of locks, if not defined */
#endif

#ifndef NPROC
#define NPROC            50      /* number of locks, if not defined */
#endif


#define LFREE '\09'
#define LUSED '\08'
#define DELETED '\01'
#define READ '\02'
#define WRITE '\03'

extern long ctr1000;

struct  lentry  {               /* semaphore table entry                */
        char    lstate;         /* the state SFREE or SUSED             */
        int     lockcnt;         /* count for this semaphore             */
	int 	read_count;
        int	maxprio;
	int 	process_lock[NPROC];
	int	mod_number;
	int     lqhead;         /* q index of head of list              */
        int     lqtail;         /* q index of tail of list              */
};

extern  struct  lentry  lockarr[];
extern  int     nextlock;
void release_all(int ,long , ...);
#define isbadlock(s)     (s<0 || s>=NLOCKS)

