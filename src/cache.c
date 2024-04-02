#include "cache.h"
#include <stdlib.h>

// Cache statistics counters.
uint32_t cache_total_accesses;
uint32_t cache_hits;
uint32_t cache_misses;
uint32_t cache_read_accesses;
uint32_t cache_read_hits;
uint32_t cache_write_accesses;
uint32_t cache_write_hits;

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
	return 0;
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

// New fields

// current cache metadata
uint32_t number_sets;
uint32_t block_per_set;
uint32_t index_size;
uint32_t offset_size;

// for LRU replacement policy
uint32_t **cache_counter;

/*
 * Initialize the cache depending on the input parameters S, A, and B 
 * and the statistics counter. The cache is declared in as extern in 
 * include/cache.h file.
 */
void initialize_cache()
{
	// initialise the number of sets, the number of blocks per set
	// depend on different cache_associativity
	if (cache_associativity == 1)
	{
		block_per_set = 1;
		number_sets = cache_size / cache_block_size;
	}
	else if (cache_associativity == 2)
	{
		block_per_set = cache_size / cache_block_size;
		number_sets = 1;
	}
	else if (cache_associativity == 3)
	{
		block_per_set = 2;
		number_sets = cache_size / (cache_block_size * 2);
	}
	else if (cache_associativity == 4)
	{
		block_per_set = 4;
		number_sets = cache_size / (cache_block_size * 4);
	}

	// Initialise offset_size
	double i;
	offset_size = 0;
	for (i = cache_block_size; i > 1; i = i / 2)
	{
		offset_size++;
	}
	// offset_size = (uint32_t)(ceil(log2(cache_block_size)));

	// Initialise index_size
	if (cache_associativity == 2)
	{
		index_size = 0;
	}
	else
	{
		index_size = 0;
		double i;
		for (i = number_sets; i > 1; i = i / 2)
		{
			index_size++;
		}
	}

	// allocate cache memory
	cache = malloc(number_sets * sizeof(block_t *));
	for (uint32_t i = 0; i < number_sets; i++)
	{
		cache[i] = malloc(block_per_set * sizeof(block_t));
		for (uint32_t j = 0; j < block_per_set; j++)
		{
			cache[i][j].valid = 0;
			cache[i][j].dirty = 0;
			cache[i][j].tag = 0;
		}
	}

	// allocate cache_counter memory
	cache_counter = malloc(number_sets * sizeof(uint32_t *));
	for (uint32_t i = 0; i < number_sets; i++)
	{
		cache_counter[i] = malloc(block_per_set * sizeof(uint32_t));
		for (uint32_t j = 0; j < block_per_set; j++)
		{
			cache_counter[i][j] = 0;
		}
	}

	// initialised Cache statistics counters
	cache_total_accesses = 0;
	cache_hits = 0;
	cache_misses = 0;
	cache_read_accesses = 0;
	cache_read_hits = 0;
	cache_write_accesses = 0;
	cache_write_hits = 0;
}

/*
 * Free the allocated memory for the cache to avoid memory leaks.
 */
void free_cache()
{
	// free cache memory
	for (uint32_t i = 0; i < number_sets; i++)
	{
		free(cache[i]);
	}
	free(cache);

	// free cache_counter memory
	for (uint32_t i = 0; i < number_sets; i++)
	{
		free(cache_counter[i]);
	}
	free(cache_counter);
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

/* Helper Function
 * check through the current index,
 * compare all the counter for each block,
 * the block with the least counter is Least Recently Used.
 * return the correspond index of the block.
 */
uint32_t lru_replacement(uint32_t index)
{
	uint32_t index_block = 0;
	uint32_t current_counter = cache_counter[index][0];

	for (int i = 1; i < block_per_set; i++)
	{
		if (cache_counter[index][i] < current_counter)
		{
			current_counter = cache_counter[index][i];
			index_block = i;
		}
	}

	return index_block;
}


/*
 * Perform a read from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */

op_result_t read_from_cache(uint32_t pa) 
{
	cache_total_accesses++;
	cache_read_accesses++;

	// determine index
	uint32_t index;
	uint32_t index_mask = (1 << index_size) - 1;
	index = (pa >> offset_size) & index_mask;

	// determine tag
	uint32_t tag;
	tag = pa >> (offset_size + index_size);

	// detect the tag in the index
	uint32_t tag_fetched;
	for (int i = 0; i < block_per_set; i++)
	{
		tag_fetched = cache[index][i].tag;
		if (cache[index][i].valid == 1 && tag == tag_fetched)
		{
			cache_hits++;
			cache_read_hits++;
			// READ DATA
			cache_counter[index][i] = cache_total_accesses;
			// printf("i: %d\n", i);
			return HIT;
		}
	}
	// If the block is present, it's done

	// Else, find if there is free block in that index
	for (int i = 0; i < block_per_set; i++)
	{
		// if there is a free block, read from memory, done
		if (cache[index][i].valid == 0)
		{
			read_from_memory(pa);
			cache[index][i].tag = tag;
			cache[index][i].valid = 1;
			// READ DATA
			cache_misses++;
			cache_counter[index][i] = cache_total_accesses;
			// printf("i: %d\n", i);
			return MISS;
		}
	}

	// if there is not a free block, use LRU replacement policy to find a place
	uint32_t index_block;
	index_block = lru_replacement(index);

	// If Dirty bit is set, save block to main memory first before eviction
	if (cache[index][index_block].dirty == 1)
	{
		uint32_t current_address;
		uint32_t current_tag = cache[index][index_block].tag;
		current_address = (current_tag << (offset_size + index_size)) + (index << offset_size);
		write_to_memory(current_address);
		cache[index][index_block].dirty = 0;
	}

	read_from_memory(pa);
	cache[index][index_block].tag = tag;
	// READ DATA
	cache_misses++;
	cache_counter[index][index_block] = cache_total_accesses;
	// printf("index_block: %d\n", index_block);
	return MISS;
}

/*
 * Perform a write from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */
op_result_t write_to_cache(uint32_t pa)
{
	cache_total_accesses++;
	cache_write_accesses++;

	// determine index
	uint32_t index;
	uint32_t index_mask = (1 << index_size) - 1;
	index = (pa >> offset_size) & index_mask;

	// determine tag
	uint32_t tag;
	tag = pa >> (offset_size + index_size);

	// detect the tag in the index
	uint32_t tag_fetched;
	for (int i = 0; i < block_per_set; i++)
	{
		tag_fetched = cache[index][i].tag;
		if (cache[index][i].valid == 1 && tag == tag_fetched)
		{
			cache_hits++;
			cache_write_hits++;
			// WRITE DATA
			cache[index][i].dirty = 1;
			cache_counter[index][i] = cache_total_accesses;
			// printf("i: %d\n", i);
			return HIT;
		}
	}
	// If the block is present, it's done

	// Else, find if there is free block in that index
	for (int i = 0; i < block_per_set; i++)
	{
		// if there is a free block, read from memory, done
		if (cache[index][i].valid == 0)
		{
			read_from_memory(pa);
			cache[index][i].tag = tag;
			cache[index][i].valid = 1;
			cache[index][i].dirty = 1;
			// WRITE DATA
			cache_misses++;
			cache_counter[index][i] = cache_total_accesses;
			// printf("i: %d\n", i);
			return MISS;
		}
	}

	// if there is not a free block, use LRU replacement policy to find a place
	uint32_t index_block;
	index_block = lru_replacement(index);

	// If Dirty bit is set, save block to main memory first before eviction
	if (cache[index][index_block].dirty == 1)
	{
		uint32_t current_address;
		uint32_t current_tag = cache[index][index_block].tag;
		current_address = (current_tag << (offset_size + index_size)) + (index << offset_size);
		write_to_memory(current_address);
		cache[index][index_block].dirty = 0;
	}

	read_from_memory(pa);
	cache[index][index_block].tag = tag;
	cache[index][index_block].dirty = 1;
	// WRITE DATA
	cache_misses++;
	cache_counter[index][index_block] = cache_total_accesses;
	// printf("index_block: %d\n", index_block);
	return MISS;
}

// Process the S parameter properly and initialize `cache_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_S(int opt, char *optarg)
{
	// There should be a value
	if (optarg == NULL)
	{
		return -1;
	}
	// The characters should only contain 0-9
	for (char *digit = optarg; *digit != '\0'; digit++)
	{
		if (*digit < '0' || *digit > '9')
		{
			return -1;
		}
	}
	// Convert optarg to unsigned long
	char *endptr;
	unsigned long value = strtoul(optarg, &endptr, 10);

	// cache_size should be in range 4-UINT32_MAX
	if (value > UINT32_MAX || value < 4)
	{
		return -1;
	}

	// check whether the number is a power of 2
	if ((value & (value - 1)) != 0)
	{
		return -1;
	}

	cache_size = (uint32_t)value;
	return 0;
}

// Process the A parameter properly and initialize `cache_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_A(int opt, char *optarg)
{
	// There should be a value
	if (optarg == NULL)
	{
		return -1;
	}
	// The characters should only contain 1-4
	for (char *digit = optarg; *digit != '\0'; digit++)
	{
		if (*digit < '1' || *digit > '4')
		{
			return -1;
		}
	}
	// Convert optarg to unsigned long
	char *endptr;
	unsigned long value = strtoul(optarg, &endptr, 10);

	if (value == 1 || value == 2 || value == 3 || value == 4)
	{
		cache_associativity = (uint32_t)value;
		return 0;
	}
	else
	{
		return -1;
	}
}

// Process the B parameter properly and initialize `cache_block_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_B(int opt, char *optarg)
{
	// There should be a value
	if (optarg == NULL)
	{
		return -1;
	}
	// The characters should only contain 0-9
	for (char *digit = optarg; *digit != '\0'; digit++)
	{
		if (*digit < '0' || *digit > '9')
		{
			return -1;
		}
	}
	// Convert optarg to unsigned long
	char *endptr;
	unsigned long value = strtoul(optarg, &endptr, 10);

	// cache_block_size should be in range 4-UINT32_MAX
	if (value > UINT32_MAX || value < 4)
	{
		return -1;
	}
	// cache_block_size should be a multiple of 4
	if (value % 4 != 0)
	{
		return -1;
	}

	// check whether the number is a power of 2
	if ((value & (value - 1)) != 0)
	{
		return -1;
	}

	cache_block_size = (uint32_t)value;
	return 0;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
	if (ret == ERROR) {
		printf("This message should not be printed. Fix your code\n");
	}
	else
	{
		// Print Read or Write Operation
		if (entry.accesstype == READ)
		{
			printf("R ");
		}
		else
		{
			printf("W ");
		}

		// Print Address
		printf("0x%08X ", entry.address);

		// Print Hit or Miss
		if (ret == HIT)
		{
			printf("CACHE-HIT\n");
		}
		else
		{
			printf("CACHE-MISS\n");
		}
	}
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid()
{
	// cache_size, cache_associativity, cache_block_size
	// must be initialised
	if (cache_size == 0 || cache_associativity == 0 || cache_block_size == 0)
	{
		return -1;
	}

	// cache_size must be greater than or equal to cache_block_size
	if (cache_size < cache_block_size)
	{
		return -1;
	}

	// cache_size must be divided by cache_block_size
	// depend on different cache_associativity
	if (cache_associativity == 1 || cache_associativity == 2)
	{
		if (cache_size % cache_block_size != 0)
		{
			return -1;
		}
	}
	else if (cache_associativity == 3)
	{
		if (cache_size % (cache_block_size * 2) != 0)
		{
			return -1;
		}
	}
	else if (cache_associativity == 4)
	{
		if (cache_size % (cache_block_size * 4) != 0)
		{
			return -1;
		}
	}

	return 0;
}
