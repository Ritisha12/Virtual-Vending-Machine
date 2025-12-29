/* Team 13
 * Manasvi Bandi, Lakshmi Saranya Alamanda, Ritisha Perumalla, Claira Corkery
 * 400570531, 400560245, 400560957, 400593241
 * 2025-12-01
 * This file is for testing the individual functions and checking them one by one to make sure they are doing what they are supposed to do.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "inventory.h"
#include "payment.h"
#include "purchase.h"

int main() {

    ///////////////////////////
    // Testing inventory.c
    // Function made by: Manasvi
    ///////////////////////////
    printf("----Testing inventory.c----\n");
    
    Item items[20]; 
    int count;
    
    count = loading_inventory("inventory.csv", items, 20);

    if (count == 15) { // your CSV has 15 items
        printf("loading_inventory has PASSED\n");
    } 
    
    else {
        printf("loading_inventory has FAILED\n");
    }

    // getCategory tests
    if (strcmp(getCategory("s"), "Snacks") == 0) {
        printf("getCategory('s') PASSED\n");
    } 
    
    else {
        printf("getCategory('s') FAILED\n");
    }

    if (strcmp(getCategory("D"), "Drinks") == 0) {
        printf("getCategory('D') PASSED\n");
    } 
    
    else {
        printf("getCategory('D') FAILED\n");
    }

    if (getCategory("x") == NULL) {
        printf("getCategory('x') PASSED - returned NULL\n");
    } 
    
    else if (strcmp(getCategory("x"), "Unknown") == 0) {
        printf("getCategory('x') PASSED - returned Unknown\n");
    } 
    
    else {
        printf("getCategory('x') FAILED\n");
    }

    // Test getCategory invalid length
    if (getCategory("xx") == NULL) {
        printf("getCategory('xx') PASSED\n");
    }

    // Convert items to array format for printCategory
    char *id[20], *category[20], *name[20];
    float price[20];
    int quantity[20];

    for (int i = 0; i < count; i++) {
        id[i] = items[i].id;
        category[i] = items[i].category;
        name[i] = items[i].name;
        price[i] = items[i].price;
        quantity[i] = items[i].quantity;
    }

    printf("\nTesting printCategory with Snacks\n");
    printCategory(count, id, category, name, price, quantity, "s");

    printf("\nTesting printCategory with Drinks\n");
    printCategory(count, id, category, name, price, quantity, "D");

    printf("\nTesting printCategory with FirstAid\n");
    printCategory(count, id, category, name, price, quantity, "f");

    printf("\nTesting printCategory unknown category\n");
    printCategory(count, id, category, name, price, quantity, "Z"); 

    printf("\nTesting usage()\n");
    usage();

    printf("\nTesting help()\n");
    help();
    
    // 1. Direct test of read_inventory function
    printf("\nTesting read_inventory directly\n");
    char *testIDs[20], *testCategories[20], *testNames[20];
    float testPrices[20];
    int testQtys[20];
    
    FILE* directFile = fopen("inventory.csv", "r");
    if (directFile) {
        int readCount = read_inventory(directFile, testIDs, testCategories, testNames, testPrices, testQtys);
        fclose(directFile);
        printf("read_inventory returned %d items\n", readCount);
        
        // Free allocated memory
        for (int i = 0; i < 20; i++) {
            free(testIDs[i]);
            free(testCategories[i]);
            free(testNames[i]);
        }
    }
    
    // 2. Test read_inventory with malformed CSV (missing quantity)
    printf("\nTesting read_inventory with bad CSV format\n");
    FILE* badFormatFile = fopen("bad_format.csv", "w");
    fprintf(badFormatFile, "ID,Category,Name,Price,Quantity\n");
    fprintf(badFormatFile, "T1,Snacks,Test,1.50,5\n");
    fprintf(badFormatFile, "T2,Drinks,BadRow,2.00\n");  
    fclose(badFormatFile);
    
    char *badIDs[20], *badCategories[20], *badNames[20];
    float badPrices[20];
    int badQtys[20];
    
    FILE* badReadFile = fopen("bad_format.csv", "r");
    if (badReadFile) {
        int badCount = read_inventory(badReadFile, badIDs, badCategories, badNames, badPrices, badQtys);
        fclose(badReadFile);
        printf("read_inventory with bad format returned %d items\n", badCount);
        
        // Free allocated memory
        for (int i = 0; i < 20; i++) {
            free(badIDs[i]);
            free(badCategories[i]);
            free(badNames[i]);
        }
    }
    remove("bad_format.csv");
    
    // 3. Test read_inventory with empty file (triggers EOF)
    printf("\nTesting read_inventory with empty CSV\n");
    FILE* emptyTestFile = fopen("empty_test.csv", "w");
    fprintf(emptyTestFile, "ID,Category,Name,Price,Quantity\n");  // Only header
    fclose(emptyTestFile);
    
    char *emptyIDs[20], *emptyCategories[20], *emptyNames[20];
    float emptyPrices[20];
    int emptyQtys[20];
    
    FILE* emptyReadFile = fopen("empty_test.csv", "r");
    if (emptyReadFile) {
        int emptyCount = read_inventory(emptyReadFile, emptyIDs, emptyCategories, emptyNames, emptyPrices, emptyQtys);
        fclose(emptyReadFile);
        printf("read_inventory with empty file returned %d items\n", emptyCount);
        
        // Free allocated memory
        for (int i = 0; i < 20; i++) {
            free(emptyIDs[i]);
            free(emptyCategories[i]);
            free(emptyNames[i]);
        }
    }
    remove("empty_test.csv");
    
    // 4. Test getCategory returning "Unknown"
    printf("\nTesting getCategory with truly invalid character\n");
    char* unknownResult = getCategory("z");  
    if (strcmp(unknownResult, "Unknown") == 0) {
        printf("getCategory('z') returned Unknown: PASSED\n");
    } 
    
    else {
        printf("getCategory('z') returned Unknown: FAILED (got: %s)\n", unknownResult);
    }
    
    // 5. Test printCategory with "Unknown" category
    printf("\nTesting printCategory with Unknown category\n");
    printCategory(count, id, category, name, price, quantity, "z");

    // This is for testing the categories 

    // Test uppercase 'S' 
    if (strcmp(getCategory("S"), "Snacks") == 0) {
        printf("getCategory('S') uppercase PASSED\n");
    } 
    
    else {
        printf("getCategory('S') uppercase FAILED\n");
    }

    // Test lowercase 'd'
    if (strcmp(getCategory("d"), "Drinks") == 0) {
        printf("getCategory('d') lowercase PASSED\n");
    } 
    
    else {
        printf("getCategory('d') lowercase FAILED\n");
    }

    // Test uppercase 'F' 
    if (strcmp(getCategory("F"), "FirstAid") == 0) {
        printf("getCategory('F') uppercase PASSED\n");
    } 
    
    else {
        printf("getCategory('F') uppercase FAILED\n");
    }

    // Test printCategory with NULL category 
    printf("\nTesting printCategory with multi-character input (triggers NULL check)\n");
    printCategory(count, id, category, name, price, quantity, "abc");

    // Test printCategory with S (uppercase)
    printf("\nTesting printCategory with uppercase S\n");
    printCategory(count, id, category, name, price, quantity, "S");

    // Test printCategory with d (lowercase)
    printf("\nTesting printCategory with lowercase d\n");
    printCategory(count, id, category, name, price, quantity, "d");

    // Test printCategory with F (uppercase)
    printf("\nTesting printCategory with uppercase F\n");
    printCategory(count, id, category, name, price, quantity, "F");

    ///////////////////////////
    // Testing purchase.c
    // Function made by: Ritisha
    ///////////////////////////
    printf("\n----Testing purchase.c----\n");

    // Reload items for fresh testing
    count = loading_inventory("inventory.csv", items, 20);

    // Test buy_item with valid ID
    printf("\nTesting buy_item with valid ID\n");
    int purchasedIndex;
    int initial_qty = items[0].quantity;
    int result = buy_item(items, count, items[0].id, &purchasedIndex);
    if (result == 0 && items[0].quantity == initial_qty - 1) {
        printf("buy_item valid ID PASSED\n");
    } 
    
    else {
        printf("buy_item valid ID FAILED\n");
    }

    // Test buy_item out of stock
    printf("\nTesting buy_item out of stock\n");
    items[1].quantity = 0;
    result = buy_item(items, count, items[1].id, &purchasedIndex);
    if (result == -1) {
        printf("buy_item out of stock PASSED\n");
    } 
    
    else {
        printf("buy_item out of stock FAILED\n");
    }

    // Test buy_item invalid ID
    printf("\nTesting buy_item with invalid ID\n");
    result = buy_item(items, count, "INVALID_ID", &purchasedIndex);
    if (result == -2) {
        printf("buy_item invalid ID PASSED\n");
    } 
    
    else {
        printf("buy_item invalid ID FAILED\n");
    }

    // Test save_inventory success
    printf("\nTesting save_inventory success path\n");
    save_inventory("test_inventory.csv", items, count);
    printf("save_inventory success path executed\n");

    // Test save_inventory failure
    printf("\nTesting save_inventory failure path\n");
    save_inventory("/invalid_path/test_inventory.csv", items, count);
    printf("save_inventory failure path executed\n");

    // Clean up
    remove("test_inventory.csv");

    printf("\nTesting loading_inventory with non-existent file\n");
    Item dummy_items[20];
    int fail_count = loading_inventory("this_file_does_not_exist.csv", dummy_items, 20);
    if (fail_count == 0) {
        printf("loading_inventory with non-existent file PASSED (returned 0)\n");
    } 
    
    else {
        printf("loading_inventory with non-existent file FAILED\n");
    }

    // Test loading_inventory with invalid path
    printf("\nTesting loading_inventory with invalid path\n");
    int fail_count2 = loading_inventory("/invalid/path/inventory.csv", dummy_items, 20);
    if (fail_count2 == 0) {
        printf("loading_inventory with invalid path PASSED (returned 0)\n");
    } 
    
    else {
        printf("loading_inventory with invalid path FAILED\n");
    }

    // Test case to trigger display_transaction
    Item testItem;
    snprintf(testItem.id, sizeof(testItem.id), "S01");
    snprintf(testItem.category, sizeof(testItem.category), "Snacks");
    snprintf(testItem.name, sizeof(testItem.name), "Cheesy Doritos");
    testItem.price = 2.67;
    testItem.quantity = 4;

    // Test display_transaction
    printf("---- Testing display_transaction ----\n");
    display_transaction(&testItem);

    // Test print_receipt
    printf("---- Testing print_receipt ----\n");
    float userPayment = 3.00;
    float tax = testItem.price * 0.13;
    float change = userPayment - (testItem.price + tax);

    print_receipt(&testItem, userPayment, change, tax);

    // Another test case: high price & exact payment
    Item expensiveItem;
    snprintf(expensiveItem.id, sizeof(expensiveItem.id), "F19");
    snprintf(expensiveItem.category, sizeof(expensiveItem.category), "FirstAid");
    snprintf(expensiveItem.name, sizeof(expensiveItem.name), "Deluxe First Aid Kit");
    expensiveItem.price = 99.99;
    expensiveItem.quantity = 2;

    printf("\n---- Testing print_receipt with expensive item ----\n");
    userPayment = 112.49;  // $99.99 + 13% tax
    tax = expensiveItem.price * 0.13;
    change = userPayment - (expensiveItem.price + tax);

    // Test case to trigger print_receipt
    print_receipt(&expensiveItem, userPayment, change, tax);

    ///////////////////////////
    // Testing payment.c
    ///////////////////////////
    printf("\n----Testing payment.c----\n");

    // 1. Test findprice
    printf("\nTesting findprice...\n");
    float price_value;        // single float
    price_value = findprice("S01");
    printf("Price of S01: %.2f\n", price_value);
    price_value = findprice("S01"); // Assuming S01 exists in inventory.csv
    printf("Price of S01: %.2f\n", *price);

    // 2. Test getchange (simulate cash)
    printf("\nTesting getchange with cash...\n");

    // Create the input file
    FILE *f = fopen("input.txt", "w");
    fprintf(f, "cash\n5.00\n");  // type "cash" then insert 5.00
    fclose(f);

    // Redirect stdin
    freopen("input.txt", "r", stdin);

    float useramount;
    float newChange = getchange(price_value, &useramount);

    printf("User amount: %.2f, Change: %.2f\n", useramount, newChange);

    // Restore stdin and remove temp file
    freopen("/dev/tty", "r", stdin);
    remove("input.txt");

    // Card payment test
    FILE *f_card = fopen("input_card.txt", "w");
    fprintf(f_card, "card\n");
    fclose(f_card);

    freopen("input_card.txt", "r", stdin);
    float cardAmount;
    float cardChange = getchange(price_value, &cardAmount);
    printf("Card payment: User amount %.2f, Change %.2f\n", cardAmount, cardChange);
    freopen("/dev/tty", "r", stdin);
    remove("input_card.txt");

    // insufficienct cash
    FILE *f_retry = fopen("input_retry.txt", "w");
    fprintf(f_retry, "cash\n1.00\nY\n5.00\n");
    fclose(f_retry);

    freopen("input_retry.txt", "r", stdin);
    float retryAmount;
    float retryChange = getchange(price_value, &retryAmount);
    printf("Retry: User amount %.2f, Change %.2f\n", retryAmount, retryChange);
    freopen("/dev/tty", "r", stdin);
    remove("input_retry.txt");

    // handling uncovered lines in finfprice
    float missing = findprice("INVALID_ID"); // test ID not in inventory.csv
    printf("Price of INVALID_ID: %.2f\n", missing);

    // else in getchange
    FILE *f_invalid = fopen("input_invalid.txt", "w");
    fprintf(f_invalid, "bitcoin\n"); // invalid payment type
    fclose(f_invalid);

    freopen("input_invalid.txt", "r", stdin);
    float invalidAmount;
    float invalidChange = getchange(price_value, &invalidAmount);
    freopen("/dev/tty", "r", stdin);
    remove("input_invalid.txt");

    // not enough money
    FILE *f_cancel = fopen("input_cancel.txt", "w");
    fprintf(f_cancel, "cash\n1.00\nN\n"); // fails and cancels
    fclose(f_cancel);

    freopen("input_cancel.txt", "r", stdin);
    float cancelAmount;
    float cancelChange = getchange(price_value, &cancelAmount);
    freopen("/dev/tty", "r", stdin);
    remove("input_cancel.txt");

    // invalid retry
    FILE *f_bad = fopen("input_bad.txt", "w");
    fprintf(f_bad, "cash\n1.00\nX\n"); // invalid retry option
    fclose(f_bad);

    freopen("input_bad.txt", "r", stdin);
    float badAmount;
    float badChange = getchange(price_value, &badAmount);
    freopen("/dev/tty", "r", stdin);
    remove("input_bad.txt");

    // random file not found
    rename("inventory.csv", "inventory.random");
    float missingFile = findprice("S01");
    rename("inventory.random", "inventory.csv");

    // getchange not getting triggerd
    FILE *f_invalidcash = fopen("input_invalidcash.txt", "w");
    fprintf(f_invalidcash, "cash\n0.0001\n"); // triggers 'else'
    fclose(f_invalidcash);
    freopen("input_invalidcash.txt", "r", stdin);
    float invalidCashAmount;
    float invalidCashChange = getchange(price_value, &invalidCashAmount);
    freopen("/dev/tty", "r", stdin);
    remove("input_invalidcash.txt");

    // not enough money not getting triggered
    FILE *f_retry_exact = fopen("input_retry_exact.txt", "w");
    fprintf(f_retry_exact, "cash\n1.00\nY\n%.2f\n", price_value * 1.13); // exact total with tax
    fclose(f_retry_exact);
    freopen("input_retry_exact.txt", "r", stdin);
    float retryExactAmount;
    float retryExactChange = getchange(price_value, &retryExactAmount);
    freopen("/dev/tty", "r", stdin);
    remove("input_retry_exact.txt");

    

    printf("\n==== All tests are complete! ====\n");
    return 0;
}