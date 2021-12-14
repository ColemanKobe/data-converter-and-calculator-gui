#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


struct results_payload {
    GtkEntry *entry_top;
    GtkEntry *entry_bot;
    GtkWidget *label_to_change;
};

long bin_add(long num1, long num2){
    int power_counter = 0;
    long sum = 0;
    int remainder = 0;

    while (num1 != 0 || num2 != 0) {
        sum += ((num1 % 10 + num2 % 10 + remainder) % 2) * pow(10, power_counter);
        remainder =(num1 % 10 + num2 % 10 + remainder) / 2;
        num1 = num1 / 10;
        num2 = num2 / 10;
        power_counter += 1;
    }
    if (remainder != 0){
        sum += (remainder * pow(10, power_counter));
    }
    return sum;
}

void ones_comp(char* num){
    int index = 0;
    while (*(num + index) != '\0'){
        if (*(num + index) == '0'){
            *(num + index) = '1';
        } else {
             *(num + index) = '0';
        }
        index += 1;
    }
}

long twos_comp(char* num){
    ones_comp(num);
    char *num_ptr;
    long ones_compliment = strtol(num, &num_ptr, 10);
    long twos_compliment = bin_add(ones_compliment, 1);
    return twos_compliment;
}

char* bin_sub(const char* num1, const char* num2){
    // create a modifyable copy of num2
    char* num2_copy = (char *)malloc(sizeof(char)*21);
    strcpy(num2_copy, num2);
    long twos_compliment = twos_comp(num2_copy);
    char *num1_ptr;
    long long_num1 = strtol(num1, &num1_ptr, 10);

    char negative_sign = 32;
    int power_counter = 0;
    long sum = 0;
    int remainder = 0;
    long i;

    while (long_num1 != 0 || twos_compliment != 0) {
        sum += ((long_num1 % 10 + twos_compliment % 10 + remainder) % 2) * pow(10, power_counter);
        remainder =(long_num1 % 10 + twos_compliment % 10 + remainder) / 2;
        long_num1 = long_num1 / 10;
        twos_compliment = twos_compliment / 10;
        power_counter += 1;
    }
    if (remainder == 0){
        negative_sign = '-';
    }
    char* result = (char*)malloc(22 * sizeof(char));
    sprintf(result, "%c%ld", negative_sign, sum);
    return result;
}
long binaryproduct(long binary1, long binary2)
{
    int i = 0, remainder = 0, sum[20];
    long binaryprod = 0;

    while (binary1 != 0 || binary2 != 0)
    {
        sum[i++] =(binary1 % 10 + binary2 % 10 + remainder) % 2;
        remainder =(binary1 % 10 + binary2 % 10 + remainder) / 2;
        binary1 = binary1 / 10;
        binary2 = binary2 / 10;
    }
    if (remainder != 0)
        sum[i++] = remainder;
    --i;
    while (i >= 0)
        binaryprod = binaryprod * 10 + sum[i--];
    return binaryprod;
}
long bin_mult(long num1, long num2) {
    int digit, factor = 1;
    long multiply = 0;

    while (num2 != 0)
    {
        digit =  num2 % 10;
        if (digit == 1)
        {
            num1 = num1 * factor;
            multiply = binaryproduct(num1, multiply);
        }
        else
            num1 = num1 * factor;
        num2 = num2 / 10;
        factor = 10;
    }
    return multiply;
}

char* bin_div(const char* num1, const char* num2) {
    // Initialize
    long i = 0;
    long count = strlen(num1);
    char* q = (char *)malloc(sizeof(char)*21);
    strcpy(q, num1);
    char* m = (char *)malloc(sizeof(char)*21);
    strcpy(m, num2);
    char* a = (char *)malloc(sizeof(char)*21);
    while (i < count + 1) {
     *(a + i) = '0';
     i++;
    }
    *(a + count + 1) = '\0';
    char* temp = (char *)malloc(sizeof(char)*21);


    while (strlen(m) <= strlen(q)) {
        i = strlen(m) + 1;
        while (i > 0) {
            *(m + i) = *(m + i - 1);
            i--;
        }
        *(m) = '0';
    }

    while (count != 0) {
        // left shift
        i = 0;
        while (i < strlen(a) - 1) {
          *(a + i) = *(a + i + 1);
          i++;
        }
        *(a + strlen(a) - 1) = *(q);

        i = 0;
        while (i < strlen(q) - 1) {
          *(q + i) = *(q + i + 1);
          i++;
        }

        // subtract
        strcpy(temp, a);
        a = bin_sub(a, m);
        if(*a == '-') {
           i = 0;
           while (i <= strlen(a) - 1) {
            *(a + i) = *(a + i + 1);
            i++;
           }
        }
        if (*(a) == '1') {
              *(q + strlen(q) - 1) = '0';
              memset(a, 0, sizeof(char) * strlen(a));
              // addition
              strcpy(a, temp);
        }
        else {
            *(q + strlen(q) - 1) = '1';
        }
        memset(temp, 0, sizeof(char) * strlen(temp));
        count--;
    }
    return q;
}

void add(GtkWidget *widget, gpointer data) {
    struct results_payload *results_payload = data;
    GtkEntry* entry_top = results_payload->entry_top;
    GtkEntry* entry_bot = results_payload->entry_bot;
    GtkWidget* label_to_change = results_payload->label_to_change;

    const char *input_top = (char *)malloc(sizeof(char)*21);
    input_top = gtk_entry_get_text(entry_top);

    const char *input_bot = (char *)malloc(sizeof(char)*21);
    input_bot = gtk_entry_get_text(entry_bot);

    char *top_ptr;
    char *bot_ptr;

    long top_num = strtol(input_top, &top_ptr, 10);
    long bot_num = strtol(input_bot, &top_ptr, 10);
    long sum = bin_add(top_num, bot_num);

    char* label_output = (char*)malloc(50 * sizeof(char));
    sprintf(label_output, "Answer: %ld", sum);
    gtk_label_set_text(GTK_LABEL(results_payload->label_to_change), label_output);

}

void subtract(GtkWidget *widget, gpointer data) {
    struct results_payload *results_payload = data;
    GtkEntry* entry_top = results_payload->entry_top;
    GtkEntry* entry_bot = results_payload->entry_bot;
    GtkWidget* label_to_change = results_payload->label_to_change;

    const char *input_top = (char *)malloc(sizeof(char)*21);
    input_top = gtk_entry_get_text(entry_top);

    const char *input_bot = (char *)malloc(sizeof(char)*21);
    input_bot = gtk_entry_get_text(entry_bot);


    char *result = bin_sub(input_top, input_bot);
    char* label_output = (char*)malloc(50 * sizeof(char));
    sprintf(label_output, "Answer: %s", result);
    gtk_label_set_text(GTK_LABEL(results_payload->label_to_change), label_output);
}

void multiply(GtkWidget *widget, gpointer data) {
    struct results_payload *results_payload = data;
    GtkEntry* entry_top = results_payload->entry_top;
    GtkEntry* entry_bot = results_payload->entry_bot;
    GtkWidget* label_to_change = results_payload->label_to_change;

    const char *input_top = (char *)malloc(sizeof(char)*21);
    input_top = gtk_entry_get_text(entry_top);

    const char *input_bot = (char *)malloc(sizeof(char)*21);
    input_bot = gtk_entry_get_text(entry_bot);

    char *top_ptr;
    char *bot_ptr;

    long top_num = strtol(input_top, &top_ptr, 10);
    long bot_num = strtol(input_bot, &bot_ptr, 10);
    long product = bin_mult(top_num, bot_num);

    char* label_output = (char*)malloc(50 * sizeof(char));
    sprintf(label_output, "Answer: %ld", product);
    gtk_label_set_text(GTK_LABEL(results_payload->label_to_change), label_output);
}

void divide(GtkWidget *widget, gpointer data) {
    struct results_payload *results_payload = data;
    GtkEntry* entry_top = results_payload->entry_top;
    GtkEntry* entry_bot = results_payload->entry_bot;
    GtkWidget* label_to_change = results_payload->label_to_change;

    const char *input_top = (char *)malloc(sizeof(char)*21);
    input_top = gtk_entry_get_text(entry_top);

    const char *input_bot = (char *)malloc(sizeof(char)*21);
    input_bot = gtk_entry_get_text(entry_bot);

    char* quotient = bin_div(input_top, input_bot);

    char* label_output = (char*)malloc(50 * sizeof(char));
    sprintf(label_output, "Answer: %s", quotient);
    gtk_label_set_text(GTK_LABEL(results_payload->label_to_change), quotient);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;

    GtkWidget *btn_add;
    GtkWidget *btn_sub;
    GtkWidget *btn_mul;
    GtkWidget *btn_div;

    GtkWidget *lbl_results;

    GtkWidget *entry_top;
    GtkWidget *entry_bot;

    GtkWidget *lbl_prompt_top;
    GtkWidget *lbl_prompt_bot;

    GtkWidget *vbox;

    GtkWidget *frame_op;


    gtk_init(&argc, &argv);

    // Create the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Binary Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Create grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID (grid), 35);
    gtk_grid_set_column_spacing(GTK_GRID (grid), 35);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // create frames
    frame_op = gtk_frame_new("Operations");
    gtk_frame_set_shadow_type(GTK_FRAME(frame_op), GTK_SHADOW_IN);
    gtk_grid_attach(GTK_GRID(grid), frame_op, 0, 0, 1, 4);

    // Create buttons
    btn_add = gtk_button_new_with_label("Add");
    gtk_widget_set_tooltip_text(btn_add, "Add");

    btn_sub = gtk_button_new_with_label("Subtract");
    gtk_widget_set_tooltip_text(btn_sub, "Subtract");

    btn_mul = gtk_button_new_with_label("Multiply");
    gtk_widget_set_tooltip_text(btn_mul, "Multiply");

    btn_div = gtk_button_new_with_label("Divide");
    gtk_widget_set_tooltip_text(btn_div, "Divide");

    vbox = gtk_box_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX (vbox), btn_add, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (vbox), btn_sub, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (vbox), btn_mul, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX (vbox), btn_div, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER(frame_op), vbox);

    // Add result label
    lbl_results = gtk_label_new("Results");
    gtk_label_set_justify(GTK_LABEL(lbl_results), GTK_JUSTIFY_CENTER);
    gtk_grid_attach(GTK_GRID(grid), lbl_results, 1, 2, 2, 2);

    // Add prompts and textboxes
    lbl_prompt_top = gtk_label_new ("Enter first number:" );
    gtk_grid_attach(GTK_GRID(grid), lbl_prompt_top, 1, 0, 1, 1);
    entry_top = gtk_entry_new ();
    gtk_entry_set_max_length(GTK_ENTRY(entry_top), 16);
    gtk_grid_attach(GTK_GRID(grid), entry_top, 2, 0, 1, 1);

    lbl_prompt_bot = gtk_label_new ("Enter second number:" );
    gtk_grid_attach(GTK_GRID(grid), lbl_prompt_bot, 1, 1, 1, 1);
    entry_bot = gtk_entry_new ();
    gtk_entry_set_max_length(GTK_ENTRY(entry_bot), 16);
    gtk_grid_attach(GTK_GRID(grid), entry_bot, 2, 1, 1, 1);


    // // Signals

    struct results_payload results_data;
    results_data.entry_top = GTK_ENTRY(entry_top);
    results_data.entry_bot = GTK_ENTRY(entry_bot);
    results_data.label_to_change = lbl_results;

    // Add signal
    g_signal_connect(G_OBJECT(btn_add), "clicked",
        G_CALLBACK(add), &results_data);

    // Sub signal
    g_signal_connect(G_OBJECT(btn_sub), "clicked",
        G_CALLBACK(subtract), &results_data);

    // Mul signal
    g_signal_connect(G_OBJECT(btn_mul), "clicked",
        G_CALLBACK(multiply), &results_data);

    // Div signal
    g_signal_connect(G_OBJECT(btn_div), "clicked",
        G_CALLBACK(divide), &results_data);

    g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);


    gtk_main();

    return 0;
}