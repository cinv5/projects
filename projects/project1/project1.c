
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_PROCESSES = 32;
const int MAX_LINE      = 32769;

// Global data structures are used to store the calculated time
struct SProcessTime { int process, time; };


// function that updates the turnaround time and the global TurnaroundTime arrays
void calculateTurnAroundTime( int processes[], int n, int burst_time[], int rp_time[], int turnaround_time[], struct SProcessTime *TurnaroundTime)
{
    // calculate tat by adding burst time and wait time
    for (int i = 0; i < n ; i++)
    {
        turnaround_time[i] = burst_time[i] + rp_time[i];
    }

	// update the global struct which saves the per-process turnaround time
	// logic is that if the entry is already there, then its updated
	for( int i = 0; i < n; i++)
	{
		int j = 0;
		for(; TurnaroundTime[j].process != 0; j++)
		{
			if( TurnaroundTime[j].process == processes[i] )
			{
			    TurnaroundTime[j].process = processes[i];
			    TurnaroundTime[j].time = TurnaroundTime[j].process;
				TurnaroundTime[j].time = turnaround_time[i];
				//important one
				//printf("turnaround man %d = %d\n", TurnaroundTime[j].process, turnaround_time[i]);
				break;
			}
		}
		if( TurnaroundTime[j].process == 0)
		{
			TurnaroundTime[j].process = processes[i];
			TurnaroundTime[j].time = turnaround_time[i];
			//important one
			//printf("turnaround man c %d = %d\n", TurnaroundTime[i].process, turnaround_time[i]);
		}
	}
}

// function to update wt and the global WaitingTime arrays

void calculateResponseTime(int processes[], int n, int burst_time[], int rp_time[], struct SProcessTime *ResponseTime)
{
    // calculating waitingtime for all processes, even ones that are mentioned multiple times
    rp_time[0] = 0;
    for (int i = 1; i < n ; i++)
    {
        rp_time[i] =  burst_time[i-1] + rp_time[i-1];
    }

	// update the global struct which saves the per-process waiting time
	// logic is that if the entry is already there, then it is not updated
	for( int i = 0; i < n; i++)
	{
		int j = 0;
		for(; ResponseTime[j].process != 0; j++)
		{
			if( ResponseTime[j].process == processes[i] )
			//printf("response man %d = %d\n", ResponseTime[j].process, rp_time[i]);
				break;
		}
		if( ResponseTime[j].process == 0)
		{
			ResponseTime[j].process = processes[i];
			ResponseTime[j].time = rp_time[i];
			//important one
			//printf("response man %d = %d\n", ResponseTime[i].process, rp_time[i]);
		}
	}
}
/*
void calculateWaitTime( int processes[], int n, int burst_time[], int rp_time[], int wait_time[], struct SProcessTime *WaitTime)
{
    // calculate tat by adding burst time and wait time
    for (int i = 0; i < n ; i++)
    {
        wait_time[i] = burst_time[i] + rp_time[i];
    }

	// update the global struct which saves the per-process turnaround time
	// logic is that if the entry is already there, then its updated
	for( int i = 0; i < n; i++)
	{
		int j = 0;
		for(; WaitTime[j].process != 0; j++)
		{
			if( WaitTime[j].process == processes[i] )
			{
			    WaitTime[j].process = processes[i];
			    WaitTime[j].time = WaitTime[j].process;
				WaitTime[j].time = wait_time[i];
				printf("waittime man %d = %d\n", WaitTime[j].process, wait_time[i]);
				break;
			}
		}
		if( WaitTime[j].process == 0)
		{
			WaitTime[j].process = processes[i];
			WaitTime[j].time = wait_time[i];
			//printf("waittime man %d = %d\n", WaitTime[i].process, wait_time[i]);
		}
	}
}
*/


void calculateWaitTime(int processes[], int n, int burst_time[], int wait_time[], struct SProcessTime *WaitTime)
{
    // calculating waitingtime for all processes, even ones that are mentioned multiple times
    wait_time[0] = 0;
    for (int i = 1; i < n ; i++)
    {
        //printf("wait time initial %d vs burst_time initial %d\n vs wait_time initial %d\n", wait_time[i-1], burst_time[i-1] , wait_time[i-1]);
        wait_time[i] =  burst_time[i-1] + wait_time[i-1];

    }

	// update the global struct which saves the per-process waiting time
	// logic is that if the entry is already there, then it is not updated
	for( int i = 0; i < n; i++)
	{
		int j = 0;
		//printf("i is %d\n", i);
		for(; WaitTime[j].process != 0; j++)
		{
			if( WaitTime[j].process == processes[i] ){
			    //printf("wat is %d vs was is %d\n", WaitTime[j].process, processes[i]);

				//WaitTime[j].time = wait_time[i];
				//printf("wait man %d = %d\n", WaitTime[i].process, wait_time[i]);

				//WaitTime[j].process = processes[i];
			    //WaitTime[j].time = WaitTime[j].process;
				//WaitTime[j].time = wait_time[i];
				//printf("wait man %d = %d\n", WaitTime[j].process, wait_time[i]);

			    wait_time[i] = WaitTime[j].time + burst_time[j+1];

				//important one
				//printf("wait man %d = %d\n", WaitTime[j].process, wait_time[i]);

				//printf("location of this %d\n", WaitTime[j].time + burst_time[j+1]);
				//printf("j is %d\n", j+1);
				break;
			}
		}
		if( WaitTime[j].process == 0)
		{

			WaitTime[j].process = processes[i];
			//printf("wait man  %d vs value %d\n", WaitTime[j].process, processes[i]);

			WaitTime[j].time = wait_time[i];

			//wait_time[i] = WaitTime[j].time + burst_time[j+1];

			//printf("wait man time %d vs time value %d\n", WaitTime[j].time, wait_time[i]);
			//important
			//printf("wait man %d = %d\n", WaitTime[i].process, wait_time[i]);
		}
	}
}

// function to calculate times according to FCFS
void calculateFCFS( int n, int *processes, int *burst_time)
{
    int rp_time[n], turnaround_time[n], wait_time[n], total_rt = 0, total_tat = 0, total_wt = 0;
    int sumOf = 0, sum = 0;

	struct SProcessTime ResponseTime[MAX_PROCESSES];
	struct SProcessTime TurnaroundTime[MAX_PROCESSES];
	//struct SProcessTime WaitTime[MAX_PROCESSES];
	struct SProcessTime WaitTime[MAX_PROCESSES];

	memset( ResponseTime, 0, sizeof ( ResponseTime));
	memset( TurnaroundTime, 0, sizeof( TurnaroundTime));
    //memset( WaitTime, 0, sizeof(WaitTime));
    memset( WaitTime, 0, sizeof(WaitTime));
    calculateResponseTime(processes, n, burst_time, rp_time, ResponseTime);
    calculateTurnAroundTime(processes, n, burst_time, rp_time, turnaround_time, TurnaroundTime);
    calculateWaitTime(processes, n, burst_time, wait_time, WaitTime);
    //calculateWaitTime(processes, n, burst_time, rp_time, wait_time, WaitTime);
	int total_processes = 0;
	for(int i = 1; i <= n; i++){
	  sumOf++;
	}
	for( int j = 0; ResponseTime[j].process != 0; j++)
	{
		total_processes++;
	}
	sum = sumOf - total_processes;
	printf("\n");
	for( int j = 0; ResponseTime[j].process != 0; j++)
	{
	if(sum == 1){
	    total_rt = total_rt + ResponseTime[j].time;
        total_tat = total_tat + TurnaroundTime[j].time;
	    total_wt = total_tat - total_rt;
	}
	else{
        total_rt = total_rt + ResponseTime[j].time;
        total_tat = total_tat + TurnaroundTime[j].time;
        total_wt = total_rt + ResponseTime[j].time;
	}
	}
	/*
    printf("Voluntary = %.2f\n", (float)total_processes);
    printf("NonContext = %.2f\n", (float)sum);
    printf("CPU: %.2f\n", 100.00);
    printf("Throughput: %.2f\n", (float)total_processes/(float)total_rt);
    printf("turn around time = %.2f\n", (float)total_tat / (float)total_processes);
    printf("waiting time = %.2f\n", (float)total_wt/(float)total_processes);
    printf("response time = %.2f\n", (float)total_rt / (float)total_processes);
    */
    printf("%d\n", total_processes);
    printf("%d\n", sum);
    printf("%.2f\n", 100.00);
    printf("%.2f\n", (double)total_processes/(double)total_rt);
    printf("%.2f\n", (double)total_tat / (double)total_processes);
    printf("%.2f\n", (double)total_wt/(double)total_processes);
    printf("%.2f\n", (double)total_rt / (double)total_processes);
}

void readFile( FILE *fp, int *n, int **processes, int **burst_time)
{
	//cout << fp << endl;
	char *line;

	size_t  len = MAX_LINE;
	line = (char *) malloc( MAX_LINE);
	int ret = getline( &line, &len, fp);
	if( ret == -1)
	{
		return;
	}

	ret = getline( &line, &len, fp);
	if( ret == -1)
	{
		return;
	}

	int p, N;
	sscanf(line, "%d %d", &p, &N);
	*n = N;

	*processes = (int *) malloc( sizeof( int ) * N);
	*burst_time = (int *) malloc( sizeof( int ) * N);

	for ( int i = 0; i <= N; i++)
	{
		ret = getline( &line, &len, fp);
		if( ret == -1)
		{
			return;
		}
		int a, b, c;
		sscanf(line, "%d %d %d", &a, &b, &c);
		(*processes)[i] = a;
		(*burst_time)[i] = b;
	}
	fclose(fp);

}

int main( int argc, char *argv[])
{
    int *processes;
    int n = 0;

    int  *burst_time;
	FILE *fp;
	if( argc == 1)
	{
		fp = stdin;
	}
	else
	{
		fp = fopen(argv[1], "r");
	}

	readFile( fp, &n, &processes, &burst_time);

    calculateFCFS(n, processes, burst_time);
    return 0;
}

