/*
 * Susannah Bennett, 95333
 */

#define _XOPEN_SOURCE

#include <unistd.h>
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>

// Input global variables for number of sets, associativity (lines per set), and number of block bits
int s, E, b; 
// Flag for printing extra information
int verbose; 
// Pointer to FILE object
FILE *traceFile; 

// Variables for counting hits, misses, and evictions from cache
int hit, miss, eviction; 
// Variable for keeping track of time (for LRU)
int time = 0;

// Struct holding information about each line of a set
struct line {
	// Variable indicating whether the line is empty or not
	int vBit;
	// Variable for the tag identifier
	int tag; 
	// Variable for counting how many times a line was accessed (used for eviction policy)
	int counter; 
} ;

// Struct holding the set of lines and count of lines
struct set {
	// Array of lines in a given set
	struct line *lines; 
	// Variable for the count of the number of lines in a given set
	int filledCount; 
} ;

// Struct for the cache
struct set *sets;

/*
 * Function for checking the cache and handling hits/misses/evictions
 *
 * Param: addr - the inputed address
 */
void checkCache(unsigned long addr) {
	// Variable for the tag from the address input
	int tagTemp = addr >> (b+s); 
	// Variable for the set number from the address input
	int setNum = (addr >> b) & ((1 << s)-1); 

	// Variables for tracking positions in the for loops
	int k, m, minIndex, min; 

	for (k = 0; k < E; k++) {
		if ((sets[setNum].lines[k].tag == tagTemp) && (sets[setNum].lines[k].vBit)) {
			// Hit case when the line is filled (valid bit set) and tag matches
			if (verbose) {
				printf(" hit");
			}
			hit++;
			// Add a time stamp to the line (determining how recently it was used)
			sets[setNum].lines[k].counter = time++; 
			return;
		}
	}

	// If it hasn't returned already, it must be a miss
	miss++;

	if (verbose){
		printf(" miss");
	}

	// Find the LRU line, by finding the minimum value for counter
	minIndex = 0;
	min = sets[setNum].lines[0].counter;

	for (m = 1; m < E; m++){
		if ((sets[setNum].lines[m].counter) < min) {
			min = sets[setNum].lines[m].counter;
			minIndex = m;
		}
	}

	// If the LRU line already has an entry (if valid bit is set), you must evict
	if (sets[setNum].lines[minIndex].vBit){
		eviction++;
		if (verbose){
			printf(" eviction");
		}
	}

	// Change the LRU to be the given entry (valid bit, tag, time stamp)
	sets[setNum].lines[minIndex].vBit = 1;
	sets[setNum].lines[minIndex].tag = tagTemp;
	sets[setNum].lines[minIndex].counter = time++;
}


int main(int argc, char **argv)
{
	char operation; // Variable for when we differentiate parsing trace entries
	unsigned long address; // Variable for given address of trace entry
	int size; // Variable for the given size of trace entry
	char c; // Variable for switch statement

    verbose = 0; // Verbose is initially set to false

    while( (c=getopt(argc,argv,"hvs:E:b:t:")) != -1){
        switch(c){
        case 'h':
        	// Prints help message
    		printf("   -h          Print this help message.\n");
    		printf("   -v          Optional verbose flag.");
   			printf("   -s <num>    Number of set index bits.\n");
   			printf("   -E <num>    Number of lines per set.\n");
   			printf("   -b <num>    Number of block offset bits.\n");
   			printf("   -t <file>   Trace file.\n");
    		exit(1);
        case 'v':
        	// Sets verbose flag to true
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
        	b = atoi(optarg);
        	break;
        case 't':
        	// Sets trace file
        	traceFile = fopen(optarg, "r");
        	if (traceFile == NULL){
        		perror("File not found");
        		return 1;
        	}
        	break;
        default:
            printf("./tracegen failed to parse its options.\n");
            exit(1);
        }
    }

    // Allocating for sets (num of items, size of each item)
    sets = calloc((1 << s), sizeof(struct set)); 

    // Allocating space for every line in a set
    int i;
    for(i = 0; i < (1<<s); i++) {
    	sets[i].lines = calloc(E, sizeof(struct line));
    }

    // For parsing and handling trace file entries:
    while ((fscanf(traceFile, "%c %lx, %d\n", &operation, &address, &size)) != EOF) {
    	if (verbose) {
    		// When verbose is true, print all of the extra information
    		printf("%c %lx, %d", operation, address, size);
    	}

    	// Determining operation type
    	if (operation == 'I'){
    		// Do nothing for an instruction load
    		continue;
    	} else if (operation == 'L'|| operation == 'S'){
    		// Check cache for the data load or store (effectively the same in this lab)
    		checkCache(address);
    	} else if (operation == 'M'){
    		// Calling checkCache with either be a hit or will result in the entry being added (which then becomes a hit)
    		checkCache(address);
    		// A load followed by a store, which will result in a hit regardless
    		hit++;
    		if (verbose) {
    			printf(" hit");
    		}
    	}
    	printf("\n");
    }

    // Close the file at the end
    int err;
    err = fclose(traceFile);
    if (err == EOF){
    	perror("File not found");
    	return 1;
    }

    // De-allocate the space
    for (i = 0; i < (1 << s); i++) {
    	free(sets[i].lines);
    }
    free(sets);

    printSummary(hit, miss, eviction);

    return 0;
}