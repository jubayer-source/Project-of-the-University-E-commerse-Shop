// C Program to implement Shopping Cart
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Structure for user details
struct details {
    char uname[50];
    int age;
    char password[100];
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
struct details s[100];

// Function prototypes
void signup();
void account_validate();
int validate();
void login();
void cart();
void shop();
void items();
void item_order(int item);
void shop_initialize();
void Shop(int shop_choice);

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
        scanf("%d", &choice);

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

// Signup function
void signup()
{
    printf("\n\n\t******************Welcome to Signup Page*****************\n\n");

    // Clear the input buffer before reading the name
    while (getchar() != '\n'); 
    
    printf("\tEnter Your name : ");
    fgets(t_name, sizeof(t_name), stdin);
    t_name[strcspn(t_name, "\n")] = 0; // Remove the newline character

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
        account_validate();
        login();
    } else {
        printf("\nSignup failed. Please try again.\n\n");
        main();
    }
}

// Validation function
int validate()
{
    // Name Validation
    for (i = 0; t_name[i] != '\0'; i++) {
        if (!((t_name[i] >= 'a' && t_name[i] <= 'z') || (t_name[i] >= 'A' && t_name[i] <= 'Z') || t_name[i] == '.' || t_name[i] == ' ')) {
            printf("\n\nPlease enter a valid Name!\n\n");
            return 0;
        }
    }
    
    // Email Validation
    count = 0;
    for (i = 0; t_email[i] != '\0'; i++) {
        if (t_email[i] == '@' || t_email[i] == '.')
            count++;
    }
    if (count < 2 || strlen(t_email) < 5) {
        printf("\n\nPlease Enter a Valid E-Mail\n\n");
        return 0;
    }
    
    // Password Validation
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

    // Age Validation
    if (t_age <= 0) {
        printf("\n\nPlease Enter a valid age\n\n");
        return 0;
    }

    // Mobile Number Validation (for Bangladeshi numbers)
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

// Account Validation
void account_validate()
{
    for (i = 0; i < 100; i++) {
        if (!strcmp(t_email, s[i].email)) {
            printf("\n\nAccount Already Existed. Please Login !\n\n");
            return;
        }
    }
    
    // If no account exists, create a new one
    strcpy(s[j].uname, t_name);
    s[j].age = t_age;
    strcpy(s[j].password, t_password1);
    strcpy(s[j].email, t_email);
    strcpy(s[j].mobile, t_mobile);
    j++;
    printf("\n\n\nAccount Successfully Created!\n\n");
}

// Login function
void login()
{
    printf("\n\n\t******************Welcome to Login Page********************\n\n");
    printf("\n\nLOGIN\n\n");
    printf("\t Enter Your Email: ");
    scanf("%s", t_email);
    printf("\t Enter Your Password: ");
    scanf("%s", t_password1);

    for (i = 0; i < 100; i++) {
        if (!strcmp(s[i].email, t_email)) {
            if (!strcmp(s[i].password, t_password1)) {
                printf("\n\nWelcome %s, ", s[i].uname);
                printf("You are successfully logged in\n\n");
                printf("We Provide two ways of search : \n");
                printf("1) Search By Shop\n");
                printf("2) Search by item\n");
                printf("3) Exit\n\n");
                printf("Please Enter your choice : ");
                scanf("%d", &search_choice);

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
                        main();
                        break;
                    }
                    default: {
                        printf("Please Enter a valid choice!!!\n\n");
                        break;
                    }
                }
                return;
            } else {
                printf("\n\nInvalid Password!!\n");
                printf("Please Enter the correct password\n\n");
                login();
                return;
            }
        }
    }
    printf("\n\nAccount doesn't exist, Please signup!!\n\n");
    main();
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

    scanf("%d", &shop_choice);
    if (shop_choice > 4) {
        printf("Please Enter a valid choice\n\n");
        shop();
    } else if (shop_choice == 4) {
        main();
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
        scanf("%d", &item_choice);

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
            shop();
            return;
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
        scanf("%d", &item);
        if (item > 11 || item < 1) {
            printf("Please Enter a valid choice\n\n");
        } else if (item == 10) {
            cart();
            return;
        } else if (item == 11) {
            main();
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

    if ((item % 3) == 1 || (item == 1)) {
        printf("Please Enter the Count of %s : ", m[shop_id].item1);
        scanf("%d", &n);
        total = total + (n * m[shop_id].first);
    } else if ((item % 3) == 2 || (item == 2)) {
        printf("Please Enter the Count of %s : ", m[shop_id].item2);
        scanf("%d", &n);
        total = total + (n * m[shop_id].second);
    } else if ((item % 3) == 0 || (item == 3)) {
        printf("Please Enter the Count of %s : ", m[shop_id].item3);
        scanf("%d", &n);
        total = total + (n * m[shop_id].third);
    }
}

// Cart function
// Function to implement the cart
void cart()
{
    char confirm_order;

    printf("\n\n\n\n\t*********************************Cart*********************************");
    printf("\n\nYour Total Order Amount is : %d\n", total);
    printf("\n\nDo you wish to order (y/n) : ");
    // Use scanf to read a character
    scanf(" %c", &confirm_order);

    if (confirm_order == 'y' || confirm_order == 'Y') {
        printf("\n\nThank You for your Order");
        printf("\nYour item is on the way. Welcome again \n\n");
        // Reset the total for the next user
        total = 0;
        main();
    } else if (confirm_order == 'n' || confirm_order == 'N') {
        printf("To cancel Your Order = 1\nTo Exit = 0\nSelect option : ");
        scanf("%d", &confirm);
        if (confirm == 1) {
            printf("\n\nOops! Your item is cancelled!! Exiting..\n\n");
            printf("Thank You visit again!\n");
            // Reset the total for the next user
            total = 0;
            main();
        } else {
            printf("\n\n\t\t************Thank You******************\n\n");
            login();
        }
    } else {
        printf("\n\nPlease Enter the correct choice\n\n");
        cart();
    }
}
