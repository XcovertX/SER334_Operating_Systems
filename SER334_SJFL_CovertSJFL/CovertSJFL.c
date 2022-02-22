/**
 * Simulates Shortest-Job-First scheduling algorithms
 *
 * Completion time 8hrs
 * @author James Covert
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// globals
int process_count;
int tick_count;
int proc_id;
int *t;
int *a;
int live_t;
float live_a;
int *cp;
int *pn;
int p_num;
int p_time;
int tick_time;
int wait_time;
int tot_time;
int turn_time;
int small;
int small_id;
int error_count;
int live_errors;

// functions
void read_file(FILE* file){
    fscanf(file, "%d", &tick_count);
    fscanf(file, "%d", &process_count);
    printf("Tick Count: %d ::: Process Count: %d\n", tick_count, process_count);
}

/**
 * Simulates smallest job first
 */
void simulate_SJF(int *proc[]){
    printf("== Shortest Job First ==\n");

    tot_time = 0;
    turn_time = 0;
    wait_time = 0;

    for(int i = 0; i < tick_count; i++) {
        tick_time = 0;
        printf("Simulating %dth round of processes @ time %d: \n", i, tot_time);

        for (int j = 0; j < process_count; j++) {
            small = 999;
            small_id = 0;
            for (int k = 0; k < process_count; k++) {
                if (small > proc[k][i]) {
                    small = proc[k][i];
                    small_id = k;
                }
            }
            cp[j] = small;
            pn[j] = small_id;
            proc[small_id][i] = 999;
        }

        for (int i = 0; i < process_count; i++) {
            printf("  Process %d took %d.\n", pn[i], cp[i]);
            turn_time = turn_time + tick_time + cp[i];
            tot_time = tot_time + cp[i];
            wait_time = wait_time + tick_time;
            tick_time = tick_time + cp[i];
        }
    }
}

/**
 * Simulates smallest job first live
 */
void simulate_SJFL(int *t_time[], int *proc_2[]){
    printf("== Shortest-Job-First Live ==\n");

    tot_time = 0;
    turn_time = 0;
    wait_time = 0;
    error_count = 0;
    live_errors = 0;

    for (int i = 0; i < tick_count; i++) {
        tick_time = 0;
        printf("Simulating %dth round of processes @ time %d: \n", i, tot_time);

        for(int j = 0; j < process_count; j++) {
            small = 999;
            small_id = 0;
            for(int k = 0; k < process_count; k++) {
                if(small > t_time[k][i]) {
                    small = t_time[k][i];
                    small_id = k;
                }
            }
            pn[j] = small_id;
            cp[j] = small;
            t_time[small_id][i] = 999;
        }

        for (int j = 0; j < process_count; j++) {
            printf("  Process %d was estimated for %d but took %d.\n", pn[j], cp[j], proc_2[pn[j]][i]);

            if(proc_2[pn[j]][i] > cp[j]) {
                live_errors = proc_2[pn[j]][i] - cp[j];
            } else {
                live_errors = cp[j] - proc_2[pn[j]][i];
            }

            turn_time = turn_time + tick_time + proc_2[pn[j]][i];
            tot_time = tot_time + proc_2[pn[j]][i];
            wait_time = wait_time + tick_time;
            tick_time = tick_time + proc_2[pn[j]][i];
            error_count = error_count + live_errors;
        }
    }
}
/**
 * Frees all memory
 */
void free_all(int *proc_1[], int *proc_2[], int *t_time[]){
    for(int i = 0; i < process_count; i++) {
        free(proc_1[i]);
        free(proc_2[i]);
        free(t_time[i]);
    }
    proc_1 = NULL;
    proc_2 = NULL;
    t_time = NULL;
    free(t);
    free(a);
    free(cp);
    free(pn);
}

int main(int argc, char *argv[]) {

    FILE *f;
    char fName[255];
    strcpy(fName, argv[1]);
    f = fopen(fName, "r");
    read_file(f);

    t = (int *)malloc(process_count * sizeof(int));
    a = (int *)malloc(process_count * sizeof(int));
    int *proc_1[process_count];
    int *proc_2[process_count];
    int *t_time[process_count];

    for(int i = 0; i < process_count; i++) {
        proc_1[i] = (int *)malloc(tick_count * sizeof(int));
    }

    cp = (int *)malloc(process_count * sizeof(int));
    pn = (int *)malloc(process_count * sizeof(int));
    p_num = 0;

    for(int i = 0; i < process_count; i++) {
        t_time[i] = (int *)malloc(tick_count * sizeof(int));
    }

    //Read Data: Dynamic - Supports any number of ticks and processes.
    while(p_num < process_count) {

        fscanf(f, "%d", &proc_id);
        fscanf(f, "%d", &live_t);
        fscanf(f, "%f", &live_a);

        t[p_num] = live_t;
        a[p_num] = live_a;

        for(int k = 0; k < tick_count; k++) {

            fscanf(f, "%d", &p_time);
            proc_1[p_num][k] = p_time;

            if (k == 0) {
                t_time[p_num][k] = live_t;
            } else {
                t_time[p_num][k] = ((int)live_a * proc_1[p_num][k - 1] + ((1 - (int)live_a) * t_time[p_num][k - 1]));
            }
        }
        p_num++;
    }

    for(int i = 0; i < process_count; i++) {
        proc_2[i] = (int *)malloc(tick_count * sizeof(int));
    }

    for(int i = 0; i < process_count; i++) {
        for(int j = 0; j < tick_count; j++){
            proc_2[i][j] = proc_1[i][j];
        }
    }
    fclose(f);

    simulate_SJF(proc_1);
    printf("\n");
    printf("Turnaround time: %d\n", turn_time);
    printf("Waiting time: %d\n", wait_time);
    printf("\n");

    simulate_SJFL(proc_2, t_time);
    printf("\n");
    printf("Turnaround time: %d\n", turn_time);
    printf("Waiting time: %d\n", wait_time);
    printf("Estimation Error: %d\n", error_count);

    free_all(proc_1, proc_2, t_time);

    return 0;
}

