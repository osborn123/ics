#include "cache.h"
#include <stdlib.h>
#include "math.h"
// Cache statistics counters.
uint32_t cache_total_accesses;
uint32_t cache_hits;
uint32_t cache_misses;
uint32_t cache_read_accesses;
uint32_t cache_read_hits;
uint32_t cache_write_accesses;
uint32_t cache_write_hits;
uint32_t index_size;
uint32_t offset_size;
int  block;
uint32_t cache_lines;
uint32_t cache_line;
uint32_t blocks;
uint32_t tag_size;
uint32_t set;




// Input parameters to control the cache.
uint32_t cache_size;
uint32_t cache_associativity;
uint32_t cache_block_size;

block_t **cache;

/*
 * Perform a read from the memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to read from memory.
 */
int read_from_memory(uint32_t pa)
{
return 0;
}

/*
 * Perform a write from the cache to memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to write to memory.
 */
int write_to_memory(uint32_t pa)
{
return 0 ;
}

/*
 *********************************************************
  Please edit the below functions for Part 1.
  You are allowed to add more functions that may help you
  with your implementation. However, please do not add
  them in any file. All your new code should be in cache.c
  file below this line.
 *********************************************************
*/

/*
 * Initialize the cache depending on the input parameters S, A, and B
 * and the statistics counter. The cache is declared in as extern in
 * include/cache.h file.
 */
void initialize_cache()
{
    int number_sets;
    int block_per_set;

    if (cache_associativity == 1) {
        number_sets = cache_size / cache_block_size;
        block_per_set = 1;
    } else if (cache_associativity == 2) {
        number_sets = 1;
        block_per_set = cache_size / cache_block_size;
    } else {
        int num_ways = (int)pow(2, cache_associativity - 2);
        number_sets = cache_size / (cache_block_size * num_ways);
        block_per_set = num_ways;
    }
    cache = malloc(number_sets * sizeof(block_t *));
    for (int i = 0; i < number_sets; i++) {
        cache[i] = malloc(block_per_set * sizeof(block_t));
        for (int j = 0; j < block_per_set; j++) {
            cache[i][j].valid = 0;
            cache[i][j].dirty = 0;
        }
    }
}

/*
 * Free the allocated memory for the cache to avoid memory leaks.
 */
void free_cache()
{
    int num_sets;
    if (cache_associativity == 1) {
        num_sets = cache_size / cache_block_size;
    } else if (cache_associativity == 2) {
        num_sets = 1;
    } else {
        int num_ways = (int)pow(2, cache_associativity - 2);
        num_sets = cache_size / (cache_block_size * num_ways);
    }
    for (int i = 0; i < num_sets; i++) {
        free(cache[i]);
    }
    free(cache);
    return;
}

// Print cache statistics.
void print_cache_statistics()
{
	printf("\n* Cache Statistics *\n");
	printf("total accesses: %d\n", cache_total_accesses);
	printf("hits: %d\n", cache_hits);
	printf("misses: %d\n", cache_misses);
	printf("total reads: %d\n", cache_read_accesses);
	printf("read hits: %d\n", cache_read_hits);
	printf("total writes: %d\n", cache_write_accesses);
	printf("write hits: %d\n", cache_write_hits);
}

/*
 * Perform a read from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */

op_result_t read_from_cache(uint32_t pa)
{
    int emptyFlag = 0;
    int cache_line;
    int cache_lines;
    uint32_t cached_tag;
    uint32_t tag_pa;
    uint32_t index;

    uint32_t index_mask = (1 << index_size) - 1;

    cache_total_accesses++;
    cache_read_accesses++;


    switch(cache_associativity){

        case 1:
            cache_lines = 1;


            emptyFlag = 1;
            cache_line = 0;
            break;

        case 2:
            cache_lines = block;
            break;

            // 2-way set associative
        case 3:
            cache_lines = 2;
            break;

            // 4-way set associative
        case 4:
            cache_lines = 4;
            break;
    }


    if(cache_associativity == 2) {
        index = 0;
    } else {
        index = (pa >> offset_size) & index_mask;
    }

    tag_pa = pa >> (offset_size + index_size);


    for(int i = 0; i < cache_lines; i++) {
        cached_tag = cache[index][i].tag >> (offset_size + index_size);
        if (cache[index][i].valid == 1 && tag_pa == cached_tag) {
            cache_hits++;
            cache_read_hits++;

            //TODO: Return Data from Cache

            return HIT;
        }
    }


    cache_misses++;


    for(int i = 0; i < cache_lines; i++) {
        if(cache[index][i].valid == 0) {
            emptyFlag = 1;
            cache_line = i;
            break;
        }
    }


    if(emptyFlag == 0) {
        //TODO in Task 1: Choose correct block to evict using LRU, save which cache line to evict in var `cache_line`

        // If block is not empty, evict it
        if(cache[index][cache_line].valid == 1) {

            // If Dirty bit is set, save block to main memory first before eviction
            if(cache[index][cache_line].dirty == 1) {

                //TODO: Save the block to main memory

                cache[index][cache_line].dirty = 0;
            }
        }
    }

    // Update the tag+index+offset combo in cache on a MISS
    cache[index][cache_line].tag = pa;
    cache[index][cache_line].valid = 1;

    //TODO: Fetch the block from main memory

    return MISS;
}

/*
 * Perform a write from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */
op_result_t write_to_cache(uint32_t pa)
{
    int emptyFlag = 0;
    int cache_line;
    uint32_t cached_tag;
    uint32_t tag_pa;
    uint32_t index;

    uint32_t index_mask = (1 << index_size) - 1;

    cache_total_accesses++;
    cache_write_accesses++;


    if(cache_associativity == 1){
        emptyFlag = 1;
        cache_line = 0;
    }

    if(cache_associativity == 2) { // Fully associative
        index = 0;
    } else {
        index = (pa >> offset_size) & index_mask;
    }

    tag_pa = pa >> (offset_size + index_size);

    // Cache Write HIT
    for(int i = 0; i < cache_lines; i++) {
        cached_tag = cache[index][i].tag >> (offset_size + index_size);
        if (cache[index][i].valid == 1 && tag_pa == cached_tag) {
            cache_hits++;
            cache_write_hits++;

            cache[index][i].dirty = 1;



            return HIT;
        }
    }


    cache_misses++;


    for(int i = 0; i < cache_lines; i++) {
        if(cache[index][i].valid == 0) {
            emptyFlag = 1;
            cache_line = i;
            break;
        }
    }

    if(emptyFlag == 0) {


        if(cache[index][cache_line].valid == 1) {
            if(cache[index][cache_line].dirty == 1) {

            }
        }
    }

    // Update cache with the new block
    cache[index][cache_line].tag = pa;
    cache[index][cache_line].valid = 1;
    cache[index][cache_line].dirty = 1;



    return MISS;
}

// Process the S parameter properly and initialize `cache_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_S(int opt, char *optarg)
{

    if (optarg == NULL || *optarg == '\0') {
        return -1;
    }


    for (char *digit = optarg; *digit != '\0'; digit++) {
        if (*digit < '0' || *digit > '9') {
            return -1;
        }
    }

    // Convert cache_size to unsigned long
    char *endptr;
    unsigned long S = strtoul(optarg, &endptr, 10);


    if (*endptr != '\0') {
        return -1;
    }


    if (S > UINT32_MAX) {
        return -1;
    }


    if (S == 0) {
        return -1;
    }


    cache_size = (uint32_t) S;
    return 0;
}

// Process the A parameter properly and initialize `cache_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_A(int opt, char *optarg)
{
    if (optarg == NULL || *optarg == '\0') {
        return -1;
    }


    for (char *digit = optarg; *digit != '\0'; digit++) {
        if (*digit < '0' || *digit > '9') {
            return -1;
        }
    }


    char *endptr;
    unsigned long A = strtoul(optarg, &endptr, 10);


    if (*endptr != '\0') {
        return -1;
    }


    if (A == 1 || A == 2 || A == 3 || A == 4) {
        cache_associativity = (uint32_t) A;
        return 0;
    }

    else {
        return -1;
    }
}

// Process the B parameter properly and initialize `cache_block_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_B(int opt, char *optarg)
{
    if (optarg == NULL || *optarg == '\0') {
        return -1;
    }


    for (char *digit = optarg; *digit != '\0'; digit++) {
        if (*digit < '0' || *digit > '9') {
            return -1;
        }
    }


    char *endptr;
    unsigned long B = strtoul(optarg, &endptr, 10);


    if (*endptr != '\0') {
        return -1;
    }


    if (B > UINT32_MAX) {
        return -1;
    }


    if (B == 0) {
        return -1;
    }


    if (B % 4 != 0) {
        return -1;
    }


    cache_block_size = (uint32_t) B;
    return 0;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
    if (ret == ERROR) {
        printf("This message should not be printed. Fix your code\n");
    }
    else {
        // Print Read or Write Operation
        if (entry.accesstype == READ) {
            printf("R ");
        }
        else {
            printf("W ");
        }

        // Print Address
        printf("0x%08X CACHE-", entry.address);

        //Print Hit or Miss
        if(ret == HIT) {
            printf("HIT\n");
        }
        else {
            printf("MISS\n");
        }
    }
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid()
{
    // cache_size must be able to divide cache_block_size
    if (cache_size % cache_block_size != 0) {
        return -1;
    }

    // cache_size must be greater than or equal to cache_block_size
    if (cache_size < cache_block_size) {
        return -1;
    }

    // Initialise number of cache lines
    cache_line = cache_size / cache_block_size;

    // Initialise number of sets
    switch(cache_associativity) {
        // When direct mapped
        case 1:
            set = cache_line;
            blocks = 1;
            break;

            // When fully associative
        case 2:
            set = 1;
            blocks = cache_line;
            break;

            // When 2-way set associative
        case 3:
            if(cache_line % 2 == 0) {
                set = cache_line / 2;
                blocks = 2;
            }
            else {
                return -1;
            }
            break;

            // When 4-way set associative
        case 4:
            if(cache_line % 4 == 0) {
                set = cache_line / 4;
                blocks = 4;
            }
            else {
                return -1;
            }
            break;
    }

    // Initialise offset_size. 32 bit address and ensure min of 1 bit
    // for tag and index
    offset_size = (uint32_t) (ceil(log2(cache_block_size)));
    if(offset_size >= 30) {
        return -1;
    }


    if(cache_associativity == 2) {
        index_size = 0;
    }
    else{
        index_size = (uint32_t) (ceil(log2(set)));
    }
    if(index_size >= 30) {
        return -1;
    }


    tag_size = 32 - offset_size - index_size;

    if(tag_size <= 0) {
        return -1;
    }

    return 0;
}
