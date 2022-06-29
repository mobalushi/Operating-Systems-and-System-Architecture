#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Declare global variables, including dynamic array to store sequence of traversed tracks
int sequence_size;
int *sequence = NULL;

/*********************************************************/
void InitializeParameters() {
    // declare local vars
    int i;

    // prompt for the sequence size
    printf("Enter size of sequence: ");
    fflush(stdout);
    scanf("%d", &sequence_size);

    // allocate memory for the sequence of traversed tracks
    sequence = (int *) malloc(sequence_size * sizeof(int));

    // prompt for starting track, store in index 0
    printf("Enter starting track: ");
    fflush(stdout);
    scanf("%d", &(sequence[0]));

    // prompt for sequence of tracks to seek, store in index 1 to "sequence size-1"
    printf("Enter sequence of tracks to seek: ");
    fflush(stdout);
    for (i = 1; i < sequence_size; i++) {
        scanf("%d", &(sequence[i]));
    } // for

    return;

} // InitializeParameters

/*********************************************************/
void CalculateDistanceFIFO() {
    // declare local variables
    int i;
    int total_distance = sequence[0];

    // begin printing sequence of traversal
    printf("Traversed sequence: %d", sequence[0]);

    // calculate total distance of traversed tracks
    for (i = 1; i < sequence_size; i++) {
        total_distance += abs(sequence[i] - sequence[i - 1]);
        // print sequence of traversal
        printf(" %d", sequence[i]);
    } // for

    // print total distance of tracks traversed
    printf("\nThe distance of the traversed tracks is: %d\n", total_distance);

    return;

} // CalculateDistanceFIFO

/*********************************************************/
void CalculateDistanceSSTF() {
    // declare local variables
    int i;
    int shortest_distance;
    int shortest_index;
    int *done = (int *) calloc(sequence_size, sizeof(int));

    // initialize current track and distance traversed to starting track
    int current_track = sequence[0];
    done[0] = 1;
    int total_distance = current_track;
    int num_done = 1;

    // begin printing sequence of traversal
    printf("Traversed sequence: %d", sequence[0]);

    // until every track is traversed
    while (num_done < sequence_size) {
        // initialize shortest distance to INT_MAX
        shortest_distance = INT_MAX;

        // for each track in sequence
        for (i = 1; i < sequence_size; i++) {
            // if not already traversed
            if (done[i] == 0) {
                // if distance to traverse is shorter than current shortest distance
                if (abs(current_track - sequence[i]) <= shortest_distance) {
                    // set current shortest distance and index of the track w/ shortest distance
                    shortest_distance = abs(current_track - sequence[i]);
                    shortest_index = i;
                } // if
            } // if
        } // for

        // set "done" value for track w/shortest distance to 1 (mark as traversed)
        done[shortest_index] = 1;

        // increment number of tracks that have been traversed
        num_done++;

        // update total distance traversed
        total_distance += shortest_distance;

        //set current track to new position, print position
        current_track = sequence[shortest_index];

        // print position
        printf(" %d", current_track);
    } // while

    // print total distance traversed
    printf("\nThe distance of the traversed tracks is: %d\n", total_distance);

    // free dynamically allocated array
    free(done);

    return;

} // CalculateDistanceSSTF

/*********************************************************/
void CalculateDistanceScan() {
    // declare local variables
    int i;
    int direction;
    int at_least_one;
    int shortest_distance;
    int shortest_index;
    int *done = (int *) calloc(sequence_size, sizeof(int));

    //prompt for initial direction (0=descreasing, 1=increasing)
    printf("Enter initial direction: (0=decreasing, 1=increasing): ");
    fflush(stdout);
    scanf("%d", &direction);

    // initialize current track and distance traversed to starting track
    int current_track = sequence[0];
    done[0] = 1;
    int total_distance = current_track;
    int num_done = 1;

    // begin printing sequence of traversal
    printf("Traversed sequence: %d", sequence[0]);

    // until every track is traversed
    while (num_done < sequence_size) {
        // initialize shortest distance to INT_MAX
        shortest_distance = INT_MAX;
        at_least_one = 0;

        // for each track in sequence
        for (i = 1; i < sequence_size; i++) {
            // if not already traversed
            if (done[i] == 0) {
                //if distance to traverse is shorter than current shortest distance in the current direction
                if (abs(current_track - sequence[i]) <= shortest_distance) {
                    if ( ( ( (sequence[i] - current_track) > 0) && (direction == 1) ) ||
                         ( ( (sequence[i] - current_track) < 0) && (direction == 0) ) ) {
                        // set current shortest distance and index of the track w/ shortest distance
                        shortest_distance = abs(current_track - sequence[i]);
                        shortest_index = i;
                        // set flag that at least one track was traversed
                        at_least_one = 1;
                    }
                } // if
            } // if
        } // for

        // if at least one track was traversed
        if (at_least_one == 1) {
            // set "done" value for track w/shortest distance to 1 (mark as traversed)
            done[shortest_index] = 1;

            // increment number of traversed tracks
            num_done++;

            // update total distance traversed
            total_distance += shortest_distance;

            //set current track to new position, print position
            current_track = sequence[shortest_index];

            // print position
            printf(" %d", current_track);
        } // if
        else {
            // else change direction
            direction = 1 - direction;
        } // else
    } // while

    // print total distance traversed
    printf("\nThe distance of the traversed tracks is: %d\n", total_distance);

    // free dynamically allocated array
    free(done);

    return;

} // CalculateDistanceScan

/***************************************************************/
void FreeMemory() {
    // if sequence is not NULL, free sequence
    if (sequence != NULL) {
        free(sequence);
    }

    return;
} // FreeMemory

/*********************************************************/
void CalculateDistanceCScan() {
    // declare local variables
    int i;
    int at_least_one;
    int shortest_distance;
    int shortest_index;
    int largest_track = 0;
    int *done = (int *) calloc(sequence_size, sizeof(int));

    // initialize current track and number traversed to starting track
    int current_track = sequence[0];
    done[0] = 1;
    int total_distance = current_track;
    int num_done = 1;

    // begin printing sequence of traversal
    printf("Traversed sequence: %d", sequence[0]);

    // until every track is traversed
    while (num_done < sequence_size) {
        // initilize shortest distance to INT_MAX
        shortest_distance = INT_MAX;
        at_least_one = 0;

        // for each track in sequence
        for (i = 1; i < sequence_size; i++) {
            // if not already traversed
            if (done[i] == 0) {
                //if distance to traverse is shorter than current shortest distance and a positive value (only increasing direction)
                if ( ((sequence[i] - current_track) < shortest_distance) &&
                     ((sequence[i] - current_track) > 0) ) {
                    // set current shortest distance and index of the track w/ shortest distance
                    shortest_distance = sequence[i] - current_track;
                    shortest_index = i;
                    // set flag that at least one track was traversed
                    at_least_one = 1;
                } // if
            } // if
        } // for

        // if at least one track was traversed
        if (at_least_one == 1) {
            // set "done" value for track w/shortest distance to 1 (mark as traversed)
            done[shortest_index] = 1;

            // increment number of tracks that have been traversed
            num_done++;

            // if largest track was reached
            if (largest_track == 1) {
                // update total distance traversed by decrementing by distance to track (subtracts distance from 0 to track)
                total_distance -= sequence[shortest_index];

                // reset "largest track" flag
                largest_track = 0;
            } // if
            else {
                // update total distance traversed by distance to track
                total_distance += shortest_distance;
            } // else

            //set current track to new position, print position
            current_track = sequence[shortest_index];

            // print position
            printf(" %d", current_track);
        } // if
        else {
            // update total distance by current track (adds remaining distance before going back to 0)
            total_distance += current_track;

            // reset current track to 0 (loop back around)
            current_track = 0;

            // set "largest_track" flag to 1
            largest_track = 1;
        } //
    } // while

    // print total distance traversed
    printf("\nThe distance of the traversed tracks is: %d\n", total_distance);

    // free dynamically allocated array
    free(done);

    return;

} // CalculateDistanceCScan

/***************************************************************/
int main() {
    /* declare local vars */
    int user_choice = 0;

    /* while user has not chosen to quit */
    while (user_choice != 6) {
        /* print menu of options */
        printf("\nDisk scheduling\n"
                "---------------\n"
                "1) Enter parameters\n"
                "2) Calculate distance to traverse tracks using FIFO\n"
                "3) Calculate distance to traverse tracks using SSTF\n"
                "4) Calculate distance to traverse tracks using Scan\n"
                "5) Calculate distance to traverse tracks using C-Scan\n"
                "6) Quit program and free memory\n\n");

        /* prompt for menu selection */
        printf("Enter selection: ");
        fflush(stdout);
        scanf("%d", &user_choice);

        /* call appropriate procedure based on choice--use switch statement */
        switch (user_choice) {
        case 1:
            InitializeParameters();
            break;
        case 2:
            CalculateDistanceFIFO();
            break;
        case 3:
            CalculateDistanceSSTF();
            break;
        case 4:
            CalculateDistanceScan();
            break;
        case 5:
            CalculateDistanceCScan();
            break;
        case 6:
            FreeMemory();
            printf("Quitting program...\n");
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }

    } /* while loop */

     return 1; /* indicates success */

} // main
