#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vending.h"

void clear_buffer();
int convert_label_to_index(char *label);
char* convert_index_to_label(int i);

int main() {
    Vending_Machine_T vm = { 0 };

    int loop = 1;
    int choice = -1;
    int menu = 0; // 0 for user menu, 1 for owner
    while (loop) {
        display_vm(&vm);
        double money, ret, price;
        int quantity;
        char label[3];
        char name[20];
        if (menu == 0) {
            display_user_menu();
            printf("Select an option: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Amount: ");
                    scanf("%lf", &money);
                    insert_money(&vm, money);
                    break;
                case 2:
                    printf("Balance: %.2f\n", get_balance(&vm));
                    break;
                case 3:
                    printf("Label: ");
                    clear_buffer();
                    scanf("%2s", label);
                    ret = get_price(&vm, label);
                    if (ret == -1) {
                        puts("Invalid label.");
                    } else if (ret == -2) {
                        puts("No item there.");
                    } else {
                        printf("Price: $%.2f\n", ret);
                    }
                    break;
                case 4:
                    printf("Label: ");
                    clear_buffer();
                    scanf("%2s", label);
                    ret = purchase(&vm, label);
                    if (ret == -1) {
                        puts("Invalid label.");
                    } else if (ret == -2) {
                        puts("No item there.");
                    } else if (ret == -3) {
                        puts("You have not inserted enough money.");
                    } else if (ret == -4) {
                        puts("There is none of that item left.");
                    }
                    break;
                case 5:
                    printf("$%.2f change returned.\n", return_change(&vm));
                    break;
                case 6:
                    menu = 1;
                    break;
                case 7:
                    puts("Have a nice day!");
                    loop = 0;
                    break;
                default:
                    puts("Not a valid option. Please try again.");
            }
        } else {
            display_owner_menu();
            printf("Select an option: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Item name: ");
                    clear_buffer();
                    fgets(name, 20, stdin);
                    name[strlen(name) - 1] = '\0';
                    printf("Item price: ");
                    scanf("%lf", &price);
                    ret = add_item(&vm, name, price);
                    if (ret == -5) {
                        puts("Vending machine is full!");
                    }
                    break;
                case 2:
                    printf("Label: ");
                    clear_buffer();
                    scanf("%2s", label);
                    printf("Quantity: ");
                    scanf("%d", &quantity);
                    ret = restock(&vm, label, quantity);
                    if (ret == -1) {
                        puts("Invalid label.");
                    } else if (ret == -2) {
                        puts("There is no item there.");
                    }
                    break;
                case 3:
                    printf("Label: ");
                    clear_buffer();
                    scanf("%2s", label);
                    ret = remove_item(&vm, label);
                    if (ret == -1) {
                        puts("Invalid label.");
                    } 
                    break;
                case 4:
                    printf("Total Earnings: $%.2f\n", get_earnings(&vm));
                    break;
                case 5:
                    menu = 0;
                    break;
                default:
                    puts("Not a valid option. Please try again.");
            }
        }
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (vm.items[row][col] != NULL) {
                free(vm.items[row][col]);
            }
        }
    }
}

void clear_buffer() {
    while (getchar() != '\n');
}

void display_user_menu() {
    puts("[1] Insert Money");
    puts("[2] Check Balance");
    puts("[3] Check Price");
    puts("[4] Purchase");
    puts("[5] Return Change");
    puts("[6] Owner Menu");
    puts("[7] Exit");
}

void display_owner_menu() {
    puts("[1] Add an Item");
    puts("[2] Restock an Item");
    puts("[3] Remove an Item");
    puts("[4] Check Total Sales");
    puts("[5] Return to User Menu");
}

char* convert_index_to_label(int i) {
    int row = i / 3;
    int col = i % 3;
    char *label = (char *) malloc(3 * sizeof(char));
    sprintf(label, "%c%d", 'A' + row, col + 1);
    return label;
}

int convert_label_to_index(char *label) {
    if (strlen(label) != 2) {
        return -1;
    }
    if (label[0] < 'A' || label[0] > 'C' || label[1] < '1' || label[1] > '3') {
        return -1;
    }

    return 3 * (label[0] - 'A') + (label[1] - '1');
}

void display_vm(Vending_Machine_T const *vm) {
    for (int i = 0; i < 64; i++) {
        printf("-");
    }
    printf("\n");

    for (int row = 0; row < 3; row++) {
        printf("|");
        for (int col = 0; col < 3; col++) {
            Item_T *item = vm->items[row][col];
            printf("%-20s|", item == NULL ? "" : item->name);
        }
        printf("\n|");
        for (int col = 0; col < 3; col++) {
            Item_T *item = vm->items[row][col];
            char *label = convert_index_to_label(row * 3 + col);
            if (item == NULL) {
                printf("%-20s|", label);
            } else {
                char to_print[20];
                sprintf(to_print, "%s $%.2f (%d)", label, item->price, vm->quantity[row][col]);
                printf("%-20s|", to_print);
            }
            free(label);
        }
        printf("\n");
        for (int i = 0; i < 64; i++) {
            printf("-");
        }
        printf("\n");
    }
}

void insert_money(Vending_Machine_T *vm, double money) {
    vm->balance += money;
}

double get_balance(Vending_Machine_T const *vm) {
    return vm->balance;
}

double get_price(Vending_Machine_T const *vm, char *label) {
    int idx = convert_label_to_index(label);
    if (idx == -1) {
        return idx;
    }
    Item_T *item = vm->items[idx / 3][idx % 3];
    if (item == NULL) {
        return -2;
    }
    return item->price;
}

int purchase(Vending_Machine_T *vm, char *label) {
    int idx = convert_label_to_index(label);
    if (idx == -1) {
        return idx;
    }
    int row = idx / 3;
    int col = idx % 3;
    Item_T *item = vm->items[row][col];
    if (item == NULL) {
        return -2;
    } else if (vm->balance < item->price) {
        return -3;
    } else if (vm->quantity[row][col] == 0) {
        return -4;
    }

    vm->balance -= item->price;
    vm->earnings += item->price;
    vm->quantity[row][col]--;
    return 0;
}

double return_change(Vending_Machine_T *vm) {
    double old = vm->balance;
    vm->balance = 0;
    return old;
}

int add_item(Vending_Machine_T *vm, char *name, double price) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (vm->items[row][col] != NULL) {
                continue;
            }
            Item_T *item = (Item_T *) malloc(sizeof(Item_T));
            vm->items[row][col] = item;
            strncpy(item->name, name, 20); 
            item->price = price;
            return 0;
        }
    }

    return -5;
}

int restock(Vending_Machine_T *vm, char *label, int quantity) {
    int idx = convert_label_to_index(label);
    if (idx == -1) {
        return idx;
    }
    int row = idx / 3;
    int col = idx % 3;
    Item_T *item = vm->items[row][col];
    if (item == NULL) {
        return -2;
    }

    vm->quantity[row][col] += quantity;
    return 0;
}

int remove_item(Vending_Machine_T *vm, char *label) {
    int idx = convert_label_to_index(label);
    if (idx == -1) {
        return idx;
    }
    int row = idx / 3;
    int col = idx % 3;
    Item_T *item = vm->items[row][col];
    if (item == NULL) {
        return 0;
    }
    free(vm->items[row][col]);
    vm->items[row][col] = NULL;
    vm->quantity[row][col] = 0;
    return 0;
}

double get_earnings(Vending_Machine_T const *vm) {
    return vm->earnings;
}
