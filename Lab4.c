//Mohamed Al Balushi, Lab4, COMP322L
#include <stdio.h>
#include <stdlib.h>

// declare structure to store block information (id, starting address, ending address, link to next block)
struct node {
    int id;
    int start;
    int end;
    struct node *link;
};

// declare linked list to connect allocation blocks
typedef struct node block_type;
block_type *block_list = NULL;

// declare global variables
int pm_size;
int hole_algo;
int remaining;

/********************************************************************/
void InitializeParameters() {
    // Set hole_algo to an invalid value initially
    hole_algo = -1;

    // prompt for size of physical memory and choice of hole-fitting algorithm (0=first-fit, 1=best-fit), initialize remaining memory
    printf("Enter size of physical memory: ");
    fflush(stdout);
    scanf("%d", &pm_size);
    remaining = pm_size;

    // Loop until a valid value in entered by the user
    while (hole_algo < 0 || hole_algo > 1) {
        printf("Enter hole-fitting algorithm (0=first fit, 1=best_fit): ");
        fflush(stdout);
        scanf("%d", &hole_algo);
    }

    // initilize linked list with "dummy" block of size 0
    block_list = (block_type*) malloc(sizeof(block_type));
    block_list->id = -1;
    block_list->start = 0;
    block_list->end = 0;
    block_list->link = NULL;

    return;
}

/********************************************************************/
void PrintAllocationTable() {

    // declare local variables
    block_type *current_block = block_list->link;

    // print table header
    printf("\n%-6s%-8s%-5s\n-------------------\n", "ID", "Start", "End");

    // print table containing block id, starting address, ending address
    while (current_block != NULL) {
        // printing 1 row at a time
        printf("%-6d%-8d%-5d\n", current_block->id, current_block->start,
                current_block->end);
        current_block = current_block->link;
    }

    return;
}

/********************************************************************/
void AllocateMemoryForBlock() {
    // declare local variables
    block_type *new_block;
    block_type *current_block;
    block_type *best_block;
    int at_least_one = 0;
    int block_id = 0;
    int block_size = 0;
    int hole_start = 0;
    int hole_end = 0;
    int hole_size = 0;
    int best_start = 0;

    // initialize best hole so far to size of physical memory
    int best_hole = pm_size;

    // prompt for block id & block size
    printf("Enter block id: ");
    fflush(stdout);
    scanf("%d", &block_id);

    printf("Enter block size: ");
    fflush(stdout);
    scanf("%d", &block_size);

    // check if size of block is larger than remaining unallocated space, if so, print message and return
    if (block_size > remaining) {
        printf("Out of memory!\n");
        return;
    }

    // allocate space for new block and set id
    new_block = (block_type*) malloc(sizeof(block_type));
    new_block->id = block_id;

    // if only "dummy" block exists, insert block at end of linked list, set fields, return
    if (block_list->link == NULL) {
        new_block->start = 0;
        new_block->end = block_size;
        new_block->link = NULL;
        block_list->link = new_block;
        remaining -= block_size;
        return;
    }

    // else traverse list until either appropriate hole is found or the end of the list is reached
    current_block = block_list->link;
    while (current_block != NULL) {
        // if id already exists, reject request and return
        if (current_block->id == block_id) {
            printf("Block ID already exists!\n");
            return;
        }

        // set values for start and end of currently found hole
        hole_start = current_block->end;
        if (current_block->link != NULL) {
            hole_end = current_block->link->start;
        } else {
            hole_end = pm_size;
        }

        hole_size = hole_end - hole_start;

        // if hole is large enough for block
        if (hole_size >= block_size) {
            at_least_one = 1;

            // if first-fit algorithm
            if (hole_algo == 0) {
                // set start & end fields of new block & add block into linked list
                new_block->start = hole_start;
                new_block->end = hole_start + hole_size;
                new_block->link = current_block->link;
                current_block->link = new_block;
                // reduce remaining available memory and return
                remaining -= hole_size;
                return;
            } // FIRST-FIT
              // else--best-fit algorithm
            else {
                // if hole is smaller than best so far
                if (hole_size < best_hole) {
                    // set values of best start & best end & best hole size so far
                    best_start = hole_start;
                    best_hole = hole_end - hole_start;
                    // update best block & advance next block
                    best_block = current_block;
                } // found better fitting hole
            } // BEST-FIT
        } // Large enough hole

        // advance next block
        current_block = current_block->link;
    }

    if (at_least_one == 0) {
        printf("Cannot fit block!\n");
        return;
    }

    // set start & end fields of new block & add block into linked list
    new_block->start = best_start;
    new_block->end = best_start + block_size;
    new_block->link = best_block->link;
    best_block->link = new_block;
    // reduce remaining available memory and return
    remaining -= block_size;

    return;
}

/********************************************************************/
void DeallocateMemoryForBlock() {
    // declare local variables
    block_type *current_block, *prev_block;
    int block_id = 0;

    // prompt for block id
    printf("Enter block id: ");
    fflush(stdout);
    scanf("%d", &block_id);

    current_block = block_list;
    // until end of linked list is reached or block id is found
    while ((current_block != NULL) && (current_block->id != block_id)) {
        prev_block = current_block;
        // advance next block
        current_block = current_block->link;
    } // while

    // if end of linked list reached, print block id not found
    if (current_block == NULL) {
        printf("Block ID not found!\n");
        return;
    }

    // else remove block and deallocate memory
    prev_block->link = current_block->link;
    remaining += current_block->end - current_block->start;
    free(current_block);

    return;
}

/********************************************************************/
void DefragmentMemory() {
    // declare local variables
    block_type *current_block;
    int block_size;
    int prev_end = 0;

    current_block = block_list;
    // until end of list is reached
    while (current_block != NULL) {
        // calculate current hole size
        block_size = current_block->end - current_block->start;

        // adjust start & end fields of current block to eliminate hole
        current_block->start = prev_end;
        current_block->end = current_block->start + block_size;
        prev_end = current_block->end;
        current_block = current_block->link;
    } // while

    return;
}

/********************************************************************/
void FreeBlocks(block_type *node) {
    // if node is NULL, return
    if (node == NULL) {
        return;
    }
    // else
    else {
        //recursively call procedure on node->link
        FreeBlocks(node->link);
        // deallocate memory from node
        free(node);
    }

    return;
}

/***************************************************************/
int main() {
    /* declare local vars */
    int user_choice = 0;

    /* while user has not chosen to quit */
    while (user_choice != 5) {
        /* print menu of options */
        printf("\nMemory allocation\n"
                "-----------------\n"
                "1) Enter parameters\n"
                "2) Allocate memory for block\n"
                "3) Deallocate memory for block\n"
                "4) Defragment memory\n"
                "5) Quit program\n\n");

        /* prompt for menu selection */
        printf("Enter selection: ");
        fflush(stdout);
        scanf("%d", &user_choice);

        /* call appropriate procedure switch statement */
        switch (user_choice) {
        case 1:
            InitializeParameters();
            break;
        case 2:
            AllocateMemoryForBlock();
            PrintAllocationTable();
            break;
        case 3:
            DeallocateMemoryForBlock();
            PrintAllocationTable();
            break;
        case 4:
            DefragmentMemory();
            PrintAllocationTable();
            break;
        case 5:
            FreeBlocks(block_list);
            printf("Quitting program...\n");
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }
    } /* while loop */

    return 1; /* indicates success */
} /* end of procedure */
