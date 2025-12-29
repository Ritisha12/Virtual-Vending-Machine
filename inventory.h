/* Team 13
 * Manasvi Bandi, Lakshmi Saranya Alamanda, Ritisha Perumalla, Claira Corkery
 * 400570531, 400560245, 400560957, 400593241
 * 2025-11-24
 * This file contains constants and function for managing all in one file.
 */

#define maximumItems 20
#define idLength 5
#define categoryLength 10
#define nameLength 25

void usage();
void help();
int read_inventory(FILE *file, char *id[], char *category[], char *name[], float price[], int quantity[]);
char *getCategory(char *arg);
void printCategory(int count, char *id[], char *category[], char *name[], float price[], int quantity[], char *arg);