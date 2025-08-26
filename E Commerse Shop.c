// C Program to implement Shopping Cart with Labeled File I/O
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Structure for user details
struct details {
    char uname[50];
    int age;
    char password[100]; // This will store the hashed password
    char email[100];
    char mobile[15];
};

// Structure for shop and item details
struct Shops {
    char shop[100];
    char item_name[4][50];
    int item_price[4];
    int item_qty[4];
};

// New structure for items in the cart
struct CartItem {
    char name[50];
    int price;
    int quantity;
};

// Array of structures
struct Shops m[10]; // Increased size to handle more shops
int num_shops = 0; // New variable to track the number of shops

// Array to hold the items in the user's cart
struct CartItem cart_items[100];
int cart_count = 0;

// Function prototypes
void signup();
int validate();
void login();
void cart();
void shop();
void items();
void item_order(int item);
void load_shop_data(); // New function to load data from file
void save_shop_data(); // New function to save data to file
void Shop(int shop_choice);
void run_search_options();
void generate_invoice();

// New function prototypes for file handling and hashing
void save_user_to_file(struct details user);
int is_email_exist(char* email);
void hash_password(char* password, char* hashed_password);

// Global variables
char t_name[100], t_password1[100];
char t_password2[100], t_email[100];
char t_mobile[100];
int flag = 1, i, j = 0, count = 0, caps = 0;
int Small = 0, total = 0, success = 0, special = 0, numbers = 0;
int x, choice, t_age, item_choice, n, shop_choice, search_choice, confirm, ch, item, shop_id;

// Driver code
int main()
{
    load_shop_data(); // Load data from file at the start of the program

    while (1) {
        printf("\n\n\t******************Welcome to Shop Cart*******************\n");
        printf("\n\n1)SIGNUP");
        printf("\n2)LOGIN");
        printf("\n3)EXIT");

        printf("\n\n\nEnter your choice : ");
        if (scanf("%d", &choice) != 1) {
            // Handle invalid input (non-integer)
            while (getchar() != '\n');
            printf("\n\nPlease enter a valid choice!!\n");
            continue; // Continue to the next iteration of the while loop
        }

        switch (choice) {
            case 1: {
                signup();
                break;
            }
            case 2: {
                login();
                break;
            }
            case 3: {
                printf("\n\t*************************Thank you Visit Again***********************\n\n");
                save_shop_data(); // Save the updated data before exiting
                return 0;
            }
            default: {
                printf("\n\nPlease enter a valid choice!!\n");
                break;
            }
        }
    }
}

// Simple hashing function for demonstration
void hash_password(char* password, char* hashed_password) {
    char key[] = "my_secret_key";
    int key_len = strlen(key);
    int pass_len = strlen(password);

    for (int i = 0; i < pass_len; i++) {
        hashed_password[i] = password[i] ^ key[i % key_len];
    }
    hashed_password[pass_len] = '\0';
}

// Function to check if an email already exists in the file
int is_email_exist(char* email) {
    FILE* fp = fopen("users.dat", "r");
    if (fp == NULL) {
        return 0;
    }

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "email:", 6) == 0) {
            char key[50], value[150];
            sscanf(line, "%s %[^\n]", key, value);
            if (strcmp(value, email) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found;
}

// Function to save user details to a file
void save_user_to_file(struct details user) {
    FILE* fp = fopen("users.dat", "a");
    if (fp == NULL) {
        printf("Error: Could not open user data file.\n");
        return;
    }

    char hashed_pass[100];
    hash_password(user.password, hashed_pass);

    fprintf(fp, "name: %s\n", user.uname);
    fprintf(fp, "age: %d\n", user.age);
    fprintf(fp, "password: %s\n", hashed_pass);
    fprintf(fp, "email: %s\n", user.email);
    fprintf(fp, "mobile: %s\n", user.mobile);
    fprintf(fp, "---\n");

    fclose(fp);
}

// Signup function
void signup()
{
    printf("\n\n\t******************Welcome to Signup Page*****************\n\n");

    while (getchar() != '\n');

    printf("\tEnter Your name : ");
    fgets(t_name, sizeof(t_name), stdin);
    t_name[strcspn(t_name, "\n")] = 0;

    printf("\tEnter Your Email : ");
    scanf("%s", t_email);

    printf("\tEnter Password (6-12 chars, must include special character, Uppercase, lowercase, and numbers): ");
    scanf("%s", t_password1);

    printf("\tConfirm Password : ");
    scanf("%s", t_password2);

    printf("\tEnter Your Mobile Number (01...) : ");
    scanf("%s", t_mobile);

    printf("\tEnter Your Age (10-80) : ");
    scanf("%d", &t_age);

    x = validate();
    if (x == 1) {
        if (is_email_exist(t_email)) {
            printf("\n\nAccount Already Existed. Please Login !\n\n");
        } else {
            struct details new_user;
            strcpy(new_user.uname, t_name);
            new_user.age = t_age;
            strcpy(new_user.password, t_password1);
            strcpy(new_user.email, t_email);
            strcpy(new_user.mobile, t_mobile);

            save_user_to_file(new_user);
            printf("\n\n\nAccount Successfully Created!\n\n");
        }
    } else {
        printf("\nSignup failed. Please fix the errors listed above.\n\n");
    }
}

// Validation function with comprehensive error checking
int validate()
{
    int validation_status = 1;

    // Validate name
    for (i = 0; t_name[i] != '\0'; i++) {
        if (!((t_name[i] >= 'a' && t_name[i] <= 'z') || (t_name[i] >= 'A' && t_name[i] <= 'Z') || t_name[i] == '.' || t_name[i] == ' ')) {
            printf("\n\tERROR: Please enter a valid Name! Name can only contain letters, spaces, and dots.\n");
            validation_status = 0;
            break;
        }
    }

    // Validate email
    int at_pos = -1, dot_pos = -1;
    for (i = 0; t_email[i] != '\0'; i++) {
        if (t_email[i] == '@') {
            at_pos = i;
        } else if (t_email[i] == '.') {
            dot_pos = i;
        }
    }
    if (at_pos < 1 || dot_pos < at_pos + 2 || dot_pos == strlen(t_email) - 1) {
        printf("\n\tERROR: Please enter a valid E-Mail address (e.g., example@domain.com).\n");
        validation_status = 0;
    }


    // Validate password length
    if (strlen(t_password1) < 6 || strlen(t_password1) > 12) {
        printf("\n\tERROR: Your password length must be between 6 to 12 characters.\n");
        validation_status = 0;
    }

    // Validate password match
    if (strcmp(t_password1, t_password2) != 0) {
        printf("\n\tERROR: Password Mismatch. The confirmed password does not match.\n");
        validation_status = 0;
    }

    // Validate password complexity
    caps = 0, Small = 0, numbers = 0, special = 0;
    for (i = 0; t_password1[i] != '\0'; i++) {
        if (t_password1[i] >= 'A' && t_password1[i] <= 'Z')
            caps++;
        else if (t_password1[i] >= 'a' && t_password1[i] <= 'z')
            Small++;
        else if (t_password1[i] >= '0' && t_password1[i] <= '9')
            numbers++;
        else if (t_password1[i] == '@' || t_password1[i] == '&' || t_password1[i] == '#' || t_password1[i] == '*' || t_password1[i] == '!' || t_password1[i] == '$' || t_password1[i] == '%')
            special++;
    }
    if (caps < 1 || Small < 1 || numbers < 1 || special < 1) {
        printf("\n\tERROR: Please enter a strong password. It must contain at least one uppercase, lowercase, number, and special character (@, &, #, *, !, $, %%).\n");
        validation_status = 0;
    }

    // Validate age
    if (t_age < 10 || t_age > 80) {
        printf("\n\tERROR: Please enter a valid age between 10 and 80.\n");
        validation_status = 0;
    }

    // Validate mobile number
    if (strlen(t_mobile) != 11 || strncmp(t_mobile, "01", 2) != 0) {
        printf("\n\tERROR: Please enter a valid Bangladeshi Mobile Number (e.g., 01*********). The number must be 11 digits long and start with '01'.\n");
        validation_status = 0;
    } else {
        for (i = 0; i < strlen(t_mobile); i++) {
            if (!isdigit(t_mobile[i])) {
                printf("\n\tERROR: Mobile number must contain only digits.\n");
                validation_status = 0;
                break;
            }
        }
    }

    return validation_status;
}

// Login function (fixed infinite loop)
void login()
{
    printf("\n\n\t******************Welcome to Login Page********************\n\n");
    printf("\n\nLOGIN\n\n");
    printf("\t Enter Your Email: ");
    scanf("%s", t_email);
    printf("\t Enter Your Password: ");
    scanf("%s", t_password1);

    FILE* fp = fopen("users.dat", "r");
    if (fp == NULL) {
        printf("\n\nAccount doesn't exist. Please signup!!\n\n");
        return;
    }

    char file_uname[50], file_hashed_pass[100], file_email[100];
    char line[200];
    int user_found = 0;
    char hashed_input_pass[100];

    hash_password(t_password1, hashed_input_pass);

    while (fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);

        if (strcmp(key, "name:") == 0) {
            strcpy(file_uname, value);
        } else if (strcmp(key, "password:") == 0) {
            strcpy(file_hashed_pass, value);
        } else if (strcmp(key, "email:") == 0) {
            strcpy(file_email, value);

            if (strcmp(file_email, t_email) == 0) {
                user_found = 1;
            }
        } else if (strcmp(key, "---") == 0) {
            if (user_found) {
                if (strcmp(file_hashed_pass, hashed_input_pass) == 0) {
                    printf("\n\nWelcome %s, ", file_uname);
                    printf("You are successfully logged in\n\n");
                    fclose(fp);
                    run_search_options();
                    return;
                } else {
                    printf("\n\nInvalid Password!!\n");
                    fclose(fp);
                    return; // Return to main menu
                }
            }
            user_found = 0; // Reset for next user
        }
    }
    fclose(fp);

    if (!user_found) {
        printf("\n\nAccount doesn't exist, Please signup!!\n\n");
    }
}

// New function to handle post-login search options
void run_search_options() {
    while(1) {
        printf("\n\nWe Provide two ways of search : \n");
        printf("1) Search By Shop\n");
        printf("2) Search by item\n");
        printf("3) Logout\n\n");
        printf("Please Enter your choice : ");

        if (scanf("%d", &search_choice) != 1) {
            while (getchar() != '\n');
            printf("Please Enter a valid choice!!!\n\n");
            continue;
        }

        switch (search_choice) {
            case 1: {
                shop();
                break;
            }
            case 2: {
                items();
                break;
            }
            case 3: {
                printf("\n\tYou have been logged out.\n");
                return;
            }
            default: {
                printf("Please Enter a valid choice!!!\n\n");
                break;
            }
        }
    }
}

// New function to load shop data from file
void load_shop_data() {
    FILE* fp = fopen("products.dat", "r");
    if (fp == NULL) {
        printf("products.dat not found. Creating a new file with default data.\n");
        num_shops = 3;
        strcpy(m[1].shop, "Aarong"); strcpy(m[1].item_name[1], "Cotton Panjabi"); m[1].item_price[1] = 1450; m[1].item_qty[1] = 5;
        strcpy(m[1].item_name[2], "Handloom Saree"); m[1].item_price[2] = 2200; m[1].item_qty[2] = 3;
        strcpy(m[1].item_name[3], "Leather Bag"); m[1].item_price[3] = 3200; m[1].item_qty[3] = 0;

        strcpy(m[2].shop, "Yellow"); strcpy(m[2].item_name[1], "Casual Shirt"); m[2].item_price[1] = 1250; m[2].item_qty[1] = 10;
        strcpy(m[2].item_name[2], "Jeans Pant"); m[2].item_price[2] = 1800; m[2].item_qty[2] = 7;
        strcpy(m[2].item_name[3], "Graphic Tee"); m[2].item_price[3] = 850; m[2].item_qty[3] = 2;

        strcpy(m[3].shop, "Ecstasy"); strcpy(m[3].item_name[1], "Formal Shirt"); m[3].item_price[1] = 1650; m[3].item_qty[1] = 8;
        strcpy(m[3].item_name[2], "Blazer"); m[3].item_price[2] = 4500; m[3].item_qty[2] = 4;
        strcpy(m[3].item_name[3], "Slim Fit Trousers"); m[3].item_price[3] = 1900; m[3].item_qty[3] = 6;
        save_shop_data();
        return;
    }

    char line[200];
    int shop_index = 0;
    int item_index = 0;

    while (fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        if (sscanf(line, "%[^:]: %[^\n]", key, value) == 2) {
            if (strcmp(key, "shop") == 0) {
                shop_index++;
                strcpy(m[shop_index].shop, value);
                item_index = 0;
            } else if (strncmp(key, "item", 4) == 0) {
                item_index = atoi(&key[4]);
                strcpy(m[shop_index].item_name[item_index], value);
            } else if (strncmp(key, "price", 5) == 0) {
                item_index = atoi(&key[5]);
                m[shop_index].item_price[item_index] = atoi(value);
            } else if (strncmp(key, "quantity", 8) == 0) {
                item_index = atoi(&key[8]);
                m[shop_index].item_qty[item_index] = atoi(value);
            }
        }
    }
    num_shops = shop_index;
    fclose(fp);
}

// New function to save shop data to file
void save_shop_data() {
    FILE* fp = fopen("products.dat", "w");
    if (fp == NULL) {
        printf("Error: Could not save product data.\n");
        return;
    }

    for (int i = 1; i <= num_shops; i++) {
        fprintf(fp, "shop: %s\n", m[i].shop);
        for (int j = 1; j <= 3; j++) {
            fprintf(fp, "item%d: %s\n", j, m[i].item_name[j]);
            fprintf(fp, "price%d: %d\n", j, m[i].item_price[j]);
            fprintf(fp, "quantity%d: %d\n", j, m[i].item_qty[j]);
        }
        fprintf(fp, "---\n");
    }

    fclose(fp);
}

// Shop function
void shop()
{
    printf("\n\nPlease Choose the Shop : \n\n");
    for(int i = 1; i <= num_shops; i++) {
        printf("%d) %s\n", i, m[i].shop);
    }
    printf("%d) Exit\n\nPlease select the shop\t", num_shops + 1);

    if (scanf("%d", &shop_choice) != 1) {
        while (getchar() != '\n');
        printf("Please Enter a valid choice\n\n");
        return;
    }

    if (shop_choice > num_shops + 1) {
        printf("Please Enter a valid choice\n\n");
        shop();
    } else if (shop_choice == num_shops + 1) {
        return;
    } else {
        Shop(shop_choice);
    }
}

// Function to display items in a shop
void Shop(int shop_choice)
{
    while (1) {
        printf("\n\nList of items available in %s\n\n", m[shop_choice].shop);
        printf("1) %s --> %d (Available: %d)\n", m[shop_choice].item_name[1], m[shop_choice].item_price[1], m[shop_choice].item_qty[1]);
        printf("2) %s --> %d (Available: %d)\n", m[shop_choice].item_name[2], m[shop_choice].item_price[2], m[shop_choice].item_qty[2]);
        printf("3) %s --> %d (Available: %d)\n", m[shop_choice].item_name[3], m[shop_choice].item_price[3], m[shop_choice].item_qty[3]);
        printf("4) Cart\n5) Exit\n\nPlease Enter Your Choice : ");

        if (scanf("%d", &item_choice) != 1) {
            while (getchar() != '\n');
            printf("Please Enter a valid choice\n\n");
            continue;
        }

        int quantity = 0;
        switch (item_choice) {
            case 1:
                if (m[shop_choice].item_qty[1] == 0) {
                    printf("\nSorry, '%s' is out of stock. \n\n", m[shop_choice].item_name[1]);
                    break;
                }
                printf("Please Enter the quantity (Max: %d): ", m[shop_choice].item_qty[1]);
                scanf("%d", &quantity);
                if (quantity <= 0 || quantity > m[shop_choice].item_qty[1]) {
                    printf("\nInvalid quantity. Please enter a value between 1 and %d.\n", m[shop_choice].item_qty[1]);
                } else {
                    // Add item to cart
                    strcpy(cart_items[cart_count].name, m[shop_choice].item_name[1]);
                    cart_items[cart_count].price = m[shop_choice].item_price[1];
                    cart_items[cart_count].quantity = quantity;
                    cart_count++;
                    // Decrease the stock
                    m[shop_choice].item_qty[1] -= quantity;
                    printf("\n%s added to cart!\n", m[shop_choice].item_name[1]);
                }
                break;
            case 2:
                if (m[shop_choice].item_qty[2] == 0) {
                    printf("\nSorry, '%s' is out of stock. \n\n", m[shop_choice].item_name[2]);
                    break;
                }
                printf("Please Enter the quantity (Max: %d): ", m[shop_choice].item_qty[2]);
                scanf("%d", &quantity);
                if (quantity <= 0 || quantity > m[shop_choice].item_qty[2]) {
                    printf("\nInvalid quantity. Please enter a value between 1 and %d.\n", m[shop_choice].item_qty[2]);
                } else {
                    // Add item to cart
                    strcpy(cart_items[cart_count].name, m[shop_choice].item_name[2]);
                    cart_items[cart_count].price = m[shop_choice].item_price[2];
                    cart_items[cart_count].quantity = quantity;
                    cart_count++;
                    // Decrease the stock
                    m[shop_choice].item_qty[2] -= quantity;
                    printf("\n%s added to cart!\n", m[shop_choice].item_name[2]);
                }
                break;
            case 3:
                if (m[shop_choice].item_qty[3] == 0) {
                    printf("\nSorry, '%s' is out of stock. \n\n", m[shop_choice].item_name[3]);
                    break;
                }
                printf("Please Enter the quantity (Max: %d): ", m[shop_choice].item_qty[3]);
                scanf("%d", &quantity);
                if (quantity <= 0 || quantity > m[shop_choice].item_qty[3]) {
                    printf("\nInvalid quantity. Please enter a value between 1 and %d.\n", m[shop_choice].item_qty[3]);
                } else {
                    // Add item to cart
                    strcpy(cart_items[cart_count].name, m[shop_choice].item_name[3]);
                    cart_items[cart_count].price = m[shop_choice].item_price[3];
                    cart_items[cart_count].quantity = quantity;
                    cart_count++;
                    // Decrease the stock
                    m[shop_choice].item_qty[3] -= quantity;
                    printf("\n%s added to cart!\n", m[shop_choice].item_name[3]);
                }
                break;
            case 4:
                cart();
                return;
            case 5:
                return;
            default:
                printf("Please Enter a valid choice\n\n");
                break;
        }
    }
}

// Items function
void items()
{
    while (1) {
        printf("\n\nPlease choose the item\n\n");
        for(int i = 1; i <= num_shops; i++) {
            printf("%d) %s\t--> %d (Available: %d)\n", (i-1)*3 + 1, m[i].item_name[1], m[i].item_price[1], m[i].item_qty[1]);
            printf("%d) %s\t--> %d (Available: %d)\n", (i-1)*3 + 2, m[i].item_name[2], m[i].item_price[2], m[i].item_qty[2]);
            printf("%d) %s\t--> %d (Available: %d)\n", (i-1)*3 + 3, m[i].item_name[3], m[i].item_price[3], m[i].item_qty[3]);
        }
        printf("%d) Cart\n", num_shops * 3 + 1);
        printf("%d) Exit\n", num_shops * 3 + 2);
        printf("\nPlease Enter Your Choice : ");

        if (scanf("%d", &item) != 1) {
            while (getchar() != '\n');
            printf("Please Enter a valid choice\n\n");
            continue;
        }

        if (item > num_shops * 3 + 2 || item < 1) {
            printf("Please Enter a valid choice\n\n");
        } else if (item == num_shops * 3 + 1) {
            cart();
            return;
        } else if (item == num_shops * 3 + 2) {
            return;
        } else {
            item_order(item);
        }
    }
}

// item_order function
void item_order(int item)
{
    shop_id = (item - 1) / 3 + 1;
    int item_index = (item - 1) % 3 + 1;
    int quantity = 0;

    if (m[shop_id].item_qty[item_index] == 0) {
        printf("\nSorry, '%s' is out of stock. \n\n", m[shop_id].item_name[item_index]);
        return;
    }

    printf("Please Enter the quantity (Max: %d): ", m[shop_id].item_qty[item_index]);
    scanf("%d", &quantity);
    if (quantity <= 0 || quantity > m[shop_id].item_qty[item_index]) {
        printf("\nInvalid quantity. Please enter a value between 1 and %d.\n", m[shop_id].item_qty[item_index]);
    } else {
        strcpy(cart_items[cart_count].name, m[shop_id].item_name[item_index]);
        cart_items[cart_count].price = m[shop_id].item_price[item_index];
        cart_items[cart_count].quantity = quantity;
        cart_count++;
        m[shop_id].item_qty[item_index] -= quantity;
        printf("\n%s added to cart!\n", m[shop_id].item_name[item_index]);
    }
}

// Cart function
void cart()
{
    char confirm_order;

    printf("\n\n\n\n\t*********************************Cart*********************************");

    // Display a summary of the items in the cart
    int grand_total = 0;
    printf("\n\n\t********************* Order Summary *********************\n\n");
    printf("%-25s %-10s %-10s %-10s\n", "Item Name", "Price", "Qty", "Subtotal");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < cart_count; i++) {
        int subtotal = cart_items[i].price * cart_items[i].quantity;
        printf("%-25s %-10d %-10d %-10d\n", cart_items[i].name, cart_items[i].price, cart_items[i].quantity, subtotal);
        grand_total += subtotal;
    }

    printf("----------------------------------------------------------\n");
    printf("%-45s %-10d\n", "TOTAL AMOUNT:", grand_total);
    printf("\n\t***************************************************\n\n");

    // Ask for final confirmation
    printf("\n\nDo you wish to order (y/n) : ");

    while (getchar() != '\n');
    scanf(" %c", &confirm_order);

    if (confirm_order == 'y' || confirm_order == 'Y') {
        generate_invoice(); // Call the new invoice function here
        printf("\n\nThank You for your Order");
        printf("\nYour item is on the way. Welcome again \n\n");

        // Save the updated stock to the file after a successful purchase
        save_shop_data();

        // Reset the cart for the next order
        cart_count = 0;
        total = 0;
    } else if (confirm_order == 'n' || confirm_order == 'N') {
        printf("To cancel Your Order = 1\nTo Exit = 0\nSelect option : ");
        scanf("%d", &confirm);
        if (confirm == 1) {
            printf("\n\nOops! Your item is cancelled!! Exiting..\n\n");
            printf("Thank You visit again!\n");
            cart_count = 0;
            total = 0;
        } else {
            printf("\n\n\t\t************Thank You******************\n\n");
        }
    } else {
        printf("\n\nPlease Enter the correct choice\n\n");
        cart();
    }
}

// New function to generate and display the invoice
void generate_invoice() {
    int grand_total = 0;

    printf("\n\n\t********************* INVOICE *********************\n\n");
    printf("%-25s %-10s %-10s %-10s\n", "Item Name", "Price", "Qty", "Subtotal");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < cart_count; i++) {
        int subtotal = cart_items[i].price * cart_items[i].quantity;
        printf("%-25s %-10d %-10d %-10d\n", cart_items[i].name, cart_items[i].price, cart_items[i].quantity, subtotal);
        grand_total += subtotal;
    }

    printf("----------------------------------------------------------\n");
    printf("%-45s %-10d\n", "TOTAL AMOUNT:", grand_total);
    printf("\n\t***************************************************\n\n");
}
