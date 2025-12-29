/*
 * File: purchase.c
 * Description: Implements inventory/purchase helper functions used by the
 *              vending machine application (load/save inventory, buy items,
 *              display receipts and transactions).
 * AUTHOR: Ritisha Perumalla
 * DATE: 2025-12-01
 * VERSION: v1.0.1
 */

#include <stdio.h>
#include <strings.h>
#include "purchase.h"

/* Loads inventory from a CSV file into the provided array.
 * filename: path to CSV file to read
 * items: output array to populate with loaded items
 * returns: number of items loaded on success, 0 if the file couldn't be opened
 */
int loading_inventory(const char *filename, Item items[], int max_items) {
    FILE *fp = fopen(filename, "r"); // opens csv file to read
    if (fp == NULL) {
        fprintf(stderr,"Inventory file can't be opened"); // prints error if file can't be opened
        return 0;
    }

    // Skip the header line, only keep if the first line isn't removed
    char header[100];
    fgets(header, sizeof(header), fp);

    int count = 0;

    //reads csv into the items array
    while (count < max_items && fscanf(fp, "%9[^,],%19[^,],%49[^,],%f,%d\n",items[count].id, items[count].category, items[count].name, &items[count].price, &items[count].quantity) == 5) 
    {
        count++; // increase count for successful read item
    }

    fclose(fp);
    return count;
}

/* Searches for an item by item_id and attempts to purchase one unit.
 * items: array of item entries
 * count: number of valid entries in items
 * returns: 0 on successful purchase, -1 if out of stock, -2 if item not found
 * side effects: decrements quantity on success and prints messages/receipt to stdout.
 */
int buy_item(Item items[], int count, const char *item_id, int *purchasedIndex) {
    for (int i = 0; i < count; i++) {
        // comparing current and user entered id
        if (strcasecmp(items[i].id, item_id) == 0) {
            if (items[i].quantity <= 0) {
                printf(" %s is out of stock.\n", items[i].name); // out of stock
                return -1;
            }
            // reduces the item
            items[i].quantity--;
            *purchasedIndex = i;
            return 0;
        }
    }
    // if user enter's invalid ID
    printf(" Item with ID %s was not found.\n", item_id);
    return -2; 
}

/* Writes the current items array back to a CSV file (overwrites file)
 * filename: path to CSV file to write
 * items: array of item entries to write
 * count: number of items to write from the array
 */
void save_inventory(const char *filename, Item items[], int count) {
    FILE *fp = fopen(filename, "w"); // open file to write / override
    if (fp == NULL) {
        fprintf(stderr,"Failed to open inventory file for writing");
        return;
    }

     // Write the header line first (Maybe modify if i remve first line)
    fprintf(fp, "ID,Category,Name,Price,Quantity\n");


    // write back into the csv file with updated info
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%.2f,%d\n",items[i].id,items[i].category,items[i].name,items[i].price,items[i].quantity);
    }


    fclose(fp);
}


/* Prints a short transaction verification for item 
 * item: pointer to the item to display
 */
void display_transaction(const Item *item) {
    printf("\nVerify Transaction\n");
    printf("-----------\n");
    printf("Item:      %s\n", item->name);      
    printf("ID:        %s\n", item->id);
    printf("Price:     $%.2f\n", item->price);
    printf("-----------\n\n");
}

/* Prints a receipt for the purchased item to the console
 * item: pointer to the item to display
 */
void print_receipt(const Item *item, float userPayment, float change, float tax) {
    printf("\n                   Receipt                      \n");
    printf("--------------------------------------------------\n");
    printf("%-15s: %s\n", "Item", item->name);      
    printf("%-15s: %s\n", "ID", item->id);
    printf("%-15s: $%.2f\n", "Price", item->price);
    printf("%-15s: $%.2f\n", "Tax", tax); // replace with calculated tax
    printf("%-15s: $%.2f\n", "Total", item->price + tax); // replace with calculated tax
    printf("%-15s: $%.2f\n", "User Inputted", userPayment); // replace price with user inputted amount
    printf("%-15s: $%.2f\n", "Change", change); // replace with calculated change
    printf("%-15s: %s\n", "Status", "Purchase was successful!"); 
    printf("--------------------------------------------------\n\n");
}