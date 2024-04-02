#include "pt.h"
#include <stdlib.h>
#include "tlb.h"
#include "types.h"
#include "ll.h"

page_t *free_page_list;
page_t *used_page_list;
pt_entry_t *page_table;

// Page table statistics counters.
uint32_t page_table_total_accesses;
uint32_t page_table_faults;
uint32_t page_table_faults_with_dirty_page;

// New constants
uint32_t PAGE_TABLE_SIZE = (1 << 14); // 2^14
uint32_t OFFSET_BITS = 12;

void initialize_pt_system()
{
    // free page list is being initialized
    init_free_page_list(&free_page_list);
    init_pt();
    return;
}

/*
 * Initialize the "page_table"
 * The "page_table" is declared in as extern in include/pt.h file.
 */
void init_pt()
{
    page_table = malloc(PAGE_TABLE_SIZE * sizeof(pt_entry_t));
    for (uint32_t i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        page_table[i].PPN = -1;
        page_table[i].dirty = 0;
        page_table[i].present = 0;
    }

    page_table_total_accesses = 0;
    page_table_faults = 0;
    page_table_faults_with_dirty_page = 0;

    return;
}

void free_pt()
{
    // free page_table memory
    free(page_table);

    // free free_page_list memory
    page_t *n = free_page_list;
    while (n)
    {
        n = n->next;
        free(free_page_list);
        free_page_list = n;
    }

    // free used_page_list memory
    n = used_page_list;
    while (n)
    {
        n = n->next;
        free(used_page_list);
        used_page_list = n;
    }
}

void init_free_page_list(page_t **free_page_list)
{
    for (int i = 255; i >= 0; i--)
    {
        page_t *new_page = (page_t *)malloc(sizeof(page_t));
        new_page->ppn = i;
        new_page->next = NULL;
        insert_in_ll(free_page_list, new_page);
    }
}

// Extract the VPN from the address and use it.
int check_page_table(uint32_t address)
{
    // return -1 if the entry is missing or present bit is 0 aka page fault
    // return PPN if the page is hit

    page_table_total_accesses++;
    uint32_t vpn = address >> OFFSET_BITS;
    if (page_table[vpn].present)
    {
        uint32_t PPN = page_table[vpn].PPN;

        // update lru
        page_t *current_page = used_page_list;
        if (current_page->ppn != PPN)
        {
            while (current_page->next)
            {
                if (current_page->next->ppn == PPN)
                {
                    break;
                }
                current_page = current_page->next;
            }
            page_t *update_page = current_page->next;
            current_page->next = current_page->next->next;
            insert_in_ll(&used_page_list, update_page);
        }

        return PPN;
    }
    else
    {
        page_table_faults++;
        return -1;
    }
}

// Extract the VPN from the address and use it.
void update_page_table(uint32_t address, uint32_t PPN)
{
    // set PPN for VPN in page table entry
    // set present bit in page table entry

    uint32_t vpn = address >> OFFSET_BITS;
    page_table[vpn].PPN = PPN;
    page_table[vpn].present = 1;

    page_t *current_page = used_page_list;
    if (current_page->ppn == PPN)
    {
        current_page->page_table_entry = &page_table[vpn];
    }
    else
    {
        while (current_page->next)
        {
            if (current_page->next->ppn == PPN)
            {
                current_page->next->page_table_entry = &page_table[vpn];
                break;
            }
            current_page = current_page->next;
        }
        page_t *update_page = current_page->next;
        current_page->next = current_page->next->next;
        insert_in_ll(&used_page_list, update_page);
    }

    insert_or_update_tlb_entry(address, PPN);
}

// set the dirty bit of the entry to 1
void set_dirty_bit_in_page_table(uint32_t address)
{
    uint32_t vpn = address >> OFFSET_BITS;
    page_table[vpn].dirty = 1;
}

// LRU is to be use to find the victim page
page_t *get_victim_page()
{
    // you may use the used_page_list to find the victim page
    // return the victim page

    page_t *blru_page = used_page_list;
    while (blru_page->next)
    {
        if (blru_page->next->next == NULL)
        {
            break;
        }
        blru_page = blru_page->next;
    }
    page_t *victim_page = blru_page->next;
    if (victim_page->page_table_entry->dirty == 1)
    {
        page_table_faults_with_dirty_page++;
    }
    victim_page->page_table_entry->present = 0;

    blru_page->next = blru_page->next->next;
    insert_in_ll(&used_page_list, victim_page);

    return victim_page;
}

// pops a page from the free page linked-list
page_t *get_free_page()
{
    if (free_page_list == NULL)
    {
        return NULL;
    }
    else
    {
        page_t *will_use = delete_from_top_of_ll(&free_page_list);
        insert_in_ll(&used_page_list, will_use);
        return will_use;
    }
    return 0;
}

// print pt entries as per the spec
void print_pt_entries()
{
    // printf("\nPage Table Entries (Present-Bit Dirty-Bit PPN)\n");
    printf("\nPage Table Entries (Present-Bit Dirty-Bit VPN PPN)\n");

    for (uint32_t i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        if (page_table[i].present == 1)
        {
            printf("%d %d 0x%05x 0x%05x\n", page_table[i].present, page_table[i].dirty, i, page_table[i].PPN);
        }
    }
}

// print pt statistics as per the spec
void print_pt_statistics()
{
    printf("\n* Page Table Statistics *\n");
    printf("total accesses: %d\n", page_table_total_accesses);
    printf("page faults: %d\n", page_table_faults);
    printf("page faults with dirty bit: %d\n", page_table_faults_with_dirty_page);
}
