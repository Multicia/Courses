#include <lab0.h>
#include <stdio.h>
extern int activated;


void printsyscallsummary()
{
	int i,j,count;
	int time_taken;
	char *names[27];
	names[0]="freemem";
	names[1]="chprio";
	names[2]="getpid";
	names[3]="getprio";
	names[4]="gettime";
	names[5]="kill";
	names[6]="receive";
	names[7]="recvclr";
	names[8]="recvtim";
	names[9]="resume";
	names[10]="scount";
	names[11]="sdelete";
	names[12]="send";
	names[13]="setdev";
	names[14]="setnok";
	names[15]="screate";
	names[16]="signal";
	names[17]="signaln";
	names[18]="sleep";
	names[19]="sleep10";
	names[20]="sleep100";
	names[21]="sleep1000";
	names[22]="sreset";
	names[23]="stacktrace";
	names[24]="suspend";
	names[25]="unsleep";
	names[26]="wait";
		
	if(activated == 3)
	{
		for(i=0;i<NPROC;i++)
		{
			count=0;
			for(j=0;j<27;j++)
				if(Info[i][j].freq != 0)
					count++;
			if(count!=0)
				kprintf("\nProcess [pid:%d]\n",i);
			for(j=0;j<27;j++)
			{
				if(Info[i][j].freq != 0)
				{
					if(Info[i][j].time == 0)
						time_taken=0;
					else
						time_taken = (Info[i][j].time/Info[i][j].freq);
					kprintf("\t Syscall: %s, count: %d, average execution time: %d (ms)\n",names[j],Info[i][j].freq,time_taken); 
				}
			}
		}
	}
	if(activated == 1)
	{
			kprintf("\n syscallsummary_stop not called");
	}
	if(activated == -1)
	{
		 kprintf("\n syscallsummary_start not called");
	}
}
