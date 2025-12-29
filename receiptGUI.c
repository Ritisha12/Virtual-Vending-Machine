#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SZ 2048 // max length for one line from the CSV

// Struct now holds ID, Category, Name, Price (all strings)
typedef struct {
    char a[512], b[512], c[512], price[64];
} Row;

static void read_csv(const char *fn, Row **out, int *n) {
    FILE *f = fopen(fn, "r");
    if (!f) { 
	    *out = NULL; 
	    *n = 0; 
	    return; 
    }
    
    int cap = 128;
    Row *rows = malloc(cap * sizeof(Row));
    *n = 0;
    char line[LINE_SZ];
    int lineno = 0;

    while (fgets(line, sizeof line, f)) {
        lineno++;
        line[strcspn(line, "\r\n")] = 0;
        if (lineno == 1 && strncmp(line, "ID,", 3) == 0) 
		continue;
        if (line[0] == 0) 
		continue;

        char *t = line, *tok;
        tok = strtok(t, ","); 
	strncpy(rows[*n].a, tok ? tok : "", 511);   // ID
        tok = strtok(NULL, ","); 
	strncpy(rows[*n].b, tok ? tok : "", 511); // Category
        tok = strtok(NULL, ","); 
	strncpy(rows[*n].c, tok ? tok : "", 511); // Name
        tok = strtok(NULL, ","); 
	strncpy(rows[*n].price, tok ? tok : "0", 63); // Price (4th token)
        (*n)++;
        if (*n >= cap) { 
		cap *= 2; 
		rows = realloc(rows, cap * sizeof(Row)); 
	}
    }

    fclose(f);
    if (*n == 0) { 
	    free(rows); 
	    rows = NULL; 
    }
    *out = rows;
}

// Toggle/checkbox callback
static void on_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer user_data) {
    GtkListStore *store = GTK_LIST_STORE(user_data);
    GtkTreeIter iter;
    GtkTreePath *path = gtk_tree_path_new_from_string(path_str);
    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path)) {
        gboolean val = FALSE;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &val, -1);
        val = !val;
        gtk_list_store_set(store, &iter, 0, val, -1);
    }
    gtk_tree_path_free(path);
}

// Print receipt: reads columns 1=ID,2=Category,3=Name,4=Price 
static void on_print_receipt(GtkButton *b, gpointer user_data) {
    GtkTreeView *tv = GTK_TREE_VIEW(user_data);
    GtkTreeModel *model = gtk_tree_view_get_model(tv);
    GString *out = g_string_new("VENDING MACHINE\n-------------------------\n");

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    gboolean any = FALSE;
    double total = 0.0;

    while (valid) {
        gboolean sel = FALSE;
        char *id = NULL, *cat = NULL, *name = NULL, *price_s = NULL;

        // model columns: 0=selected,1=ID,2=Category,3=Name,4=Price
        gtk_tree_model_get(model, &iter,
                           0, &sel,
                           1, &id,
                           2, &cat,
                           3, &name,
                           4, &price_s,
                           -1);

        if (sel) {
            any = TRUE;
            double price = price_s ? atof(price_s) : 0.0;
            total += price;
            // print Name (or fallback to ID) and price 
            g_string_append_printf(out, "%-20s   $%.2f\n",
                                   name ? name : (id ? id : "Item"),
                                   price);
        }

        if (id) g_free(id);
        if (cat) g_free(cat);
        if (name) g_free(name);
        if (price_s) g_free(price_s);

        valid = gtk_tree_model_iter_next(model, &iter);
    }

    if (!any) {
        GtkWidget *d = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                              "No items selected.");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        g_string_free(out, TRUE);
        return;
    }

    g_string_append(out, "-------------------------\n");
    g_string_append_printf(out, "TOTAL:       $%.2f\n", total);

    GtkWidget *d = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                          GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                          "%s", out->str);
    gtk_window_set_title(GTK_WINDOW(d), "Receipt");
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);

    g_string_free(out, TRUE);
}

int main(int argc, char *argv[]) {
    const char *file = (argc > 1) ? argv[1] : "inventory.csv";
    gtk_init(&argc, &argv);

    Row *rows; int n;
    read_csv(file, &rows, &n);

    GtkWidget *w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(w),"Vending Machine - Item Selection");
    gtk_window_set_default_size(GTK_WINDOW(w), 800, 400);
    g_signal_connect(w, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // create treeview and scrolled window ONCE
    GtkWidget *tv = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tv), TRUE);

    GtkWidget *sc = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sc),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(sc), tv);   // add treeview to scrolled window 

    // make a vertical box to hold scrolled window and button 
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(w), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), sc, TRUE, TRUE, 0);

    /* model + columns
       Column 0: selection (boolean)
       Column 1: ID (string)
       Column 2: Category (string)
       Column 3: Name (string)
       Column 4: Price (string)
    */
    GtkListStore *store = gtk_list_store_new(5, G_TYPE_BOOLEAN, G_TYPE_STRING,
                                             G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Toggle/checkbox column 
    GtkCellRenderer *renderer_toggle = gtk_cell_renderer_toggle_new();
    g_signal_connect(renderer_toggle, "toggled", G_CALLBACK(on_toggle), store);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Add",
                                                renderer_toggle, "active", 0, NULL);

    // ID column 
    GtkCellRenderer *r = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "ID", r, "text", 1, NULL);
    // Category column
    r = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Category", r, "text", 2, NULL);
    // Name column
    r = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Name", r, "text", 3, NULL);
    // Price column
    r = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Price", r, "text", 4, NULL);

    GtkTreeIter it;
    if (rows) {
        for (int i = 0; i < n; ++i) {
            gtk_list_store_append(store, &it);
            // default selection = FALSE; set price column from CSV 
            gtk_list_store_set(store, &it,
                               0, FALSE,
                               1, rows[i].a,
                               2, rows[i].b,
                               3, rows[i].c,
                               4, rows[i].price,
                               -1);
        }
        free(rows);
    } else {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(w), GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Could not open '%s' or file empty.", file);
        gtk_dialog_run(GTK_DIALOG(d)); 
	gtk_widget_destroy(d);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Print Receipt button 
    GtkWidget *btn = gtk_button_new_with_label("Print Receipt");
    gtk_box_pack_start(GTK_BOX(vbox), btn, FALSE, FALSE, 0);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_print_receipt), tv);

    gtk_widget_show_all(w);
    gtk_main();
    return 0;
}

