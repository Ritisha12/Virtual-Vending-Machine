/* Team 13
 * Manasvi Bandi, Lakshmi Saranya Alamanda, Ritisha Perumalla, Claira Corkery
 * 400570531, 400560245, 400560957, 400593241
 * 2025-12-01
 * inventory.c
 * This file implements functions to manage a vending machine inventory like reading from the CSV file, storing item data, and displaying items by category.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

///////////////////////////
// Usage string which shows how the utility should be used
// Globals: None.
// Arguments: None.
// Outputs: Prints a usage message to stdout.
// Returns: None.
///////////////////////////
void usage() {
    printf("Usage: vendingmachine [CATEGORY (s|S|d||D|f|F)]\n");
}

void help() {
    printf("vendingmachine: get the category name\n\n");
    printf("Usage: vendingmachine [CATEGORY (s|S|d||D|f|F)]\n\n");

    printf("Options:\n");
    printf("vendingmachine [CATEGORY (s|S|d||D|f|F)]        The categories that the user can choose from.\n");
    printf("                                                s|S Snacks\n");
    printf("                                                d|D Drinks\n");
    printf("                                                f|F FirstAid\n");
    printf("--help                                          This displays this help message.\n\n");

    printf("Examples:\n");
    printf("./vendingmachine s                              Prints all the snack items available.\n");
    printf("./vendingmachine d                              Prints all the drink items available.\n");
    printf("./vendingmachine --help                         Prints this help function.\n");
}


///////////////////////////
// Reads inventory data from a CSV file into provided arrays.
// Globals: count (tracks number of items read so far) 
// Arguments: FILE *file (pointer to the opened CSV file), char *id[] (array of strings to store item IDs), char *category[] (array of strings to store item categories), char *name[] (array of strings to store item names), float price[] (array to store item prices), int quantity[] (array to store item quantities)
// Outputs: Arrays get assigned values from csv file.
// Returns: count (number of items successfully read)
///////////////////////////
int read_inventory(FILE *file, char *id[], char *category[], char *name[], float price[], int quantity[]) {
    int count = 0;

    // skip header line
    char headerLine[35];
    fgets(headerLine, 35, file);

    // allocating memory for each item in inventory array
    for (int i = 0; i < maximumItems; i++) {
        id[i] = malloc(idLength);
        category[i] = malloc(categoryLength);
        name[i] = malloc(nameLength);
    }

    // reading csv file and storing it into arrays
    while (count < maximumItems) {
        int outcome = fscanf(file, "%[^,],%[^,],%[^,],%f,%d\n", id[count], category[count], name[count], &price[count], &quantity[count]);

        // if the end of the file is reached then stop the loop
        if (outcome == EOF) {
            break;
        }

        // if not all 5 items were successfully read then stop (errors in csv or txt file)
        if (outcome != 5) {
            printf("Please re-check your .csv/.txt file.");
            break;
        }

        // move to the next index in the arrays for the next row
        count++;
    }

    return count;
}

///////////////////////////
// Returns the full category name corresponding to a single character.
// Globals: None.
// Arguments: char *arg (pointer to a character representing a category ('s', 'd', 'f') )
// Outputs: None.
// Returns: char* (pointer to a string representing the category name returns NULL if the character is invalid (like if its not s, d, or f))
///////////////////////////
char *getCategory(char *arg) {

    if (strlen(arg) != 1) {
        return NULL;
    }

    char c = arg[0];

    if (c == 's') {
        return "Snacks";
    }

    if (c == 'S') {
        return "Snacks";
    }

    if (c == 'd') {
        return "Drinks";
    }

    if (c == 'D') {
        return "Drinks";
    }

    if (c == 'f') {
        return "FirstAid";
    }

    if (c == 'F') {
        return "FirstAid";
    }

    return "Unknown";
}

///////////////////////////
// Prints all items in the requested category
// Globals: None.
// Arguments: int count (number of items read from the inventory), char *category[] (number of items read from the inventory), char *id[] (array of item IDs), char *name[] (array of item names), float price[] (array of item prices), int quantity[] (array of item quantities), char *arg (pointer to a character representing a category ('s', 'd', 'f') )
// Outputs: Prints item details (ID, Name, Price, Quantity) to the terminal
// Returns: N.A. (void)
///////////////////////////
void printCategory(int count, char *id[], char *category[], char *name[], float price[], int quantity[], char *arg) {
    char* userRequested = getCategory(arg);
    
    if (!userRequested) {
        printf("\n");
        printf("Category not known.\n");
        usage();
        printf("\n");
        return;
    }

    printf("\n");
    if (strcmp(userRequested, "Drinks") == 0) {
        printf("               🍹🥤 Requested Category: %s\n", userRequested);
        printf("\n");
        printf("%-5s | %-20s | %-7s | %-5s\n", "ID", "Item", "Price", "Quantity");
        printf("--------------------------------------------------------------\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(category[i], userRequested) == 0) {
                printf("%-5s | %-20s | %-7.2f | %5d\n", id[i], name[i], price[i], quantity[i]);
            }
        }
    }

    else if (strcmp(userRequested, "Snacks") == 0) {
        printf("            🍓🍡🍬🍭 Requested Category: %s\n", userRequested);
        printf("\n");
        printf("%-5s | %-20s | %-7s | %-5s\n", "ID", "Item", "Price", "Quantity");
        printf("--------------------------------------------------------------\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(category[i], userRequested) == 0) {
                printf("%-5s | %-20s | %-7.2f | %5d\n", id[i], name[i], price[i], quantity[i]);
            }
        }
    }

    else if (strcmp(userRequested, "FirstAid") == 0) {
        printf("                🚑🧰 Requested Category: %s\n", userRequested);
        printf("\n");
        printf("%-5s | %-20s | %-7s | %-5s\n", "ID", "Item", "Price", "Quantity");
        printf("--------------------------------------------------------------\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(category[i], userRequested) == 0) {
                printf("%-5s | %-20s | %-7.2f | %5d\n", id[i], name[i], price[i], quantity[i]);
            }
        }
    }

    else {
        printf("Category not known.\n");
        usage();
        return;
    }

    printf("\n");  
}