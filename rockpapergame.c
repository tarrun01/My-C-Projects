#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Clear screen using ANSI escape codes (works on macOS Terminal)
void clear_screen() {
    printf("\033[2J\033[H");
}

void print_menu() {
    printf("====== Rock - Paper - Scissors ======\n");
    printf("1. Rock\n");
    printf("2. Paper\n");
    printf("3. Scissors\n");
    printf("0. Exit\n");
    printf("Choose your move: ");
}

const char* move_name(int move) {
    switch (move) {
        case 1: return "Rock";
        case 2: return "Paper";
        case 3: return "Scissors";
        default: return "Unknown";
    }
}

int main() {
    int player, computer;
    int player_score = 0, computer_score = 0, draws = 0;

    // Seed random number generator
    srand((unsigned int)time(NULL));

    while (1) {
        clear_screen();
        printf("Score -> You: %d | Computer: %d | Draws: %d\n\n",
               player_score, computer_score, draws);

        print_menu();

        if (scanf("%d", &player) != 1) {
            // Invalid input (e.g. letter), clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input! Press Enter to try again...");
            getchar();
            continue;
        }

        if (player == 0) {
            clear_screen();
            printf("Final Score -> You: %d | Computer: %d | Draws: %d\n",
                   player_score, computer_score, draws);
            printf("Thanks for playing!\n");
            break;
        }

        if (player < 1 || player > 3) {
            printf("Please choose between 0 and 3. Press Enter to continue...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            continue;
        }

        // Computer chooses randomly between 1 and 3
        computer = (rand() % 3) + 1;

        clear_screen();
        printf("You chose:      %s\n", move_name(player));
        printf("Computer chose: %s\n\n", move_name(computer));

        // Decide result
        if (player == computer) {
            printf("Result: It's a draw!\n");
            draws++;
        } else if ((player == 1 && computer == 3) ||  // Rock beats Scissors
                   (player == 2 && computer == 1) ||  // Paper beats Rock
                   (player == 3 && computer == 2)) {  // Scissors beats Paper
            printf("Result: You win this round!\n");
            player_score++;
        } else {
            printf("Result: Computer wins this round!\n");
            computer_score++;
        }

        printf("\nPress Enter to play again...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // clear leftover '\n'
        getchar();
    }

    return 0;
}