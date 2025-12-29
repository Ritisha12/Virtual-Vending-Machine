#ifndef PURCHASE_H
#define PURCHASE_H

// Item structure
typedef struct {
    char id[10];
    char category[20];
    char name[50];
    float price;
    int quantity;
} Item;


int loading_inventory(const char *filename, Item items[], int max_items);
int buy_item(Item items[], int count, const char *item_id, int *purchasedIndex);
void save_inventory(const char *filename, Item items[], int count);
void display_transaction(const Item *item);
void print_receipt(const Item *item, float userPayment, float change, float tax);
#endif
