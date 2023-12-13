#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 19
#define INFINITY 100000

char board[BOARD_SIZE][BOARD_SIZE] = 
{
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','W','E','E','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','E','W','E','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','E','E','W','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','E','E','E','W','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},
};

typedef struct {
    int x;
    int y;
} Move;

int ai_color = 0; // 0->black, 1->white

int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

int defensive_score1 = 0;
int defensive_score2 = 0;
int attack_score1 = 0;
int attack_score2 = 0;

char set_color(int which_color) { // W
    return which_color == ai_color ? 'B' : 'W'; // W == B false -> W
}

void print_board() {
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
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

int heuristic(int depth) {
    int score = evaluate_board();
    return score + depth;
}

int minimax(int depth, int my_turn, int alpha, int beta) { 
    if (depth == 0) {
        return heuristic(depth);
    }

    int best_score = my_turn ? -INFINITY : INFINITY;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(my_turn); 
                int candidate_score = minimax(depth - 1, !my_turn, alpha, beta); 
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

Move find_best_position() {
    int best_score = -INFINITY;
    Move best_position = {-1, -1};

    int alpha = -INFINITY;
    int beta = INFINITY;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(ai_color);
                int candidate_score = minimax(2, 0, alpha, beta); // Adjust depth as needed
                board[i][j] = 'E';

                if (candidate_score > best_score) {
                    best_score = candidate_score;
                    best_position.x = i;
                    best_position.y = j;
                }
            }
        }
    }
		printf("best score: %d\n", abs(best_score));

    return best_position;
}

Move find_defensive_position() {
    int best_score = INFINITY;
    Move best_position = {-1, -1};

    int alpha = -INFINITY;
    int beta = INFINITY;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_valid_move(i, j)) {
                board[i][j] = set_color(!ai_color); // 이걸 하면 ai_color = B, !ai_color = W, 반환값 = W
                int candidate_score = minimax(2, 1, alpha, beta);
                board[i][j] = 'E';

                if (candidate_score < best_score) { 
                    best_score = candidate_score;
                    best_position.x = i;
                    best_position.y = j; 
                }
            }
        }
    }
		printf("defensive best score: %d\n", abs(best_score));

    return best_position;
}

int main() {
		board[BOARD_SIZE / 2][BOARD_SIZE / 2] = 'B';
    print_board(); 

		//공격공격
		printf("공격1 ");
		Move best_move1 = find_best_position(); 

    printf("Best Move: %c%d\n", 'A' + best_move1.y, best_move1.x + 1);
    board[best_move1.x][best_move1.y] = set_color(ai_color); 

    print_board(); 

		printf("공격2 ");
    Move best_move2 = find_best_position(); 
    printf("Best Defensive Move: %c%d\n", 'A' + best_move2.y, best_move2.x + 1);
    board[best_move2.x][best_move2.y] = set_color(ai_color); 

    print_board(); 

		//reset
		board[best_move2.x][best_move2.y] = 'E';

		//공격수비
		printf("수비2 ");
		best_move2 = find_defensive_position(); 
    printf("Best Defensive Move: %c%d\n", 'A' + best_move2.y, best_move2.x + 1);
    board[best_move2.x][best_move2.y] = set_color(ai_color); 

		print_board(); 

		//reset
		board[best_move2.x][best_move2.y] = 'E';
		board[best_move1.x][best_move1.y] = 'E';

		//수비수비
		printf("수비1 ");
		Move best_defensive_move1 = find_defensive_position(); 

    printf("Best Move: %c%d\n", 'A' + best_defensive_move1.y, best_defensive_move1.x + 1);
    board[best_defensive_move1.x][best_defensive_move1.y] = set_color(ai_color); 

    print_board(); 

		printf("수비2 ");
    Move best_defensive_move2 = find_defensive_position(); 
    printf("Best Defensive Move: %c%d\n", 'A' + best_defensive_move2.y, best_defensive_move2.x + 1);
    board[best_defensive_move2.x][best_defensive_move2.y] = set_color(ai_color); 

    print_board(); 

		//reset
		board[best_defensive_move2.x][best_defensive_move2.y] = 'E';

		//수비공격
		printf("공격2 ");
    best_defensive_move2 = find_defensive_position(); 
    printf("Best Defensive Move: %c%d\n", 'A' + best_defensive_move2.y, best_defensive_move2.x + 1);
    board[best_defensive_move2.x][best_defensive_move2.y] = set_color(ai_color); 

    print_board(); 

    return 0;
}
