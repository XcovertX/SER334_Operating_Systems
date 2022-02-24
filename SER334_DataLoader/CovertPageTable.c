/**
 * Simulates Page Tables
 *
 * completion time 1hr
 * @author James Covert
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "PageTable.h"

struct page_table_entry {
    unsigned int dirty_valid;
    int frame_number;
    int page_number;
    int order;
    int last;
    int freq;
    struct page_table_entry *next;
};

struct page_table {
    enum replacement_algorithm mode;
    int page_fault_count;
    int page_count;
    int frame_count;
    int verbose;
    struct page_table_entry *pte_head;
};

/**
 * Prints a given table
 *
 * @param page_table to print
 */
char* get_mode(struct page_table* p) {
    if (p->mode == FIFO) {
        return "FIFO";
    } else if (p->mode == LRU) {
        return "LRU";
    } else if (p->mode == MFU) {
        return "MFU";
    }
    return "unknown";
}

/**
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of frames.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table *pt = (struct page_table*)malloc(sizeof(struct page_table));
    pt->pte_head = (struct page_table_entry*)malloc(sizeof(struct page_table_entry*));
    pt->page_count = page_count;
    pt->frame_count = frame_count;
    pt->mode = algorithm;
    pt->verbose = verbose;
    return pt;
}

/**
 * Destorys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt) {
    struct page_table* pt_temp = *pt;
    struct page_table_entry* pte = pt_temp->pte_head;
    struct page_table_entry* temp;
    while (pte != NULL) {
        temp = pte->next;
        free(pte);
        pte = temp;
    }
    free(pt_temp->pte_head);
    free(pt_temp);
    pt = NULL;
}

/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page) {
    
}

/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt);

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt){
    printf("====Page Table====\n");
    printf("Mode: %s\n", get_mode(pt));
    printf("Page Faults: %d\n", pt->page_fault_count);
    printf("page frame | dirty valid\n");
    struct page_table_entry* pte = pt->pte_head;
    for(int i = 0; i < pt->page_count; i++) {
        printf("   %d     %d |      %d      %d\n",
               pte->page_number, pte->frame_number,
               0, pte->dirty_valid);
        pte = pte->next;
    }
}
