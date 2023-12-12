#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/connsix.h"
#include "../include/parsing.h"

#define BOARD_SIZE 19
#define INFINITY 100000

typedef struct {
    int x;
    int y;
} Move;

char board[BOARD_SIZE][BOARD_SIZE] = {'E'};
char wbuf[10] ;	
int ai_color = 0; // 0->black, 1->white
int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

char set_color(int which_color) {
  return which_color == ai_color ? 'B' : 'W';
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
	// 휴리스틱을 사용하여 게임 상태를 빠르게 평가
	int score = evaluate_board();
	
	// 휴리스틱에 따라 평가 점수에 가중치 부여
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
					// 가지치기
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

	return best_position;
}


int
main ()
{
	char ip[20] ;
	int port = 0 ;
	char color[10] ;
	char opp_color[10];
	printf("Enter ip: ") ;
	scanf("%s", ip) ;
	printf("Enter port number: ") ;
	scanf("%d", &port) ;
	printf("Enter player color: ") ;
	scanf("%s", color) ;

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			board[i][j] = 'E';
		}
	}

	char * redstones = lets_connect(ip, port, color) ;
	parsing_redstone(board, redstones);
	if (redstones == 0x0) {
		fprintf(stderr, "Error!\n") ;
		exit(EXIT_FAILURE) ;
	}
	printf("Received %s redstones.\n", redstones) ;

	char * first ;
	if (strcmp(color, "black") == 0) {
		strcpy(opp_color, "white");
		first = draw_and_read("K10") ;
		parsing_and_draw(board, "K10", color);
	}
	else{
		ai_color = 1;
		strcpy(opp_color, "black");
		first = draw_and_read("") ;
	}

	if (first == 0x0) {
		fprintf(stderr, "Error!\n") ;
		exit(EXIT_FAILURE) ;
	}
	printf("Read %s from server.\n", first) ;
	parsing_and_draw(board, first, opp_color);

	while (1) {
		print_board();

		Move best_move1 = find_best_position();
		printf("Best Move: %c%d\n", 'A' + best_move1.y, best_move1.x + 1);

		Move best_move2 = find_best_position();
		printf("Best Move: %c%d\n", 'A' + best_move2.y, best_move2.x + 1);
		
		snprintf(wbuf, 10, "%c%02d:%c%02d", best_move1.y, best_move1.x, best_move2.y, best_move2.x);
		parsing_and_draw(board, wbuf, color);

		char * rbuf = draw_and_read(wbuf) ;
		if (rbuf == 0x0) {
			printf("Error!\n") ;
			break ;
		}
		printf("Read %s from server.\n", rbuf) ;
		parsing_and_draw(board, rbuf, opp_color);

		if (strcmp(rbuf, "WIN") == 0 || strcmp(rbuf, "LOSE") == 0 || strcmp(rbuf, "TIE") == 0)
			break ;
	}

	return 0 ;
}