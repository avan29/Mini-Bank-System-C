// Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // Common in older compilers, can be removed if not working

// Global Structure Definition for Account
struct account {
    char name[100];
    char dob[15];
    int acc_no;
    char acc_type[10];
    char address[150];
    float balance;
};

// Global File Pointer
FILE *fp;

// Function Prototypes
void menu();
void new_acc();
void view_list();
void edit();
void transact(int acc_num, int choice);
void erase();

// Main Function
int main() {
    menu();
    return 0;
}

// Function to display the main menu
void menu() {
    int choice;
    system("cls"); // Clear screen (use "clear" for Linux/macOS)
    printf("\n\n\t\t\t\t\t** MINI BANK SYSTEM **");
    printf("\n\n\t\t\t\t\t1. Create New Account");
    printf("\n\t\t\t\t\t2. Deposit Amount");
    printf("\n\t\t\t\t\t3. Withdraw Amount");
    printf("\n\t\t\t\t\t4. View All Accounts");
    printf("\n\t\t\t\t\t5. Balance Inquiry");
    printf("\n\t\t\t\t\t6. Modify Account Details");
    printf("\n\t\t\t\t\t7. Close Account");
    printf("\n\t\t\t\t\t8. Exit");

    printf("\n\n\t\t\t\t\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: new_acc(); break;
        case 2:
        case 3:
        case 5: {
            int acc_num;
            printf("\n\t\t\t\t\tEnter Account Number: ");
            scanf("%d", &acc_num);
            transact(acc_num, choice);
            break;
        }
        case 4: view_list(); break;
        case 6: edit(); break;
        case 7: erase(); break;
        case 8:
            printf("\n\n\t\t\t\t\tThank you for using the Mini Bank System!");
            exit(0);
        default:
            printf("\n\t\t\t\t\tInvalid choice. Press any key to return to the menu...");
            getch();
            menu();
    }
}

// 1. Create New Account Function
void new_acc() {
    struct account add;
    system("cls");
    fp = fopen("record.dat", "a+"); // Open file for append and read

    printf("\n\t\t\t\t\t** CREATE NEW ACCOUNT **\n");
    printf("\t\t\t\t\tEnter the Account Number: ");
    scanf("%d", &add.acc_no);
    printf("\t\t\t\t\tEnter Customer Name: ");
    fflush(stdin); // Clear input buffer
    gets(add.name);
    printf("\t\t\t\t\tEnter Date of Birth (DD/MM/YYYY): ");
    gets(add.dob);
    printf("\t\t\t\t\tEnter Address: ");
    gets(add.address);
    printf("\t\t\t\t\tEnter Account Type (Savings/Current): ");
    gets(add.acc_type);
    printf("\t\t\t\t\tEnter Initial Deposit Amount: Rs. ");
    scanf("%f", &add.balance);

    fwrite(&add, sizeof(struct account), 1, fp); // Write structure to file

    fclose(fp);
    printf("\n\t\t\t\t\tAccount Created Successfully!");
    printf("\n\t\t\t\t\tPress any key to return to the menu...");
    getch();
    menu();
}

// 4. View All Accounts Function
void view_list() {
    struct account view;
    system("cls");
    int i = 0;
    fp = fopen("record.dat", "r");
    if (fp == NULL) {
        printf("\n\t\t\t\t\tError: File not found or empty.");
        printf("\n\t\t\t\t\tPress any key to return to the menu...");
        getch();
        menu();
        return;
    }

    printf("\n\t\t\t\t\t** ALL CUSTOMER ACCOUNTS **\n\n");
    printf("Acc No.\t\tName\t\t\tBalance (Rs.)\n");
    printf("------------------------------------------------------------------\n");

    while (fread(&view, sizeof(struct account), 1, fp) == 1) {
        printf("%d\t\t%s\t\t\t%.2f\n", view.acc_no, view.name, view.balance);
        i++;
    }

    fclose(fp);
    if (i == 0) {
         printf("\n\t\t\t\t\tNo accounts found in the system.");
    }
    printf("\n\t\t\t\t\tPress any key to return to the menu...");
    getch();
    menu();
}

// 2, 3, 5. Transaction and Inquiry Function
void transact(int acc_num, int choice) {
    struct account customer;
    long int size = sizeof(struct account);
    float amount;
    int found = 0;

    system("cls");
    fp = fopen("record.dat", "r+"); // Open for read and write
    if (fp == NULL) {
        printf("\n\t\t\t\t\tError: File not found or empty.");
        getch(); menu(); return;
    }

    // Loop through all records
    while (fread(&customer, size, 1, fp) == 1) {
        if (customer.acc_no == acc_num) {
            found = 1;
            break;
        }
    }

    if (found == 1) {
        switch (choice) {
            case 2: // Deposit
                printf("\n\t\t\t\t\t** DEPOSIT **");
                printf("\n\t\t\t\t\tCurrent Balance: Rs. %.2f", customer.balance);
                printf("\n\t\t\t\t\tEnter amount to deposit: Rs. ");
                scanf("%f", &amount);
                customer.balance += amount;
                
                // Move file pointer back by one record size
                fseek(fp, -size, SEEK_CUR);
                // Overwrite the old record with the new one
                fwrite(&customer, size, 1, fp); 
                printf("\n\t\t\t\t\tDeposit Successful! New Balance: Rs. %.2f", customer.balance);
                break;
                
            case 3: // Withdraw
                printf("\n\t\t\t\t\t** WITHDRAWAL **");
                printf("\n\t\t\t\t\tCurrent Balance: Rs. %.2f", customer.balance);
                printf("\n\t\t\t\t\tEnter amount to withdraw: Rs. ");
                scanf("%f", &amount);
                
                if (customer.balance < amount) {
                    printf("\n\t\t\t\t\tERROR: Insufficient Balance!");
                } else {
                    customer.balance -= amount;
                    fseek(fp, -size, SEEK_CUR);
                    fwrite(&customer, size, 1, fp);
                    printf("\n\t\t\t\t\tWithdrawal Successful! Remaining Balance: Rs. %.2f", customer.balance);
                }
                break;
                
            case 5: // Balance Inquiry
                printf("\n\t\t\t\t\t** BALANCE INQUIRY **");
                printf("\n\t\t\t\t\tAccount Holder Name: %s", customer.name);
                printf("\n\t\t\t\t\tAccount Type: %s", customer.acc_type);
                printf("\n\t\t\t\t\tCurrent Balance: Rs. %.2f", customer.balance);
                break;
        }
    } else {
        printf("\n\t\t\t\t\tERROR: Account Number not found.");
    }

    fclose(fp);
    printf("\n\t\t\t\t\tPress any key to return to the menu...");
    getch();
    menu();
}

// 6. Modify Account Function (Similar logic to transact)
void edit() {
    // Logic: 
    // 1. Get Acc No.
    // 2. Open file in r+ mode.
    // 3. Search and find the record.
    // 4. Prompt for new details (name, address, etc.).
    // 5. Use fseek and fwrite to overwrite the old record with the modified one.
    // (Implementation omitted for brevity, but the logic is critical for the report)
    printf("\n\t\t\t\t\t** MODIFY ACCOUNT LOGIC HERE **");
    printf("\n\t\t\t\t\tPress any key to return to the menu...");
    getch();
    menu();
}

// 7. Close Account Function (Similar logic to transact, but needs a temporary file)
void erase() {
    // Logic: 
    // 1. Get Acc No.
    // 2. Open 'record.dat' in read mode and a 'temp.dat' in write mode.
    // 3. Copy all records EXCEPT the one to be deleted from 'record.dat' to 'temp.dat'.
    // 4. Close both files.
    // 5. Delete 'record.dat'.
    // 6. Rename 'temp.dat' to 'record.dat'.
    // (Implementation omitted for brevity, but the logic is critical for the report)
    printf("\n\t\t\t\t\t** CLOSE ACCOUNT LOGIC HERE **");
    printf("\n\t\t\t\t\tPress any key to return to the menu...");
    getch();
    menu();
}