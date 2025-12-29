#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventory.h"
#include "payment.h"
#include "purchase.h"

int main(int argc, char *argv[]) {

    if (argc < 2) { 
        usage(); 
        return 1; 
    } 
    
    char categoryChar = argv[1][0]; // first character of argument

    float Price;
    float Change;

    FILE *file = fopen("inventory.csv", "r");
    if (!file) {
	    file = fopen("inventory.csv", "w+"); // create if missing
    }
    if (!file) {
	    perror("Failed to open or create inventory.csv");
	    return -1;
    }

    if (argc == 2) {

        if (strcmp(argv[1], "--help") == 0) {
            help();
            exit(0);
        }    
    } 

    if (!file) {
        printf("Failed to open inventory.csv\n");
        return 1;
    }
    
    // allocate arrays dynamically
    char *id[maximumItems];
    char *category[maximumItems];
    char *name[maximumItems];
    float price[maximumItems];
    int quantity[maximumItems];

    for (int i = 0; i < maximumItems; i++) {
        id[i] = malloc(idLength);
        category[i] = malloc(categoryLength);
        name[i] = malloc(nameLength);
    }

    int itemsRead = read_inventory(file, id, category, name, price, quantity);
    fclose(file);

    if (argc == 2) {
        printCategory(itemsRead, id, category, name, price, quantity, argv[1]);
    }

    // ritisha
    Item items[100]; // hold upto a 100 inventory entries
    int count = loading_inventory("inventory.csv", items, 100);
    if (count == 0) {
        printf("Inventory is empty or failed to load.\n"); // error message outputted
        for (int i = 0; i < itemsRead; i++) {
            free(id[i]);
            free(category[i]);
            free(name[i]);
        }
        return 1;
    }

    //buffer to hold user input
    char item_id[10];
    printf("Enter item ID to purchase: "); // asking user input
    scanf("%9s", item_id);

    char *useritem = item_id;

    int purchasedIndex;
    // buy the item with user entered id
    int result = buy_item(items, count, item_id, &purchasedIndex);

    // if successful purchase, update inventory
    if (result == 0) {
        // claira
        Price = items[purchasedIndex].price;
        printf("Your total is $%.2f\n", Price);

        float useramount;
        Change = getchange(Price, &useramount);
        printf("Your change is %.2f\n", Change);
        printf("Amount user entered %.2f\n", useramount);

        float tax = Price * 0.13;

        save_inventory("inventory.csv", items, count);
        print_receipt(&items[purchasedIndex], useramount, Change, tax);
    }

    else if (result == -1) {
        printf("Purchase failed.\n");  // out of stock
    }

    else if (result == -2) {
        printf("Purchase failed.\n");  // invalid ID
    }

   
    // free memory
    for (int i = 0; i < itemsRead; i++) {
        free(id[i]);
        free(category[i]);
        free(name[i]);
    }

    return 0;
}
