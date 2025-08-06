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
    char item1[50];
    char item2[50];
    char item3[50];
    char item4[50];
    int first, second, third, fourth;
};

// Array of structures
struct Shops m[5];

// Function prototypes
void signup();
int validate();
void login();
void cart();
void shop();
void items();
void item_order(int item);
void shop_initialize();
void Shop(int shop_choice);
void run_search_options();

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

    printf("\tEnter Password (6-12 chars, must include special chars, caps, smalls, and numbers): ");
    scanf("%s", t_password1);

    printf("\tConfirm Password : ");
    scanf("%s", t_password2);

    printf("\tEnter Your Mobile Number : ");
    scanf("%s", t_mobile);

    printf("\tEnter Your Age : ");
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
        printf("\nSignup failed. Please try again.\n\n");
    }
}

// Validation function (remains mostly the same)
int validate()
{
    for (i = 0; t_name[i] != '\0'; i++) {
        if (!((t_name[i] >= 'a' && t_name[i] <= 'z') || (t_name[i] >= 'A' && t_name[i] <= 'Z') || t_name[i] == '.' || t_name[i] == ' ')) {
            printf("\n\nPlease enter a valid Name!\n\n");
            return 0;
        }
    }
    
    count = 0;
    for (i = 0; t_email[i] != '\0'; i++) {
        if (t_email[i] == '@' || t_email[i] == '.')
            count++;
    }
    if (count < 2 || strlen(t_email) < 5) {
        printf("\n\nPlease Enter a Valid E-Mail\n\n");
        return 0;
    }
    
    if (strcmp(t_password1, t_password2) != 0) {
        printf("\n\nPassword Mismatch\n\n");
        return 0;
    }
    
    if (strlen(t_password1) < 6 || strlen(t_password1) > 12) {
        printf("\nYour Password length must be between 6 to 12 characters.\n\n");
        return 0;
    }

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
        printf("\n\nPlease Enter a strong password. It must contain at least one uppercase, lowercase, number, and special character (@, &, #, *, !, $, %%).\n\n");
        return 0;
    }

    if (t_age <= 0) {
        printf("\n\nPlease Enter a valid age\n\n");
        return 0;
    }

    if (strlen(t_mobile) == 14 && strncmp(t_mobile, "+880", 4) == 0) {
        success = 1;
    } else if (strlen(t_mobile) == 11 && strncmp(t_mobile, "01", 2) == 0) {
        success = 1;
    } else {
        printf("\n\nPlease Enter a valid Bangladeshi Mobile Number (e.g., +8801... or 01...)\n\n");
        return 0;
    }

    if (success == 1) {
        return 1;
    }
    
    return 0;
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

// Calling shop_initialize function
void shop_initialize()
{
    strcpy(m[1].shop, "Puma");
    strcpy(m[1].item1, "puma sneaker V2");
    strcpy(m[1].item2, "mens dryflex");
    strcpy(m[1].item3, "one8x puma");
    m[1].first = 3499;
    m[1].second = 2999;
    m[1].third = 5999;

    strcpy(m[2].shop, "USA");
    strcpy(m[2].item1, "US polo Tshirt");
    strcpy(m[2].item2, "Shoes");
    strcpy(m[2].item3, "shirt");
    m[2].first = 999;
    m[2].second = 2000;
    m[2].third = 1499;

    strcpy(m[3].shop, "WROGN");
    strcpy(m[3].item1, "Mens Watch");
    strcpy(m[3].item2, "mens solid jacket");
    strcpy(m[3].item3, "casual T-shirt");
    m[3].first = 2000;
    m[3].second = 2239;
    m[3].third = 799;
}

// Shop function
void shop()
{
    shop_initialize();
    
    printf("\n\nPlease Choose the Shop \n\n1) %s\n2) %s\n3) %s",
            m[1].shop, m[2].shop, m[3].shop);
    printf("\n4) Exit\n\nPlease select the shop\t");

    if (scanf("%d", &shop_choice) != 1) {
        while (getchar() != '\n');
        printf("Please Enter a valid choice\n\n");
        return;
    }
    
    if (shop_choice > 4) {
        printf("Please Enter a valid choice\n\n");
        shop();
    } else if (shop_choice == 4) {
        return;
    } else {
        Shop(shop_choice);
    }
}

// Function to display items in a shop
void Shop(int shop_choice)
{
    total = 0;
    while (1) {
        printf("\n\nList of items available in %s\n\n1) %s --> %d\n",
                m[shop_choice].shop, m[shop_choice].item1, m[shop_choice].first);
        printf("2) %s --> %d\n3) %s --> %d\n",
                m[shop_choice].item2, m[shop_choice].second, m[shop_choice].item3, m[shop_choice].third);
        printf("4) Cart\n5) Exit\n\nPlease Enter Your Choice : ");
        
        if (scanf("%d", &item_choice) != 1) {
            while (getchar() != '\n');
            printf("Please Enter a valid choice\n\n");
            continue;
        }

        if (item_choice == 1) {
            printf("Please Enter the Count of %s\t", m[shop_choice].item1);
            scanf("%d", &n);
            total = total + (n * m[shop_choice].first);
        } else if (item_choice == 2) {
            printf("Please Enter the Count of %s : ", m[shop_choice].item2);
            scanf("%d", &n);
            total = total + (n * m[shop_choice].second);
        } else if (item_choice == 3) {
            printf("Please Enter the Count of %s : ", m[shop_choice].item3);
            scanf("%d", &n);
            total = total + (n * m[shop_choice].third);
        } else if (item_choice == 4) {
            cart();
            return;
        } else if (item_choice == 5) {
            return; // Return to previous menu
        } else {
            printf("Please Enter a valid choice\n\n");
        }
    }
}

// Items function
void items()
{
    total = 0;
    shop_initialize();
    while (1) {
        printf("\n\nPlease choose the item\n\n");
        printf("1) %s\t--> %d\n2) %s\t--> %d\n", m[1].item1, m[1].first, m[1].item2, m[1].second);
        printf("3) %s\t--> %d\n4) %s\t--> %d\n", m[1].item3, m[1].third, m[2].item1, m[2].first);
        printf("5) %s\t--> %d\n6) %s\t--> %d\n", m[2].item2, m[2].second, m[2].item3, m[2].third);
        printf("7) %s\t--> %d\n8) %s\t--> %d\n", m[3].item1, m[3].first, m[3].item2, m[3].second);
        printf("9) %s\t--> %d\n10) Cart\n", m[3].item3, m[3].third);
        printf("11) Exit");
        printf("\nPlease Enter Your Choice : ");
        
        if (scanf("%d", &item) != 1) {
            while (getchar() != '\n');
            printf("Please Enter a valid choice\n\n");
            continue;
        }

        if (item > 11 || item < 1) {
            printf("Please Enter a valid choice\n\n");
        } else if (item == 10) {
            cart();
            return;
        } else if (item == 11) {
            return;
        } else {
            item_order(item);
        }
    }
}

// item_order function
void item_order(int item)
{
    if (item >= 1 && item <= 3)
        shop_id = 1;
    else if (item >= 4 && item <= 6)
        shop_id = 2;
    else
        shop_id = 3;

    int item_index = (item - 1) % 3 + 1;

    if (item_index == 1) {
        printf("Please Enter the Count of %s : ", m[shop_id].item1);
        scanf("%d", &n);
        total = total + (n * m[shop_id].first);
    } else if (item_index == 2) {
        printf("Please Enter the Count of %s : ", m[shop_id].item2);
        scanf("%d", &n);
        total = total + (n * m[shop_id].second);
    } else if (item_index == 3) {
        printf("Please Enter the Count of %s : ", m[shop_id].item3);
        scanf("%d", &n);
        total = total + (n * m[shop_id].third);
    }
}

// Cart function
void cart()
{
    char confirm_order;

    printf("\n\n\n\n\t*********************************Cart*********************************");
    printf("\n\nYour Total Order Amount is : %d\n", total);
    printf("\n\nDo you wish to order (y/n) : ");
    
    while (getchar() != '\n'); 
    scanf(" %c", &confirm_order);

    if (confirm_order == 'y' || confirm_order == 'Y') {
        printf("\n\nThank You for your Order");
        printf("\nYour item is on the way. Welcome again \n\n");
        total = 0;
        // The function will return to the previous menu
    } else if (confirm_order == 'n' || confirm_order == 'N') {
        printf("To cancel Your Order = 1\nTo Exit = 0\nSelect option : ");
        scanf("%d", &confirm);
        if (confirm == 1) {
            printf("\n\nOops! Your item is cancelled!! Exiting..\n\n");
            printf("Thank You visit again!\n");
            total = 0;
            // The function will return to the previous menu
        } else {
            printf("\n\n\t\t************Thank You******************\n\n");
            // The function will return to the previous menu
        }
    } else {
        printf("\n\nPlease Enter the correct choice\n\n");
        cart();
    }
}
