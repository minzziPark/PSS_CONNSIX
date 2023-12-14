#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 19
#define INFINITY 100000

char board[BOARD_SIZE][BOARD_SIZE] =
    {
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'B', 'B', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'W', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
        {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
};

typedef struct
{
    int x;
    int y;
    int score;
} Move;

int ai_color = 0; // 0->black, 1->white

int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

char set_color(int which_color)
{
    return which_color == ai_color ? 'B' : 'W';
}

void print_board()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
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

int evaluate_board()
{
    int score = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'B')
            {
                score += evaluate_4_way(i, j, 'B');
            }
            else if (board[i][j] == 'W')
            {
                score -= evaluate_4_way(i, j, 'W');
            }
        }
    }
    return score;
}

int is_valid_move(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == 'E';
}

int evaluate_defensive() {
    int penalty = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'W') {
                int consecutiveStones = evaluate_4_way(i, j, 'W');
                if (consecutiveStones == 4) {
                    penalty += 100;
                } else if (consecutiveStones == 3) {
                    penalty += 10;  // 예시로 3개의 돌에 대한 패널티 추가
                }
            }
        }
    }
    return -penalty;
}

int heuristic(int defending) {
    int score = evaluate_board();

    if (defending) {
        int defensiveScore = evaluate_defensive();
        score += defensiveScore;
    }

    return score;
}


int minimax(int depth, int my_turn, int alpha, int beta, int defending) {
    if (depth == 0) {
        return heuristic(defending);
    }

    int best_score = my_turn ? -INFINITY : INFINITY;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(my_turn);
                int candidate_score = minimax(depth - 1, !my_turn, alpha, beta, defending);
                board[i][j] = 'E';

                if (my_turn) {
                    if (candidate_score > best_score) {
                        best_score = candidate_score;
                        alpha = candidate_score;
                    }
                } else {
                    if (candidate_score < best_score) {
                        best_score = candidate_score;
                        beta = candidate_score;
                    }
                }

                if (alpha >= beta) {
                    return best_score;
                }
            }
        }
    }

    return best_score;
}

Move find_best_position(int defending) {
    int best_score = -INFINITY;
    Move best_position = {-1, -1};

    int alpha = -INFINITY;
    int beta = INFINITY;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(ai_color);
                int candidate_score = minimax(2, 0, alpha, beta, defending); // Adjust depth as needed
                board[i][j] = 'E';

                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_position.x = i;
                    best_position.y = j;
                    best_position.score = best_score;
                }
            }
        }
    }

    return best_position;
}

int main()
{
    board[BOARD_SIZE / 2][BOARD_SIZE / 2] = 'B';
    print_board(); // Print the initial board

    Move best_move = find_best_position(0); // Find and print the best move for the AI (white)

    printf("Best Move: %c%d\n", 'A' + best_move.y, best_move.x + 1);
    printf("score: %d\n", best_move.score);
    board[best_move.x][best_move.y] = set_color(ai_color); // Make the best move for the AI

    print_board(); // Print the board after the first move

    Move best_defensive_move = find_best_position(1); // Find and print the best defensive move for the AI (white)
    printf("Best Defensive Move: %c%d\n", 'A' + best_defensive_move.y, best_defensive_move.x + 1);
    printf("score: %d\n", best_defensive_move.score);
    board[best_defensive_move.x][best_defensive_move.y] = set_color(ai_color); // Make the best defensive move for the AI

    print_board(); // Print the board after the second move

    return 0;
}
