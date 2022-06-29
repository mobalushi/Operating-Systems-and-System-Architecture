
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* declare global variables including a table structure to hold scheduling information */
struct node {
    /* Process data */
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int already_started;
    int end_time;
    int turnaround_time;
}*table = NULL;

/* global variables */
int number_of_processes;

typedef struct node table_type;

/* a function that finds the maximum of two integers */
#define max(a, b) (a > b ? a : b)

/* Enumeration of choices */
enum Choice {
    None = 0, EnterParameters, ScheduleFIFO, ScheduleSJF, ScheduleSRT, QuitAndFree, ChoiceCount
};


void print_scheduling_table() {
    /* declare local variables */
    int i;

    /* print table header */
    printf("\nID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");

    /* for each process i=0 up to but not including number of processes */
    for (i = 0; i < number_of_processes; i++) {
        /* print the contents (id, arrival, total_cycles) of each field of the table's index  */
        printf("%d\t%d\t%d", table[i].id, table[i].arrival, table[i].total_cpu);

        /* if process has been scheduled ("done" field is 1, print other contents (start time, end time, turnaround time) ) */
        if (table[i].done == 1) {
            printf("\t%d\t%d\t%d\n", table[i].start_time, table[i].end_time, table[i].turnaround_time);
        }
        else {
            printf("\n");
        }
    }

    return;
}


void initialize_parameters() {
    /* declare local variables */
    int i;

    /* prompt for total number of processes */
    printf("Enter total number of processes: ");
    scanf("%d", &number_of_processes);

    /* allocate memory for table to hold process parameters */
    table = (table_type *) malloc(number_of_processes * sizeof(table_type));

    /* for each process i=0 up to but not including number of processes */
    for (i = 0; i < number_of_processes; i++) {
        /* prompt for process id, arrival time, and total cycle time */
        printf("Enter process id: ");
        scanf("%d", &(table[i].id));

        printf("Enter arrival cycle for process P[%d]: ", i + 1);
        scanf("%d", &(table[i].arrival));

        printf("Enter total cycles for process P[%d]: ", i + 1);
        scanf("%d", &(table[i].total_cpu));
    }

    /* print the contents of the table */
    print_scheduling_table();

    return;
}


void schedule_fifo() {
    /* declare (and initialize when appropriate) local variables */
    int i;
    int num_done = 0;
    int min_value;
    int min_index;
    int current_cycle = 0;

    /* for each process reset "done" field to 0 */
    for (i = 0; i < number_of_processes; i++) {
        table[i].done = 0;
    }/* for-loop */

    /* while there are still processes to schedule */
    while (num_done < number_of_processes) {
        /* initialize the earliest arrival time to INT_MAX (largest integer value) */
        min_value = INT_MAX;

        /* for each process not yet scheduled */
        for (i = 0; i < number_of_processes; i++) {
            if (table[i].done == 0) {
                /* check if process has earlier arrival time
                than current earliest and update */
                if (table[i].arrival < min_value) {
                    min_value = table[i].arrival;
                    min_index = i;
                } /* if */
            }/* if */
        }/* for-loop */

        /* set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time */
        table[min_index].start_time = current_cycle;
        table[min_index].end_time = current_cycle + table[min_index].total_cpu;
        table[min_index].turnaround_time = table[min_index].end_time - table[min_index].arrival;
        table[min_index].done = 1;

        /* update current cycle time and increment number of processes scheduled */
        current_cycle += table[min_index].total_cpu;
        num_done++;
    }/* while-loop */

    /* print the contents of the table */
    print_scheduling_table();

    return;
}


void schedule_sjf() {
    /* declare (and initialize when appropriate) local variables */
    int i;
    int num_done = 0;
    int min_value;
    int min_index;
    int current_cycle = 0;

    /* for each process reset "done" field to 0 */
    for (i = 0; i < number_of_processes; i++) {
        table[i].done = 0;
    }/* for-loop */

    /* while there are still processes to schedule */
    while (num_done < number_of_processes) {
        /* initialize the smallest total cpu time to INT_MAX (largest integer value) */
        min_value = INT_MAX;

        /* for each process not yet scheduled */
        for (i = 0; i < number_of_processes; i++) {
            if (table[i].done == 0) {
                /* check if process has lower total cycle than current lowest
                    and has arrival time less than or equal to current cycle time and update */
                if ((table[i].total_cpu < min_value) && (table[i].arrival <= current_cycle)) {
                    min_value = table[i].total_cpu;
                    min_index = i;
                } /* if */
            }/* if */
        }/* for-loop */

        /* set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time */
        table[min_index].start_time = current_cycle;
        table[min_index].end_time = current_cycle + table[min_index].total_cpu;
        table[min_index].turnaround_time = table[min_index].end_time - table[min_index].arrival;
        table[min_index].done = 1;

        /* update current cycle time and increment number of processes scheduled */
        current_cycle += table[min_index].total_cpu;
        num_done++;
    }/* while-loop */

    /* print the contents of the table */
    print_scheduling_table();

    return;
}


void schedule_srt() {
    /* declare (and initialize when appropriate) local variables */
    int i;
    int num_done = 0;
    int min_value;
    int min_index;
    int current_cycle = 0;

    /* for each process reset "done" and "already_started" fields to 0 and "total_remaining" to total CPU cycles, */
    for (i = 0; i < number_of_processes; i++) {
        table[i].done = 0;
        table[i].already_started = 0;
        table[i].total_remaining = table[i].total_cpu;
    }/* for-loop */

    /* while there are still processes to schedule */
    while (num_done < number_of_processes) {
        /* initialize the lowest remaining time to INT_MAX (largest integer value) */
        min_value = INT_MAX;

        /* for each process not yet scheduled */
        for (i = 0; i < number_of_processes; i++) {
            if (table[i].done == 0) {
                if ((table[i].total_remaining < min_value) && (table[i].arrival <= current_cycle)) {
                    min_value = table[i].total_remaining;
                    min_index = i;
                }/* if */
            }/* if */
        }/* for-loop */

        /* check if process was not partially-scheduled */
        if (table[min_index].already_started == 0) {
            /* if so, set "start time", "already_started" fields of process with lowest (and available) total remaining cycle time */
            table[min_index].start_time = current_cycle;
            table[min_index].already_started = 1;
        }/* if */

        /* set end time, turnaround time of process with lowest (and available) total remaining cycle time */
        table[min_index].end_time = current_cycle + 1;
        table[min_index].turnaround_time = table[min_index].end_time - table[min_index].arrival;

        /* decrement total remaining time of process with lowest (and available) total remaining cycle time*/
        table[min_index].total_remaining--;

        /* if remaining time is 0, set done field to 1, amd increment number of scheduled processes */
        if (table[min_index].total_remaining == 0) {
            table[min_index].done = 1;
            num_done++;
        }/* if */

        /* increment cycle time*/
        current_cycle++;
    }/* while-loop */

    /* print the contents of the table */
    print_scheduling_table();

    return;
}


void free_table() {
    /* free table if not NULL */
    if (table != NULL) {
        free(table);
        table = NULL;
    }
}


int main(void) {
    /* Declare and initialize local variables */
    int user_choice = None;

    /* Loop until the user chooses to exit. */
    while (user_choice != QuitAndFree) {
        /* Print the menu */
        printf("\nBatch scheduling\n"
                "---------------- \n"
                "1) Enter parameters\n"
                "2) Schedule processes with FIFO algorithm\n"
                "3) Schedule processes with SJF algorithm\n"
                "4) Schedule processes with SRT algorithm\n"
                "5) Quit and free memory\n");

        /* prompt user to make a choice */
        printf("\nEnter selection: ");
        fflush(stdout);
        scanf("%d", &user_choice);

        /* If the choice is not valid, prompt to retry */
        while (user_choice <= None || user_choice >= ChoiceCount) {
            printf("Invalid user choice! Please retry: ");
            fflush(stdout);
            scanf("%d", &user_choice);
        }

        /* perform the choosen operation */
        switch(user_choice) {
        case EnterParameters:
            initialize_parameters();
            break;
        case ScheduleFIFO:
            schedule_fifo();
            break;
        case ScheduleSJF:
            schedule_sjf();
            break;
        case ScheduleSRT:
            schedule_srt();
            break;
        case QuitAndFree:
            free_table();
            break;
        default:
            break;
        }/* switc-case */
    }/* while-loop */

    printf("Quitting program...\n");

	return 1;
}

