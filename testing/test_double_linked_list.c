#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

// define a struct that contains the dl_list

struct data {
    int value;
    struct dl_list list;    // the list head for linking
};

// function to print all elems in the list
void print_list(struct dl_list* head) {
    struct dl_list* pos;
    printf("List contents: ");
    list_for_each(pos, head) {
        struct data* entry = list_entry(pos, struct data, list);
        printf("%d ", entry->value);
    }
    printf("\n");
}

// Function to safely delete all elements in the list
void clear_list(struct dl_list *head) {
    struct dl_list *pos, *temp;
    list_for_each_safe(pos, temp, head) {
        struct data *entry = list_entry(pos, struct data, list);
        list_del(pos);  // Remove from list
        free(entry);    // Free memory
    }
}

int main() {
    // Step 1: Initialize the list head
    DL_LIST_HEAD(my_list);
    printf("Initialized list.\n");

    // Step 2: Add elements to the list
    struct data *item1 = malloc(sizeof(struct data));
    item1->value = 10;
    list_add(&item1->list, &my_list);  // Add to the head

    struct data *item2 = malloc(sizeof(struct data));
    item2->value = 20;
    list_add_tail(&item2->list, &my_list);  // Add to the tail

    struct data *item3 = malloc(sizeof(struct data));
    item3->value = 30;
    list_add_tail(&item3->list, &my_list);  // Add to the tail

    print_list(&my_list);  // Step 3: Print the list

    // Step 4: Test list_first_entry
    struct data *first = list_first_entry(&my_list, struct data, list);
    printf("First element value: %d\n", first->value);

    // Step 5: Test safe traversal and deletion
    printf("Deleting all elements:\n");
    clear_list(&my_list);

    // Step 6: Check if the list is empty
    if (list_empty(&my_list)) {
        printf("List is now empty.\n");
    } else {
        printf("List is not empty.\n");
    }

    return 0;
}


