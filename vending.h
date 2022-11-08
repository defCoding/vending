typedef struct item {
    char name[20];
    double price;
} Item_T;

typedef struct vending_machine {
    Item_T *items[3][3]; // We can use NULL for if the item doesn't exist.
    int quantity[3][3];
    double balance;
    double earnings;
} Vending_Machine_T;

/**
 * Error Codes:
 * -1: Bad label
 * -2: No item there
 * -3: Not enough balance
 * -4: No stock
 * -5: Vending machine full
 * */

/**
 * UI Functions
 * */
void display_vm(Vending_Machine_T const *vm);
void display_user_menu();
void display_owner_menu();

/**
 * Customer Functions
 * */
void insert_money(Vending_Machine_T *vm, double money);
double get_balance(Vending_Machine_T const *vm);
double get_price(Vending_Machine_T const *vm, char *label);
int purchase(Vending_Machine_T *vm, char *label);
double return_change(Vending_Machine_T *vm);

/**
 * Owner Functions
 * */
int add_item(Vending_Machine_T *vm, char *name, double price);
int restock(Vending_Machine_T *vm, char *label, int quantity);
int remove_item(Vending_Machine_T *vm, char *label);
double get_earnings(Vending_Machine_T const *vm);
