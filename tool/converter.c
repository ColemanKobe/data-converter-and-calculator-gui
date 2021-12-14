#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct results_payload {
    GtkEntry *entry;
    int *from;
    int *to;
    GtkWidget *label_to_change;
};

void set_dec(GtkWidget *widget, gpointer data) {
    int *new_val = data;
    *new_val = 1;
}

void set_bin(GtkWidget *widget, gpointer data) {
    int *new_val = data;
    *new_val = 2;
}

void set_hex(GtkWidget *widget, gpointer data) {
    int *new_val = data;
    *new_val = 3;
}

char* d2h(int decimal){
    char* hex = (char *)malloc(sizeof(char)*10);
    for (int h = 0; h < 10; h++){
        *(hex + h) = 32;
    }
    int i = 10;
    while (decimal != 0) {
        // temporary variable to store remainder
        int temp = 0;

        // storing remainder in temp variable.
        temp = decimal % 16;
        // check if temp < 10
        if (temp < 10) {
            *(hex + i) = temp + 48;
            i-=1;
        }
        else {
            *(hex + i) = temp + 55;
            i-=1;
        }

        decimal = decimal / 16;
    }
    return hex;

}

int h2d(const char* hex){
    int decimal = 0;
    int val;
    int length = strlen(hex) - 1;

    for (int i = 0; *(hex + i)!='\0'; i++){
        if(*(hex + i)>='0' && *(hex + i)<='9')
        {
            val = *(hex + i) - 48;
        }
        else if(*(hex + i)>='a' && *(hex + i)<='f')
        {
            val = *(hex + i) - 97 + 10;
        }
        else if(*(hex + i)>='A' && *(hex + i)<='F')
        {
            val = *(hex + i) - 65 + 10;
        }

        decimal += val * pow(16, length);
        length--;
    }
    return decimal;
}

unsigned long d2b(int decimal){
	int bit = 0;
	unsigned long res = 0;
	int exponent = 0;
	int remainder;
	int quotient;
	while (decimal !=0)
	{
		remainder = decimal % 2;
		res += remainder * pow(10, exponent);
		decimal = decimal / 2;
		exponent++;
	}

	return res;
}

int b2d(unsigned long binary){
	int lastDigit;
	int placeCounter = 0;
	int res = 0;
	while (binary != 0)
	{
		lastDigit = binary % 10;
		if (lastDigit == 1)
		{
			res += pow(2, placeCounter);
		}
		placeCounter ++;
		binary = binary / 10;
	}
	return res;
}

void get_answer(GtkWidget *widget, gpointer data) {
    struct results_payload *results_payload = data;
    int from = *(results_payload->from);
    int to = *(results_payload->to);
    GtkEntry* entry = results_payload->entry;
    const char *input = (char *)malloc(sizeof(char)*20);

    char label_output[100];
    input = gtk_entry_get_text(entry);
    // if from and to are equal, give back same input
    if (from == to) {
        sprintf(label_output, "ERROR: 'to' and 'from' are equal");
    // if different, determine the type of conversion
    } else {
        // From Decimal
        if (from == 1){
            int input_number = atoi(input);
            // To binary
            if (to == 2){
                sprintf(label_output, "Result: %lu", d2b(input_number));
            // To hex
            } else if (to == 3) {
                sprintf(label_output, "Result: %s", d2h(input_number));
            }
        // From binary
        } else if (from == 2) {
            char *ptr;
            unsigned long input_number = strtoul(input, &ptr, 10);
            // To decimal
            if (to == 1){
                sprintf(label_output, "Result: %d", b2d(input_number));
            // To hex
            } else if (to == 3) {
                sprintf(label_output, "Result: %s", d2h(b2d(input_number)));
            }
        // From hex
        } else if (from ==3) {
            // To decimal
            if (to == 1){
                sprintf(label_output, "Result: %d", h2d(input));
            // To binary
            } else if (to == 2) {
                sprintf(label_output, "Result: %lu", d2b(h2d(input)));
            }
        }
    }
    gtk_label_set_text(GTK_LABEL(results_payload->label_to_change), label_output);

}

int main(int argc, char *argv[]) {
    int from_selection = 1;
    int to_selection = 1;

    GtkWidget *window;
    GtkWidget *grid;

    GtkWidget *btn_convert;
    GtkWidget *fixed;
    GtkWidget *lbl_results;

    GtkWidget *entry;
    GtkWidget *prompt;

    GtkWidget *from_vbox;
    GtkWidget *rad_from_dec;
    GtkWidget *rad_from_bin;
    GtkWidget *rad_from_hex;

    GtkWidget *to_vbox;
    GtkWidget *rad_to_dec;
    GtkWidget *rad_to_bin;
    GtkWidget *rad_to_hex;


    GtkWidget *frame_from;
    GtkWidget *frame_to;

    gtk_init(&argc, &argv);

    // Create the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Data Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Create grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID (grid), 35);
    gtk_grid_set_column_spacing(GTK_GRID (grid), 70);
    gtk_container_add(GTK_CONTAINER(window), grid);


    // create frames
    frame_from = gtk_frame_new("From");
    gtk_frame_set_shadow_type(GTK_FRAME(frame_from), GTK_SHADOW_IN);
    frame_to = gtk_frame_new("To");
    gtk_frame_set_shadow_type(GTK_FRAME(frame_to), GTK_SHADOW_IN);
    gtk_grid_attach(GTK_GRID(grid), frame_from, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), frame_to, 1, 2, 1, 1);


    // Add convert buttons
    btn_convert = gtk_button_new_with_label("Convert");
    gtk_widget_set_tooltip_text(btn_convert, "Convert");
    gtk_grid_attach(GTK_GRID(grid), btn_convert, 0, 0, 1, 1);

    // Add result label
    lbl_results = gtk_label_new("");
    gtk_label_set_justify(GTK_LABEL(lbl_results), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), lbl_results, 1, 0, 1, 1);

    // Add prompt and textbox
    prompt = gtk_label_new ("Enter your number:" );
    gtk_grid_attach(GTK_GRID(grid), prompt, 0, 1, 1, 1);
    entry = gtk_entry_new ();
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);

    // Add from radios
    from_vbox = gtk_box_new(TRUE, 0);
    rad_from_dec = gtk_radio_button_new_with_label(NULL, "Decimal");
    rad_from_bin = gtk_radio_button_new_with_label(NULL, "Binary");
    gtk_radio_button_join_group(GTK_RADIO_BUTTON(rad_from_bin), GTK_RADIO_BUTTON(rad_from_dec));
    rad_from_hex = gtk_radio_button_new_with_label(NULL, "Hexadecimal");
    gtk_radio_button_join_group(GTK_RADIO_BUTTON(rad_from_hex), GTK_RADIO_BUTTON(rad_from_dec));

    gtk_box_pack_start(GTK_BOX (from_vbox), rad_from_dec, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (from_vbox), rad_from_bin, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (from_vbox), rad_from_hex, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER(frame_from), from_vbox);

    // Add to radios
    to_vbox = gtk_box_new(TRUE, 0);
    rad_to_dec = gtk_radio_button_new_with_label(NULL, "Decimal");
    rad_to_bin = gtk_radio_button_new_with_label(NULL, "Binary");
    gtk_radio_button_join_group(GTK_RADIO_BUTTON(rad_to_bin), GTK_RADIO_BUTTON(rad_to_dec));
    rad_to_hex = gtk_radio_button_new_with_label(NULL, "Hexadecimal");
    gtk_radio_button_join_group(GTK_RADIO_BUTTON(rad_to_hex), GTK_RADIO_BUTTON(rad_to_dec));

    gtk_box_pack_start(GTK_BOX (to_vbox), rad_to_dec, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (to_vbox), rad_to_bin, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (to_vbox), rad_to_hex, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER(frame_to), to_vbox);


    // Signals

    g_signal_connect(G_OBJECT(rad_from_dec), "toggled", G_CALLBACK(set_dec), &from_selection);
    g_signal_connect(G_OBJECT(rad_from_bin), "toggled", G_CALLBACK(set_bin), &from_selection);
    g_signal_connect(G_OBJECT(rad_from_hex), "toggled", G_CALLBACK(set_hex), &from_selection);

    g_signal_connect(G_OBJECT(rad_to_dec), "toggled", G_CALLBACK(set_dec), &to_selection);
    g_signal_connect(G_OBJECT(rad_to_bin), "toggled", G_CALLBACK(set_bin), &to_selection);
    g_signal_connect(G_OBJECT(rad_to_hex), "toggled", G_CALLBACK(set_hex), &to_selection);


    struct results_payload results_data;
    results_data.entry = GTK_ENTRY(entry);
    results_data.from = &from_selection;
    results_data.to = &to_selection;
    results_data.label_to_change = lbl_results;
    g_signal_connect(G_OBJECT(btn_convert), "clicked",
        G_CALLBACK(get_answer), &results_data);

    g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);


    gtk_main();

    return 0;
}