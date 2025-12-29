#include <gtk/gtk.h>
#include <stdio.h>

#include "inventoryDisplay.h"
#include "receiptDisplay.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Launch the inventory window (pass filename or NULL)
    launch_inventory_window(NULL); // will use default "inventory.csv"

    // Launch the receipt window (pass NULL or a filename if you want)
    launch_receipt_window(NULL);

    // Single GTK main loop for both windows
    gtk_main();
    return 0;
}

