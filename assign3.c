/* Mohamed Al Balushi, COMP322/L, Assign3*/
#include <stdio.h>
#include <stdlib.h>

/* Enumeration of menu choices */
enum MenuOption {
    None, EnterParameters, DetermineSafeSequence, Quit
};

/* Declare dynamic arrays and global variables*/
int *resource = NULL;
int *available = NULL;

int *max_claim = NULL;
int *allocated = NULL;
int *need = NULL;

int num_processes;
int num_resources;

/***************************************************************/
void PrintVectors() {
    /* declare local variables */
    int i;

    /* Header on chart/table */
    printf("%-8s%-8s%-8s\n", "", "Units", "Available");

    /* for loop: i = 0 up to but not including num_resources */
    for (i = 0; i < num_resources; i++) {
        /* print number of total units and available units of each resource index */
        printf("x%-7d%-8d%-8d\n", i, resource[i], available[i]);
    } /* for-loop */

    printf("\n");
}

/***************************************************************/
void PrintMatrices() {
    /* declare local variables */
    int i, j;

    /* Header on chart/table */
    printf("%-8s%-8s", "", "Maximum");
    for (i = 0; i < num_resources - 1; i++) {
        printf("%-8s", "");
    } /* for-loop */
    printf("%-8s", "Current");
    for (i = 0; i < num_resources - 1; i++) {
        printf("%-8s", "");
    } /* for-loop */
    printf("%-8s", "Potential");
    for (i = 0; i < num_resources - 1; i++) {
        printf("%-8s", "");
    } /* for-loop */
    printf("\n");

    printf("%-8s", "");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < num_resources; j++) {
            printf("r%-7d", j);
        }
    }
    printf("\n");

    /* for each process: i=0 up to but not including num_processes */
    for (i = 0; i < num_processes; i++) {
        printf("p%-7d", i);

        /* for each resource: j=0 up to but not including num_resources*/
        for (j = 0; j < num_resources; j++) {
            printf("%-8d", max_claim[i * num_resources + j]);
        } /* for-loop */

        /* for each resource: j=0 up to but not including num_resources */
        for (j = 0; j < num_resources; j++) {
            printf("%-8d", allocated[i * num_resources + j]);
        } /* for-loop */

        /* for each resource: j=0 up to but not including num_resources */
        for (j = 0; j < num_resources; j++) {
            printf("%-8d", need[i * num_resources + j]);
        } /* for-loop */

        printf("\n");
    } /* for-loop */

    printf("\n");
}

/****************************************************************/
void InitializeParameters() {
    /* declare local variables */
    int i, j;

    /* prompt for number of processes and number of resources */
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter number of resources: ");
    scanf("%d", &num_resources);

    /* allocate memory for vectors and arrays: resource, available, max_claim, allocated, need */
    resource = (int*) malloc(num_resources * sizeof(int));
    available = (int*) malloc(num_resources * sizeof(int));

    max_claim = (int*) malloc(num_processes * num_resources * sizeof(int));
    allocated = (int*) malloc(num_processes * num_resources * sizeof(int));
    need = (int*) malloc(num_processes * num_resources * sizeof(int));

    /* for each j, prompt for number of resource units, set available value */
    printf("Enter number of units for resources (r0 to r%d): ",
            num_resources - 1);
    for (j = 0; j < num_resources; j++) {
        scanf("%d", &(resource[j]));
        available[j] = resource[j];
    } /* for-loop */

    /* for each process i, for each resource j,
     * prompt for maximum number of units requested by process,
     * set need array value */
    for (i = 0; i < num_processes; i++) {
        printf(
                "Enter max units process p%d will request from each resource (r0 to r%d): ",
                i, num_resources - 1);
        for (j = 0; j < num_resources; j++) {
            scanf("%d", &(max_claim[i * num_resources + j]));
            need[i * num_resources + j] = max_claim[i * num_resources + j];
        } /* for-loop */
    } /* for-loop */

    /* for each process i, for each resource j,
     * prompt for number of resource units allocated to process,
     * update available & need values */
    for (i = 0; i < num_processes; i++) {
        printf(
                "Enter number of units of each resource (r0 to r%d) allocated to process p%d: ",
                num_resources - 1, i);
        for (j = 0; j < num_resources; j++) {
            scanf("%d", &(allocated[i * num_resources + j]));
            need[i * num_resources + j] -= allocated[i * num_resources + j];
            available[j] -= allocated[i * num_resources + j];
        } /* for-loop */
    } /* for-loop */

    /* print vectors and arrays */
    PrintVectors();
    PrintMatrices();

    return;
}

/***************************************************************/
void RunSafeSequenceAlgorithm() {
    /* declare local variables, including vector to indicate if process is safely sequenced and "num_sequenced" count */
    int i, j;
    int num_sequenced = 0;
    int *sequenced = (int*)calloc(num_processes, sizeof(int));
    int less_than_or_equal = 1;

    /* while not all processed are sequenced */
    while (num_sequenced < num_processes) {
        /* for each process: i */
        for (i = 0; i < num_processes; i++) {
            /* Reset less_than_or_equal for each process */
            less_than_or_equal = 1;

            /* Check if the current process is sequenced or not */
            if (sequenced[i] == 0) {
                /* Print checking message */
                printf("Checking: < ");
                for (j = 0; j < num_resources; j++) {
                    printf("%d ", need[i * num_resources + j]);
                }
                printf("< <= < ");
                for (j = 0; j < num_resources; j++) {
                    printf("%d ", available[j]);
                }
                printf("> :");

                /* for each resource: j */
                for (j = 0; j < num_resources; j++) {
                    less_than_or_equal &= (need[i * num_resources + j] <= available[j]);
                } // for-loop: j

                if (less_than_or_equal == 1) {
                    printf("p%d has been safely sequenced\n", i);
                    sequenced[i] = 1;

                    /* for each resource: j */
                    for (j = 0; j < num_resources; j++) {
                        /* update number of available units of resource */
                        available[j] += allocated[i * num_resources + j];
                        /* free all resources allocated to process */
                        allocated[i * num_resources + j] = 0;
                    } // for-loop: j

                    /* increment number of sequenced processes */
                    num_sequenced++;

                } // if (less_than_or_equal == 1)
                else { /* check for sequence was false */
                    printf("p%d could not be sequenced\n", i);
                }
            } /* if (sequenced[i] == 0) */
        } /* for-loop: i */
    } /* while-loop */

    free(sequenced);
}

/***************************************************************/
void FreeAllocatedMemory() {
    /* check if vectors/array are not NULL--if so, free each vector/array */
    if (resource != NULL) {
        free(resource);
        free(available);
        free(max_claim);
        free(allocated);
        free(need);
    }
}

/***************************************************************/
int main() {

    /* Declare local variables */
    int userChoice = None;

    /* Until the user quits, print the menu, prompt for the menu choice, call the appropriate procedure */
    while (userChoice != Quit) {
        printf("Banker's Algorithm\n"
                "------------------\n"
                "1) Enter parameters\n"
                "2) Determine safe sequence\n"
                "3) Quit program\n\n"
                "Enter selection: ");

        /* Get user choice */
        scanf("%d", &userChoice);

        switch (userChoice) {
        case EnterParameters:
            InitializeParameters();
            break;
        case DetermineSafeSequence:
            RunSafeSequenceAlgorithm();
            break;
        case Quit:
            FreeAllocatedMemory();
            printf("Quitting program...\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }

    return 1;
}


