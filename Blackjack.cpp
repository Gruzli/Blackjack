#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>



// Card structure
typedef struct {
    const char *rank;
    const char *suit;
    int value;
} Card;

Card deck[DECK_SIZE];
int deckIndex = 0;

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Initialize the deck of cards
void initDeck() {
    const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "King", "Queen", "Jack", "A"};
    const char *suits[] = {"Spade", "Clover", "Heart", "Diamond"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (int u = 0; u < 4; u++) {
        for (int j = 0; j < 13; j++) {
            int index = u * 13 + j;
            deck[index].rank = ranks[j];
            deck[index].suit = suits[u];
            deck[index].value = values[j];
        }
    }
}

// Shuffle the deck of cards
void shuffleDeck() {
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = i + rand() % (DECK_SIZE - i);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Print a single card
void printCard(Card card) {
    printf("%s of %s\n", card.rank, card.suit);
}

// Print a hand of cards
void printHand(Card hand[], int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("%d. ", i + 1);
        printCard(hand[i]);
    }
}

// Deal a hand of cards
void dealHand(Card hand[], int *handSize) {
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i] = deck[deckIndex++];
    }
    *handSize = HAND_SIZE;
}

// Draw a card
void drawCard(Card hand[], int *handSize) {
    if (*handSize >= HAND_SIZE) {
        printf("Invalid draw\n");
        return;
    }
    if (deckIndex >= DECK_SIZE) {
        printf("No more cards\n");
        return;
    }
    hand[*handSize] = deck[deckIndex++];
    (*handSize)++;
}

// Throw a card
void throwCard(Card hand[], int *handSize, int cardIndex) {
    if (cardIndex < 0 || cardIndex >= *handSize) {
        printf("Invalid\n");
        return;
    }
    for (int i = cardIndex; i < *handSize - 1; i++) {
        hand[i] = hand[i + 1];
    }
    (*handSize)--;
    printf("\n\nCard is thrown\n");
}

// Calculate the value of a hand of cards
int calculateHandValue(Card hand[], int handSize) {
    int value = 0;
    int aceCount = 0;

    for (int i = 0; i < handSize; i++) {
        value += hand[i].value;
        if (hand[i].value == 11) {
            aceCount++;
        }
    }
    while (value > 21 && aceCount > 0) {
        value -= 10;
        aceCount--;
    }
    return value;
}

// Display the menu
void displayMenu() {
    puts(" ____  _            _    _            _    ");
    puts("| __ )| | __ _  ___| | _| | __ _  ___| | __");
    puts("|  _ \\| |/ _` |/ __| |/ / |/ _` |/ __| |/ /");
    puts("| |_) | | (_| | (__|   <| | (_| | (__|   < ");
    puts("|____/|_|\\__,_|\\___|_|\\_\\_|\\__,_|\\___|_|\\_\\");
    puts("                                           ");
}

// Display the rules
void displayRules() {
	displayMenu();
    puts("RULES");
    puts("1. Player can choose to Stand, Draw, or Throw.");
    puts("2. Stand ends the game and calculates the score.");
    puts("3. Throw discards a card, allowing another draw.");
    puts("4. Draw adds a card to your hand (max 3 cards).");
    puts("5. Dealer's hand is revealed when the player stands.");
    puts("6. Card values: 2-10 are face values, Kings/Queens/Jacks are 10, Aces are 11 or 1.");
    puts("7. Player and dealer can have a maximum of 3 cards.");
}

// Function to play the game
void playGame() {
    srand(time(NULL));
    initDeck();
    shuffleDeck();

    Card playerHand[HAND_SIZE];
    int playerHandSize = 0;
    Card dealerHand[HAND_SIZE];
    int dealerHandSize = 0;

    printf("Your hands are: ");
    dealHand(playerHand, &playerHandSize);
    printHand(playerHand, playerHandSize);

    bool runGame = true;
    while (runGame) {
        printf("\nChoose an option: \n1. Throw\n2. Draw\n3. Stand\nChoose: ");
        int option;
        scanf("%d", &option);

        switch (option) {
            case 1: {
                printf("Choose a card to throw (1-%d): ", playerHandSize);
                int cardIndex;
                scanf("%d", &cardIndex);
                throwCard(playerHand, &playerHandSize, cardIndex - 1);
                printHand(playerHand, playerHandSize);
                break;
            }
            case 2: {
                drawCard(playerHand, &playerHandSize);
                int playerScore = calculateHandValue(playerHand, playerHandSize);
                if (playerScore > 21) {
                    printf("Your value: %d\n", playerScore);
                    printf("Draw! Both Player and Dealer exceed 21.\n");
                    runGame = false;
                } else if (playerHandSize > HAND_SIZE) {
                    printf("Ending Unlocked: Disqualified!!!\n");
                    runGame = false;
                }
                printHand(playerHand, playerHandSize);
                break;
            }
            case 3: {
                printf("The dealer's hand:\n");
                dealHand(dealerHand, &dealerHandSize);
                printHand(dealerHand, dealerHandSize);

                int playerScore = calculateHandValue(playerHand, playerHandSize);
                int dealerScore = calculateHandValue(dealerHand, dealerHandSize);

                printf("Your value: %d\n", playerScore);
                printf("Dealer value: %d\n", dealerScore);

                if (playerScore > 21 && dealerScore > 21) {
                    printf("Draw! Both Player and Dealer exceed 21.\n");
                } else if (playerScore == dealerScore) {
                    printf("Draw! Both Player and Dealer have the same score.\n");
                } else if (playerScore > 21) {
                    printf("Dealer Wins! Unlocked Ending: Unlucky bro..\n");
                } else if (dealerScore > 21) {
                    printf("You Win! Unlocked Ending: Beat the dealer fairly\n");
                } else if (playerScore == 21) {
                    printf("You Win! Unlocked Ending: Lucky One\n");
                } else if (playerScore > dealerScore) {
                    printf("You Win! Unlocked Ending: Beat the dealer fairly\n");
                } else if (playerScore < dealerScore) {
                    printf("Dealer Wins! Unlocked Ending: Lose The Fight yet Lost The Battle As Well..\n");
                }
                runGame = false;
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int main() {
    bool exitGame = false;

    while (!exitGame) {
        clearScreen();
        displayMenu();
        printf("\n1. Play Game\n2. View Rules\n3. Quit\nChoose an option: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                clearScreen();
                playGame();
                printf("\nPress Enter to return to the main menu...");
                getchar(); // Consume the newline character left by scanf
                getchar(); // Wait for Enter key
                break;
            case 2:
                clearScreen();
                displayRules();
                printf("\nPress Enter to return to the main menu...");
                getchar(); // Consume the newline character left by scanf
                getchar(); // Wait for Enter key
                break;
            case 3:
                exitGame = true;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    printf("Thank you for playing!\n");
    return 0;
}

