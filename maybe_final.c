#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TICKETS 4

// Function prototypes
int generate_transaction_id();
void get_current_time(char *purchase_time, int size);
void clear_screen();

void buy_ticket() {
    char station[5][20] = {"Uttara", "Motijhil", "Farmgate", "Agargaon", "Mirpur"};
    char station_codes[5] = {'1', '2', '3', '4', '5'};
    int i, depart_index = -1, arrive_index = -1;
    char depart[2], arrive[2];

    printf("\nSTATIONS:\n\n");
    for (i = 0; i < 5; i++) {
        printf("# %c - %s\n\n", station_codes[i], station[i]);
    }

    printf("Enter the departing station code: ");
    scanf(" %c", &depart[0]);
    printf("Enter the arriving station code: ");
    scanf(" %c", &arrive[0]);

    for (i = 0; i < 5; i++) {
        if (depart[0] == station_codes[i]) depart_index = i;
        if (arrive[0] == station_codes[i]) arrive_index = i;
    }

    if (depart_index == -1 || arrive_index == -1) {
        printf("Invalid station code(s).\n");
        return;
    }

    int distance = abs(depart_index - arrive_index);

    // Get current time for ticket number
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char ticketnumber[8];
    sprintf(ticketnumber, "%c%c%02d%02d", station_codes[depart_index], station_codes[arrive_index], timeinfo->tm_min, timeinfo->tm_sec);

    int num_tickets;
    do {
        printf("\nHow many tickets do you want to buy? (Maximum: %d): ", MAX_TICKETS);
        scanf("%d", &num_tickets);

        if (num_tickets > MAX_TICKETS) {
            printf("Maximum purchase limit of %d tickets exceeded. Please enter a valid quantity.\n", MAX_TICKETS);
        }
    } while (num_tickets > MAX_TICKETS);

    printf("\n\nTicket number(s): ");
    for (int j = 0; j < num_tickets; j++) {
        printf("%s ", ticketnumber);
    }

    printf("\nFrom %s To %s\n", station[depart_index], station[arrive_index]);
    printf("Distance: %d km\n", distance);
    printf("Total Cost for %d ticket(s): %d Tk\n\n\n", num_tickets, distance * 15 * num_tickets);

    FILE *ticket = fopen("ticketdb.txt", "a+");
    for (int k = 0; k < num_tickets; k++) {
        fprintf(ticket, "%s %s %s %d %d\n", station[depart_index], station[arrive_index], ticketnumber, distance, distance * 15);
    }
    fclose(ticket);

    int payment_option;
    printf("\nHow do you want to pay?\n");
    printf("1. Mobile Banking\n");
    printf("2. Online Banking\n");
    scanf("%d", &payment_option);

    switch (payment_option) {
        case 1: {
            int mobile_banking_option;
            char mobile_number[20];
            char pin_number[6];

            printf("\nChoose your mobile banking option:\n");
            printf("1. bKash\n");
            printf("2. Nagad\n");
            scanf("%d", &mobile_banking_option);

            printf("Enter your mobile number: ");
            scanf("%s", mobile_number);

            printf("Enter your PIN: ");
            scanf("%s", pin_number);

            int transaction_id = generate_transaction_id();
            char purchase_time[10];
            get_current_time(purchase_time, sizeof(purchase_time));

            if (mobile_banking_option == 1) {
                printf("\nPayment successful via bKash.\n");
            } else if (mobile_banking_option == 2) {
                printf("\nPayment successful via Nagad.\n");
            } else {
                printf("\nInvalid option for mobile banking.\n");
                return;
            }

            printf("Transaction details:\n");
            printf("Transaction ID: %d\n", transaction_id);
            printf("Purchase Time: %s\n", purchase_time);
            printf("Phone Number: %s\n", mobile_number);
            break;
        }
        case 2: {
            int online_banking_option;
            char card_number[20];
            char pin_number[6];

            printf("\nChoose your online banking option:\n");
            printf("1. VISA\n");
            printf("2. MASTER CARD\n");
            scanf("%d", &online_banking_option);

            printf("Enter your card number: ");
            scanf("%s", card_number);

            printf("Enter your PIN: ");
            scanf("%s", pin_number);

            int transaction_id = generate_transaction_id();
            char purchase_time[10];
            get_current_time(purchase_time, sizeof(purchase_time));

            if (online_banking_option == 1) {
                printf("\nPayment successful via VISA.\n");
            } else if (online_banking_option == 2) {
                printf("\nPayment successful via MASTER CARD.\n");
            } else {
                printf("\nInvalid option for online banking.\n");
                return;
            }

            printf("Transaction details:\n");
            printf("Transaction ID: %d\n", transaction_id);
            printf("Purchase Time: %s\n", purchase_time);
            printf("Card Number: %s\n", card_number);
            break;
        }
        default:
            printf("\nInvalid payment option.\n");
    }
}

void cancel_ticket() {
    char line[100], tnum[8];
    printf("ENTER TICKET NUMBER: ");
    scanf("%s", tnum);
    FILE *ticket = fopen("ticketdb.txt", "r");
    char fdepart[20], farrive[20], fnum[20];
    int fdis, fprice;
    int found = 0;

    if (ticket != NULL) {
        while (fscanf(ticket, "%s %s %s %d %d", fdepart, farrive, fnum, &fdis, &fprice) != EOF) {
            if (strcmp(fnum, tnum) == 0) {
                found = 1;
                printf("TICKET NUMBER: %s\n", fnum);
                printf("TRAVELLING FROM: %s TO: %s\n", fdepart, farrive);
                printf("DISTANCE TRAVELLED: %d KM\n", fdis);
                printf("TOTAL AMOUNT: %d TK\n", fprice);
                printf("RETURN AMOUNT: %d TK\n", fprice / 2);
                break;
            }
        }
        fclose(ticket);
    }

    if (!found) {
        printf("INVALID TICKET NUMBER\n");
    }
}

int main() {
    printf("\n\n\t\t\tWELCOME TO THE METRORAIL TICKETING SYSTEM \n\n");
    int selection;
    char continue_choice;

    do {
        printf("PRESS 1 to Buy A Ticket\n");
        printf("PRESS 2 to Cancel A Ticket\n");
        printf("PRESS 3 to Exit\n\n");
        printf("Awaiting command...\n\n");
        scanf("%d", &selection);

        switch (selection) {
            case 1:
                buy_ticket();
                break;
            case 2:
                clear_screen();
                cancel_ticket();
                break;
            case 3:
                clear_screen();
                printf("Thank You, Have a Nice Journey!\n");
                exit(0);
                break;
            default:
                printf("WRONG COMMAND\n\n");
        }

        printf("Do you want to buy or cancel another ticket? (Y/N): ");
        scanf(" %c", &continue_choice);
        clear_screen();

    } while (continue_choice == 'Y' || continue_choice == 'y');

    printf("Thank You, Have a Nice Journey!\n");
    return 0;
}

// Generate a random transaction ID
int generate_transaction_id() {
    srand(time(NULL));
    return rand() % 9000 + 1000;
}

// Get current time as HH:MM
void get_current_time(char *purchase_time, int size) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    snprintf(purchase_time, size, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
}

// Clear screen in macOS/Linux
void clear_screen() {
    system("clear");
}
