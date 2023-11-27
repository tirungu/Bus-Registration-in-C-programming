#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Added for dynamic memory allocation

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SEATS 52
#define MAX_RESERVATIONS 50

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

User users[] = {
    {"Rahim_Njagi", "Rahim@2004"},
    {"Celine_Mwende", "Mwendee"},
    {"GrahamOsumba", "87903572"}
};

int numUsers = sizeof(users) / sizeof(User);

int authenticateUser(char* username, char* password) {
    int i;
	 for (i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;  // Authentication successful
        }
    }
    return 0;  // Authentication failed
}

typedef struct {
    char passengerName[50];
    int seatNumber;
    float fare; // Added fare field
} Reservation;

typedef struct {
    char destination[20];
    int available_seats;
    Reservation reservations[MAX_SEATS];
} BusRoute;

typedef struct {
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations; // Track the number of reservations
} BookingHistory;

// Initialize bus routes
void initializeRoutes(BusRoute routes[]) {
    strcpy(routes[0].destination, "Nairobi");
    routes[0].available_seats = MAX_SEATS;

    strcpy(routes[1].destination, "Kericho");
    routes[1].available_seats = MAX_SEATS;
    
}

// Display available seats
void displayAvailableSeats(BusRoute routes[], int numRoutes) {
    printf("Available Seats:\n");
    int i;
    for (i = 0; i < numRoutes; i++) {
        printf("To %s: %d\n", routes[i].destination, routes[i].available_seats);
    }
}

// Reserve a seat
int reserveSeat(BusRoute *route, Reservation *reservation, BookingHistory *history) {
    if (route->available_seats <= 0) {
        printf("No available seats for this route.\n");
        return 0; // Reservation failed
    }

    char name[50];
      printf("Enter your name: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0'; 

    route->reservations[MAX_SEATS - route->available_seats].seatNumber = MAX_SEATS - route->available_seats + 1;
    strcpy(route->reservations[MAX_SEATS - route->available_seats].passengerName, name);

    route->available_seats--;

    // Assign a unique reservation ID
    reservation->seatNumber = MAX_SEATS - route->available_seats;
    reservation->fare = 9000.0; // Initial fare

    // Add the reservation to the booking history
    history->reservations[history->numReservations] = *reservation;
    history->numReservations++;

    return 1; // Reservation successful
}

// Process payment for a reservation
int processPayment(Reservation *reservation, float amount) {
    // Simulate a payment process here (in reality, this would interact with a payment gateway)
    printf("Processing payment of Ksh%.2f for reservation ID %d.\n", amount, reservation->seatNumber);

    // Update the fare in the reservation
    reservation->fare = amount;

    return 0;
}

// Generate a receipt for a reservation
void generateReceipt(Reservation *reservation) {
    printf("------ Bus Reservation Receipt ------\n");
    printf("Reservation ID: %d\n", reservation->seatNumber);
    printf("Passenger Name: %s\n", reservation->passengerName);
    printf("Seat Number: %d\n", reservation->seatNumber);
    printf("Fare: Ksh%.2f\n", reservation->fare);
    printf("------------------------------------\n");
}

int main() {
    BusRoute routes[2];
    initializeRoutes(routes);
    int choice;
    int routeChoice;

    // Create a reservation
    Reservation reservation1;
    reservation1.seatNumber = 0;
    reservation1.fare = 0.00; // Initial fare

    // Create a booking history
    BookingHistory history;
    history.numReservations = 0; // Initialize the number of reservations

    printf("=== Bus Reservation System ===\n");

    // Authentication
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (authenticateUser(username, password)) {
        printf("Login successful. Welcome, %s!\n", username);

        do {
            printf("\nMenu:\n");
            printf("1. Display available seats\n");
            printf("2. Reserve a seat\n");
            printf("3. Make payment\n");
            printf("4. Generate receipt\n");
            printf("5. View Booking History\n");
            printf("6. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    displayAvailableSeats(routes, 2);
                    break;

                case 2:
                    printf("Select route (1 for Nairobi, 2 for Kericho): ");
                    scanf("%d", &routeChoice);
                    if (routeChoice == 1 || routeChoice == 2 || routeChoice == 3 || routeChoice == 4 || routeChoice == 5 || routeChoice == 6) {
                        int reservationStatus = reserveSeat(&routes[routeChoice - 1], &reservation1, &history);
                        if (reservationStatus == 1) {
                            printf("Reservation successful.\n");
                        } else {
                            printf("Reservation failed.\n");
                        }
                    } else {
                        printf("Invalid route choice.\n");
                    }
                    break;

                case 3:
                    if (reservation1.fare == 0) {
                        float amountToPay = 9000.00;
                        int paymentStatus = processPayment(&reservation1, amountToPay);
                        if (paymentStatus == 0) {
                            printf("Payment successful.\n");
                            printf("Updated fare for reservation ID %d: Ksh%.2f\n", reservation1.seatNumber, reservation1.fare);
                        } else {
                            printf("Sorry, payment failed.\n");
                        }
                    } else {
                        printf("Your payment has already been processed.\n");
                    }
                    break;

                case 4:
                    generateReceipt(&reservation1);
                    break;

                case 5:
                    printf("=== Booking History ===\n");
                    int i;
                    for (i = 0; i < history.numReservations; i++) {
                        generateReceipt(&history.reservations[i]);
                    }
                    break;

                case 6:
                    printf("Exiting program.\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 6);
    } else {
        printf("Authentication failed. Invalid username or password.\n");
    }

    return 0;
}
