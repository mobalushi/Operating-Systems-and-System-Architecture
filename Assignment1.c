//Mohamed Al Balushi, COMP322L, Assignment1

#include <stdio.h>
#include <stdlib.h>

/* Define structures and global variables*/

int num_processes;

struct node1 {
	int process;
	struct node1 *link;
};

typedef struct node1 linked_list_type;

struct node2 {
	int parent;
	linked_list_type *children;
} *pcb = NULL;

typedef struct node2 pcb_type;

/***************************************************************/
void print_hierarchy_of_processes() {
	/* declare local vars */
	linked_list_type *next = NULL;

	/* for each PCB index i from 0 up to (but not including) maximum number*/
	for (int i = 0 ; i < num_processes; ++i) {
		/* check if PCB[i] has a parent and children */
		if ((pcb[i].parent != -1) && (pcb[i].children != NULL)) {
			/* print message about children of the current PCB[i] */
			printf("PCB[%d] is the parent of: ", i);
			/* intialize variable to head of list of children */
			next = pcb[i].children;

			/* while the end of the linked list of children is not reached */
			while (next != NULL) {
				/* print message about current child process index */
				printf("PCB[%d] ", next->process);
				/* move to next node in linked list */
				next = next->link;
			}/* while */
			/* print newline */
			printf("\n");
		}/* if */
	} /* for */
} /* end of procedure */

/***************************************************************/
void get_parameters() {
	/* declare local vars */
	int i;

	/* prompt for maximum number of processes */
	printf("Enter maximum number of processes: ");
	scanf("%d", &num_processes);

	/* allocate memory for dynamic array of PCBs */
	pcb = (pcb_type *)malloc(num_processes * sizeof(pcb_type));

	/* Define PCB[0] */
	pcb[0].parent = 0;
	pcb[1].children = NULL;

	/* for-loop to intitialize all other indices' parent to -1 */
	for (i = 1; i < num_processes; ++i) {
		pcb[i].parent = -1;
		pcb[i].children = NULL;
	} /* for-loop */

	return;
} /* end of procedure */


/***************************************************************/
void create_child_process() {
	/* define local vars */
	int p;
	int q = 1;
	linked_list_type *new_child;

	/* prompt for parent process index p */
	printf("Enter the parent process index: ");
	scanf("%d", &p);

	/* search for first available index q without a parent in a while loop */
	while(pcb[q].parent != -1) {
		q++;
	}/* while-loop */

	/* allocate memory for new child process, initialize fields */
	new_child = (linked_list_type *)malloc(sizeof(linked_list_type));
	new_child->process = q;
	new_child->link = NULL;

	/* record the parent's index p in PCB[q] */
	pcb[q].parent = p;
	/* initialize the list of children of PCB[q] as empty */
	pcb[q].children = NULL;

	/* create a new link containing the child's index q and append the link to the end of the linked list of PCB[p] */
	if (pcb[p].children == NULL) {
		pcb[p].children = new_child;
	}
	else {
		linked_list_type *next = pcb[p].children;
		while (next->link != NULL) {
			next = next->link;
		}/* while-loop */

		next->link = new_child;
	}/* else */

	/* call procedure to print current hierarchy of processes */
	print_hierarchy_of_processes();

	return;
} /* end of procedure */


/***************************************************************/
void destroy_children(linked_list_type *node) {
	/* declare local vars */
	int q;

	/* check if end of linked list--if so return */
	if (node == NULL) {
		return;
	} /* if */
	/* else call self on next node in linked list */
	else {
		destroy_children(node->link);

		/* set variable q to current node's process index field */
		q = node->process;

		/* call self on children of PCB[q] */
		destroy_children(pcb[q].children);

		/* free memory of paramter */
		free(node);

		/* reset parent of PCB[q] to -1 */
		pcb[q].parent = -1;

		/* set paramter to NULL */
		node = NULL;
	}/* end of else */

	return;
} /* end of procedure */


/***************************************************************/
void destroy_all_descendants() {
	/* declare local vars */
	int p;

	/* prompt for process index p */
	printf("Enter the index of the process whose descendants are to be destroyed: ");
	scanf("%d", &p);

	/* call recursive procedure to destroy children of PCB[p] */
	destroy_children(pcb[p].children);

	/* reset children of PCB[p] to NULL */
	pcb[p].children = NULL;

	/* call procedure to print current hierarchy of processes */
	print_hierarchy_of_processes();

	return;
} /* end of procedure */


/***************************************************************/
void free_memory() {
	/* check if PCB is non null)	 */
	if (pcb != NULL) {
		/* check if children of PCB[0] is not null */
		if (pcb[0].children != NULL) {
			destroy_children(pcb[0].children);
		}/* if */

		/* free memory of PCB */
		free(pcb);
	} /* if */

	return;
} /* end of procedure */


/***************************************************************/
int main() {
	/* declare local vars */
	int choice = 0;

	/* while user has not chosen to quit */
	while (choice != 4) {
		/* print menu of options */
		printf("\nProcess creation and destruction\n"
				"--------------------------------\n"
				"1) Enter parameters\n"
				"2) Create a new child process\n"
				"3) Destroy all descendants of a process\n"
				"4) Quit program and free memory\n\n");

		/* prompt for menu selection */
		printf("Enter selection: ");
		scanf("%d", &choice);

		/* call appropriate procedure based on choice--use switch statement or series of if, else if, else statements */
		switch (choice) {
		case 1:
			get_parameters();
			break;
		case 2:
			create_child_process();
			break;
		case 3:
			destroy_all_descendants();
			break;
		case 4:
			free_memory();
			printf("Quitting program...\n");
			break;
		}/* while loop */
	}

	 return 1; /* indicates success */
} /* end of procedure */
