/*payment.c takes the ID that was sent from
 * another C files and grabs the price.
 * It then takes either card or cash payments
 * and returns the change and how much cash
 * the user entered.
 *
 * Claira Corkery, 12/1/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "payment.h"
/* Takes the ID and opens the .csv file.
 * It then searches until it finds the ID
 * of what the user entered and grabs the
 * price. It then returns the price back
 * to the main.
 */

float findprice(char *useritem)
{
    char buffer[1000];
    FILE *file;
    file = fopen("inventory.csv", "r");
    float price;
    char *items;
    while ((fgets(buffer, sizeof(buffer), file)))
    {
        items = strtok(buffer, ",");
        while (items)
        {
            if (strcmp(items, useritem) == 0)
            {
                items = strtok(NULL, ","); // Moving the index of items to the price in the .csv file
                items = strtok(NULL, ",");
                items = strtok(NULL, ",");
                price = atof(items);
                return price;
            }
            items = strtok(NULL, ",");
        }
    }
    fclose(file);
}

/* Takes the users input on if they want to pay
 * cash or card. If card is picked the payment goes
 * through automatically. If cash is picked it will take
 * in the cash amount and will either print the change or
 * throw an error if they put under the amount. EXIT_FAILURE
 *  will be used if they didnt pick cash or card.
 */

float getchange(float Price, float *useramount)
{
    char paymenttype[5];
    float total;
    float cash;
    float change;
    printf("Are you paying by cash or card? (enter cash/card): ");
    scanf("%4s", paymenttype);
    if (strcasecmp(paymenttype, "cash") == 0)
    {
        total = roundf(((Price * 0.13) + Price) * 100) / 100;
        printf("The total is $%.2f. Please insert your cash: ", total);
        scanf("%f", &cash);
        if ((fabs(cash - total) < 0.001))
        {
            change = 0;
            *useramount = cash;
            
        } else if((cash < total)){
            cash = 0;
            printf("Failed transaction. Giving cash back.\n");
            change = notenoughmoney(total, change, useramount);
        }
        else if (cash > total)
        {
            change = cash - total;
            *useramount = cash;
        }
        else
        {
            printf("Invalid\n");
        }
    }
    else if (strcasecmp(paymenttype, "card") == 0)
    {
        total = (Price * 0.13) + Price;
        printf("The total is $%.2f. Please insert your card: ", total);
        change = 0;
        *useramount = total;
    }
    else
    {
        printf("Incorrect option chosen closing out program.\n");
        exit(EXIT_FAILURE);
    }
    return change;
}

/* If the user doesnt enter the right amount of money this function will run.
 * It gives back the money back and will allow them to try to
 * pay again or if want to cancel EXIT_FAILURE is used to stop the
 * code. Otherwise it returns the change.
 */

float notenoughmoney(float total, float change, float *useramount)
{
    char retrypayment[2];
    float updatedchange;
    printf("Would you like to try again (type Y or N): ");
    while (scanf("%1s", retrypayment))
    {
        if (strcasecmp(retrypayment, "Y") == 0)
        {
            printf("The total is $%.2f. Please insert cash: ", total);
            scanf("%f", &updatedchange);
            if((fabs(updatedchange - total) < 0.001)){
                *useramount = updatedchange;
                change = 0;
                return change;
            }
            if (updatedchange < total)
            {
                printf("Transaction failed. Giving money back. Do you want to try again? (type Y or N): ");
            }
            if (updatedchange > total)
            {
                *useramount = updatedchange;
                change = updatedchange - total;
                return change;
            }
        }
        else if (strcasecmp(retrypayment, "N") == 0)
        {
            printf("Returning money back\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Incorrect option chosen closing out program.\n");
            exit(EXIT_FAILURE);
        }
    }
}