#include "tlb.h"
#include <stdlib.h>

// Input parameters to control the tlb.
uint32_t tlb_entries;
uint32_t tlb_associativity;

// TLB statistics counters.
uint32_t tlb_total_accesses;
uint32_t tlb_hits;
uint32_t tlb_misses;

tlb_entry_t **tlb;

// New fileds

// tlb metadata
uint32_t tlb_set_num;
uint32_t tlb_entries_per_set;

// for LRU replacement policy
void tlb_lru_update(uint32_t set_index, uint32_t entry_index)
{
    uint8_t current_dirty = tlb[set_index][entry_index].dirty;
    uint32_t current_VPN = tlb[set_index][entry_index].VPN;
    uint32_t current_PPN = tlb[set_index][entry_index].PPN;

    int j;
    for (j = entry_index + 1; j < tlb_entries_per_set; j++)
    {
        if (tlb[set_index][j].valid == 1)
        {
            tlb[set_index][j - 1].dirty = tlb[set_index][j].dirty;
            tlb[set_index][j - 1].VPN = tlb[set_index][j].VPN;
            tlb[set_index][j - 1].PPN = tlb[set_index][j].PPN;
        }
        else
        {
            break;
        }
    }
    tlb[set_index][j - 1].dirty = current_dirty;
    tlb[set_index][j - 1].VPN = current_VPN;
    tlb[set_index][j - 1].PPN = current_PPN;
}

/* outdate method for lru
uint32_t **tlb_counter;
// similar to lru_replacement() in cache.c
uint32_t lru_tlb(uint32_t tlb_set_index)
{
    uint32_t lru_tlb_index = 0;
    uint32_t current_counter = tlb_counter[tlb_set_index][0];

    for (int i = 1; i < tlb_entries_per_set; i++)
    {
        if (tlb_counter[tlb_set_index][i] < current_counter)
        {
            current_counter = tlb_counter[tlb_set_index][i];
            lru_tlb_index = i;
        }
    }

    return lru_tlb_index;
}
*/

// Check if all the necessary paramaters for the tlb are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_tlb_parameters_valid()
{
    // tlb_entries and tlb_associativity must be initialised
    if (tlb_entries == 0 || tlb_associativity == 0)
    {
        return -1;
    }

    // Check TLB entries suitble for associtivity
    if (tlb_associativity == 3 && tlb_entries % 2 != 0)
    {
        return -1;
    }
    if (tlb_associativity == 4 && tlb_entries % 4 != 0)
    {
        return -1;
    }

    return 0;
}

/*
 * Initialize the "tlb" depending on the input parameters T and L.
 * The "tlb" is declared in as extern in include/tlb.h file.
 */
void initialize_tlb()
{
    switch (tlb_associativity)
    {
    // Direct mapped
    case 1:
        tlb_set_num = tlb_entries;
        tlb_entries_per_set = 1;
        break;

    // Fully associative
    case 2:
        tlb_set_num = 1;
        tlb_entries_per_set = tlb_entries;
        break;

    // 2-set associative
    case 3:
        tlb_set_num = tlb_entries / 2;
        tlb_entries_per_set = 2;
        break;

    // 4-set associative
    case 4:
        tlb_set_num = tlb_entries / 4;
        tlb_entries_per_set = 4;
        break;
    }

    // Allocate memory for TLB
    tlb = malloc(tlb_set_num * sizeof(tlb_entry_t *));
    for (uint32_t i = 0; i < tlb_set_num; i++)
    {
        tlb[i] = malloc(tlb_entries_per_set * sizeof(tlb_entry_t));
        // Initialize for each tlb entry
        for (uint32_t j = 0; j < tlb_entries_per_set; j++)
        {
            tlb[i][j].valid = 0;
            tlb[i][j].dirty = 0;
            tlb[i][j].VPN = -1;
            tlb[i][j].PPN = -1;
        }
    }

    /* outdate LRU method
    // allocate cache_counter memory
    tlb_counter = malloc(tlb_set_num * sizeof(uint32_t *));
    for (uint32_t i = 0; i < tlb_set_num; i++)
    {
        tlb_counter[i] = malloc(tlb_entries_per_set * sizeof(uint32_t));
        for (uint32_t j = 0; j < tlb_entries_per_set; j++)
        {
            tlb_counter[i][j] = 0;
        }
    }
    */

    // initialised tlb statistics counters
    tlb_total_accesses = 0;
    tlb_hits = 0;
    tlb_misses = 0;
}

void free_tlb()
{
    // free tlb memory
    for (uint32_t i = 0; i < tlb_set_num; i++)
    {
        free(tlb[i]);
    }
    free(tlb);

    /* outdate LRU method
    // free tlb_counter memory
    for (uint32_t i = 0; i < tlb_set_num; i++)
    {
        free(tlb_counter[i]);
    }
    free(tlb_counter);
    */
}

// Process the T parameter properly and initialize `tlb_entries`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_T(int opt, char *optarg)
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

    // tlb_entries should be in range 2-UINT32_MAX
    if (value > UINT32_MAX || value < 2)
    {
        return -1;
    }

    // check whether the number is a power of 2
    if ((value & (value - 1)) != 0)
    {
        return -1;
    }

    tlb_entries = (uint32_t)value;
    return 0;
}

// Process the A parameter properly and initialize `tlb_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_L(int opt, char *optarg)
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
        tlb_associativity = (uint32_t)value;
        return 0;
    }
    else
    {
        return -1;
    }
}

// Check if the tlb hit or miss.
// Extract the VPN from the address and use it.
// Keep associativity in mind while searching.
int check_tlb(uint32_t address)
{
    // return -1 if the entry is missing or valid bit is 0 aka tlb miss
    // return PPN if the entry is valid and TAG matches aka tlb hit

    tlb_total_accesses++;

    // Extract the VPN
    uint32_t VPN = address >> 12;

    // determine index depend on different associativity
    uint32_t index;
    switch (tlb_associativity)
    {
    // directed
    case 1:
        index = VPN % tlb_entries;
        break;

    // full
    case 2:
        index = 0;
        break;

    // two-way
    case 3:
        index = VPN % (tlb_entries / 2);
        break;

    // 4-way
    case 4:
        index = VPN % (tlb_entries / 4);
        break;
    }

    // when tlb hit
    for (int i = 0; i < tlb_entries_per_set; i++)
    {
        if (tlb[index][i].valid == 1 && tlb[index][i].VPN == VPN)
        {
            // update lru
            uint32_t PPN = tlb[index][i].PPN;

            tlb_hits++;
            // tlb_counter[index][i] = tlb_total_accesses;
            tlb_lru_update(index, i);
            return PPN;
        }
    }

    // else, when tlb miss
    tlb_misses++;
    return -1;
}

void set_dirty_bit_in_tlb(uint32_t address)
{
    // set the dirty bit of the entry to 1

    // Extract the VPN
    uint32_t VPN = address >> 12;

    // determine index depend on different associativity
    uint32_t index;
    switch (tlb_associativity)
    {
    // directed
    case 1:
        index = VPN % tlb_entries;
        break;

    // full
    case 2:
        index = 0;
        break;

    // two-way
    case 3:
        index = VPN % (tlb_entries / 2);
        break;

    // 4-way
    case 4:
        index = VPN % (tlb_entries / 4);
        break;
    }

    // set the dirty bit
    for (int i = 0; i < tlb_entries_per_set; i++)
    {
        if (tlb[index][i].valid == 1 && tlb[index][i].VPN == VPN)
        {
            tlb[index][i].dirty = 1;
        }
    }
}

// LRU replacement policy has to be implemented.
void insert_or_update_tlb_entry(uint32_t address, uint32_t PPN)
{
    // if the entry is free, insert the entry
    // if the entry is not free, identify the victim entry and replace it
    // set PPN for VPN in tlb
    // set valid bit in tlb

    // Extract the VPN
    uint32_t VPN = address >> 12;

    // determine index depend on different associativity
    uint32_t index;
    switch (tlb_associativity)
    {
    // directed
    case 1:
        index = VPN % tlb_entries;
        break;

    // full
    case 2:
        index = 0;
        break;

    // two-way
    case 3:
        index = VPN % (tlb_entries / 2);
        break;

    // 4-way
    case 4:
        index = VPN % (tlb_entries / 4);
        break;
    }

    // the index inside the set of index
    uint32_t lru_tlb_index = -1;
    // find if there is free block in that index
    for (int i = 0; i < tlb_entries_per_set; i++)
    {
        // if there is a free block, set lru_tlb_index
        if (tlb[index][i].valid == 0)
        {
            lru_tlb_index = i;
            break;
        }
    }
    // when there isnot a free block
    if (lru_tlb_index == -1)
    {
        // evict first entry(the LRU entry)
        // check if dirty, write page to memory
        if (tlb[index][0].dirty == 1)
        {
            // tlb_write_to_memory(PPN << 12);
            tlb[index][0].dirty = 0;
        }

        tlb_lru_update(index, 0);

        // set lru_tlb_index with last entry
        lru_tlb_index = tlb_entries_per_set - 1;
    }

    tlb[index][lru_tlb_index].VPN = VPN;
    tlb[index][lru_tlb_index].PPN = PPN;
    tlb[index][lru_tlb_index].valid = 1;
    // tlb_counter[index][lru_tlb_index] = tlb_total_accesses;
}

// print pt entries as per the spec
void print_tlb_entries()
{
    // print the tlb entries
    printf("\nTLB Entries (Valid-Bit Dirty-Bit VPN PPN)\n");
    for (uint32_t i = 0; i < tlb_set_num; i++)
    {
        for (uint32_t j = 0; j < tlb_entries_per_set; j++)
        {
            if (tlb[i][j].valid == 1)
            {
                printf("%d %d 0x%05x 0x%05x\n", tlb[i][j].valid, tlb[i][j].dirty, tlb[i][j].VPN, tlb[i][j].PPN);
            }
            else
            {
                printf("%d %d - -\n", tlb[i][j].valid, tlb[i][j].dirty);
            }
        }
    }
}

// print tlb statistics as per the spec
void print_tlb_statistics()
{
    // print the tlb statistics
    printf("\n* TLB Statistics *\n");
    printf("total accesses: %d\n", tlb_total_accesses);
    printf("hits: %d\n", tlb_hits);
    printf("misses: %d\n", tlb_misses);
}
