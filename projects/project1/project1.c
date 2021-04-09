#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_PROCESSES = 32;
const int MAX_LINE      = 128;

// Global data structures are used to store the calculated time
struct SProcessTime { int process, time; };


// function that updates the turnaround time and the global TurnaroundTime arrays
void calculateTurnAroundTime( int processes[], int n, int burst_time[], int wait_time[], int turnaround_time[], struct SProcessTime *TurnaroundTime)
{
    // calculate tat by adding burst time and wait time
    for (int i = 0; i < n ; i++)
    {
        turnaround_time[i] = burst_time[i] + wait_time[i];
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
				TurnaroundTime[j].time = turnaround_time[i];
				break;
			}
		}
		if( TurnaroundTime[j].process == 0)
		{
			TurnaroundTime[j].process = processes[i];
			TurnaroundTime[j].time = turnaround_time[i];
		}
	}
}

// function to update wt and the global WaitingTime arrays
void calculateWaitingTime(int processes[], int n, int burst_time[], int wait_time[], struct SProcessTime *WaitingTime)
{
    // calculating waitingtime for all processes, even ones that are mentioned multiple times
    wait_time[0] = 0;
    for (int i = 1; i < n ; i++)
    {
        wait_time[i] =  burst_time[i-1] + wait_time[i-1];
    }

	// update the global struct which saves the per-process waiting time
	// logic is that if the entry is already there, then it is not updated
	for( int i = 0; i < n; i++)
	{
		int j = 0;
		for(; WaitingTime[j].process != 0; j++)
		{
			if( WaitingTime[j].process == processes[i] )
				break;
		}
		if( WaitingTime[j].process == 0)
		{
			WaitingTime[j].process = processes[i];
			WaitingTime[j].time = wait_time[i];
		}
	}
}

// function to calculate times according to FCFS
void calculateFCFS( int n, int *processes, int *burst_time)
{
    int wait_time[n], turnaround_time[n], total_wt = 0, total_tat = 0;
    float sum = 0;

	struct SProcessTime WaitingTime[MAX_PROCESSES];
	struct SProcessTime TurnaroundTime[MAX_PROCESSES];


	memset( WaitingTime, 0, sizeof ( WaitingTime));
	memset( TurnaroundTime, 0, sizeof( TurnaroundTime));

    calculateWaitingTime(processes, n, burst_time, wait_time, WaitingTime);
    calculateTurnAroundTime(processes, n, burst_time, wait_time, turnaround_time, TurnaroundTime);


	int total_processes = 0;
	for( int j = 0; WaitingTime[j].process != 0; j++)
	{
        total_wt = total_wt + WaitingTime[j].time;
        total_tat = total_tat + TurnaroundTime[j].time;
		total_processes++;
	}
 	for(int i = 0; i < total_processes; i++){
	sum = sum + *burst_time;

	}
   // printf("Average waiting time = %.2f\n", (float)total_wt / (float)total_processes);
   // printf("Average turn around time = %.2f\n", (float)total_tat / (float)total_processes);
   printf("%.2f\n", (float)total_processes);
   printf("%.2f\n", (float)total_processes / (float)total_wt);
   printf("%.2f\n", (float)total_tat / (float)total_processes);
   printf("%.2f\n", (float)total_wt / (float)total_processes);
   printf("%.2f\n", (float)total_wt / (float)total_processes);
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

	for ( int i = 0; i < N; i++)
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
    int n= 0;
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
