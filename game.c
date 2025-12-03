#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

void clear_screen_fast() {
    printf("\033[H\033[J");
}

// Enable RAW mode (no Enter needed, non-blocking input)
void enableRawMode() {
    struct termios t;

    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);   // disable canonical mode & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    // Non-blocking read
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

// Disable RAW mode on exit
void disableRawMode() {
    struct termios t;

    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Obstacle structure
typedef struct {
    int lane;
    int step;
} Obstacle;

int main() {
    srand(time(0));
    enableRawMode();

    while (1) {   // GAME RESTART LOOP
        int x = 1;       // player position
        int score = 0;
        int gameOver = 0;

        const int obstacleCount = 3;
        Obstacle obs[3];

        for (int i = 0; i < obstacleCount; i++) {
            obs[i].lane = rand() % 3;
            obs[i].step = rand() % 10;
        }

        while (!gameOver) {

            // NON-BLOCKING INPUT
            char ch;
            if (read(STDIN_FILENO, &ch, 1) == 1) {
                if (ch == 'a' && x > 0) x--;
                if (ch == 'd' && x < 2) x++;
            }

            // DRAW SCENE
            clear_screen_fast();
            printf("|--- --- ---|\n");

            for (int r = 0; r < 10; r++) {
                int printed = 0;

                for (int i = 0; i < obstacleCount; i++) {
                    if (obs[i].step == r) {
                        if (obs[i].lane == 0) printf("| *         |\n");
                        if (obs[i].lane == 1) printf("|     *     |\n");
                        if (obs[i].lane == 2) printf("|         * |\n");
                        printed = 1;
                        break;
                    }
                }

                if (!printed) printf("|           |\n");
            }

            // PLAYER DRAW
            if (x == 0) printf("| ^         |\n");
            if (x == 1) printf("|     ^     |\n");
            if (x == 2) printf("|         ^ |\n");

            printf("\nScore: %d\n", score);

            // COLLISION CHECK
            for (int i = 0; i < obstacleCount; i++) {
                if (obs[i].step == 10 && obs[i].lane == x) {
                    gameOver = 1;
                }
            }

            if (gameOver) break;

            // MOVE OBSTACLES
            for (int i = 0; i < obstacleCount; i++) {
                obs[i].step++;
                if (obs[i].step > 10) {
                    obs[i].step = 0;
                    obs[i].lane = rand() % 3;
                    score++;
                }
            }

            usleep(300000);  // 300 ms
        }

        // GAME OVER SCREEN
        clear_screen_fast();
        printf("\n\n===== GAME OVER =====\n");
        printf("Final Score: %d\n", score);
        printf("Play Again? (y/n):\n");
        // Now wait for a single valid key
        char again = 0;
        
        while (again != 'y' && again != 'n') {
            read(STDIN_FILENO, &again, 1);
        }

        if (again == 'n') break;
    }

    disableRawMode();
    printf("\nThanks for playing!\n");
    return 0;
}