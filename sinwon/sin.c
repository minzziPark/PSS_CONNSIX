#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 19

char board[BOARD_SIZE][BOARD_SIZE];

typedef struct {
    int x;
    int y;
} Move;

int ai_color = 0; // 0->black, 1->white

int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

char set_color(int which_color) {
    return which_color == ai_color ? 'B' : 'W';
}

void print_board() {
    printf("   ABCDEFGHJKLMNOPQRST\n");
    for (int ver = BOARD_SIZE; ver > 0; ver--) {
        printf("%2d ", ver);
        for (int hor = 0; hor < BOARD_SIZE; hor++)
            printf("%c", board[ver - 1][hor]);
        printf(" %d\n", ver);
    }
    printf("   ABCDEFGHJKLMNOPQRST\n");
}

int evaluate_4_way(int x, int y, char eval_color) {
    int score = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            int nx = x + j * direction_x[i];
            int ny = y + j * direction_y[i];

            if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) {
                break;
            } else if (board[nx][ny] != eval_color) {
                break;
            }
            score++;
        }
    }
    return score;
}

int evaluate_board() {
    int score = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'B') {
                score += evaluate_4_way(i, j, 'B');
            } else if (board[i][j] == 'W') {
                score -= evaluate_4_way(i, j, 'W');
            }
        }
    }
    return score;
}

int is_valid_move(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == 'E';
}

int minimax(int depth, int my_turn) {
    if (depth == 0) {
        return evaluate_board();
    }

    int best_score = my_turn ? -100000 : 100000;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(my_turn);
                int candidate_score = minimax(depth - 1, !my_turn);
                board[i][j] = 'E';

                if (my_turn && candidate_score > best_score) {
                    best_score = candidate_score;
                } else if (!my_turn && candidate_score < best_score) {
                    best_score = candidate_score;
                }
            }
        }
    }

    return best_score;
}

Move find_best_position() {
    int best_score = -100000;
    Move best_position = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(ai_color);
                int candidate_score = minimax(2, 0); // Adjust depth as needed
                board[i][j] = 'E';

                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_position.x = i;
                    best_position.y = j;
                }
            }
        }
    }

    return best_position;
}

int main() {
    // Initialize the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = 'E';
        }
    }

    // Set an initial move (black at the center)
    board[BOARD_SIZE / 2][BOARD_SIZE / 2] = 'B';

    // Print the initial board
    print_board();

    // Find and print the best move for the AI (white)
    Move best_move = find_best_position();
    printf("Best Move: %c%d\n", 'A' + best_move.y, best_move.x + 1);

    return 0;
}
