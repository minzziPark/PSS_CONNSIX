#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/connsix.h"
#include "../include/parsing.h"

#define BOARD_SIZE 19
#define INFINITY 100000

typedef struct {
    int x;
    int y;
} Move;

int ai_color = 1; // 0->black, 1->white
int ori_depth = 3;
int a = -999999;
int b =  999999;
int abx1,aby1,abx2,aby2;

char board[BOARD_SIZE][BOARD_SIZE];
char wbuf[10] ;	
int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

void set_possible(){
	for(int i=0; i<BOARD_SIZE; i++){
		for(int j=0; j<BOARD_SIZE; j++){
			if(board[i][j] == 'B' || board[i][j] == 'W'){
				for(int k=1; k<3; k++){
					if(j-k>=0 &&           board[ i ][j-k] == 'E') { board[ i ][j-k] = 'T'; }
					if(j-k>=0 && i+k<BOARD_SIZE  && board[i+k][j-k] == 'E') { board[i+k][j-k] = 'T'; }
					if(i+k<BOARD_SIZE  &&           board[i+k][ j ] == 'E') { board[i+k][ j ] = 'T'; }
					if(i+k<BOARD_SIZE  && j+k<BOARD_SIZE  && board[i+k][j+k] == 'E') { board[i+k][j+k] = 'T'; } 
					if(j+k<BOARD_SIZE  &&           board[ i ][j+k] == 'E') { board[ i ][j+k] = 'T'; }
					if(j+k<BOARD_SIZE  && i-k>=0 && board[i-k][j+k] == 'E') { board[i-k][j+k] = 'T'; } 
					if(i-k>=0 &&           board[i-k][ j ] == 'E') { board[i-k][ j ] = 'T'; }
					if(i-k>=0 && j-k>=0 && board[i-k][j-k] == 'E') { board[i-k][j-k] = 'T'; } 
				}
			}
		}
	}
}

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

int evaluate(int get_color){
	int s6 = 1000000,
        s5 = 4500,
        d5 = 2000,
        s4 = 4000,
        d4 = 2000,
        s3 = 1000,
        d3 = 300,
        s2 = 400,
        d2 = 100;
	
	int sum = 0;
	bool left = false, right = false;
	char enemy = 'W';
	char color;
	if(get_color == 1) {
		color = 'B';
	}else{
		color = 'W';
		enemy = 'B';
	}


	// 수직
	for(int i=0;i<BOARD_SIZE;++i)
	{
			for(int j=0;j<BOARD_SIZE;++j)
			{
					left = right = false;
					if(board[i][j] == color)
					{
							int k = 1;
							while(board[i][j+k] == color) ++k;
							if(j != 0 && board[i][j-1] != enemy) left = true;
							if(j+k != BOARD_SIZE && board[i][j+k] != enemy) right = true;
							switch(k)
							{
							case 6:
									sum += s6; 
									break;
							case 5:
									if(left && right) sum += s5; 
									else if(left || right) sum += d5; 
									break;
							case 4:
									if(left && right) sum += s4; 
									else if(left || right) sum += d4; 
									break;
							case 3:
									if(left && right) sum += s3; 
									else if(left || right) sum += d3; 
									break;
							case 2:
									if(left && right) sum += s2; 
									else if(left || right) sum += d2; 
									break;
							default:
									if(k > 6) sum += s6; 
									break;
							}
							j += k;
					}
			}
	}

	// 수평
    for(int i=0;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j][i] == color)
            {
                int k = 1;
                while(board[j+k][i] == color) ++k;
                if(j != 0 && board[j-1][i] != enemy) left = true;
                if(j+k != BOARD_SIZE && board[j+k][i] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; 
                    break;
                case 5:
                    if(left && right) sum += s5;
                    else if(left || right) sum += d5;
                    break;
                case 4:
                    if(left && right) sum += s4; 
                    else if(left || right) sum += d4;
                    break;
                case 3:
                    if(left && right) sum += s3;
                    else if(left || right) sum += d3; 
                    break;
                case 2:
                    if(left && right) sum += s2; 
                    else if(left || right) sum += d2; 
                    break;
                default:
                    if(k > 6) sum += s6;
                    break;
                }
                j += k;
            }
        }
    }

	// 왼쪽사선
    for(int i=0;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE && j+i<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j][j+i] == color)
            {
                int k = 1;
                while(board[j+k][j+i+k] == color) ++k;
                if(j != 0 && board[j-1][j+i-1] != enemy) left = true;
                if(j+i+k != BOARD_SIZE && board[j+k][j+i+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6;
                    break;
                case 5:
                    if(left && right) sum += s5; 
                    else if(left || right) sum += d5; 
                    break;
                case 4:
                    if(left && right) sum += s4; 
                    else if(left || right) sum += d4; 
                    break;
                case 3:
                    if(left && right) sum += s3; 
                    else if(left || right) sum += d3; 
                    break;
                case 2:
                    if(left && right) sum += s2; 
                    else if(left || right) sum += d2; 
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }
    // 위쪽 왼쪽 사선
    for(int i=1;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE && j+i<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j+i][j] == color)
            {
                int k = 1;
                while(board[j+i+k][j+k] == color) ++k;
                if(j != 0 && board[j+i-1][j-1] != enemy) left = true;
                if(j+i+k != BOARD_SIZE && board[j+i+k][j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; 
                    break;
                case 5:
                    if(left && right) sum += s5; 
                    else if(left || right) sum += d5; 
                    break;
                case 4:
                    if(left && right) sum += s4;
                    else if(left || right) sum += d4; 
                    break;
                case 3:
                    if(left && right) sum += s3; 
                    else if(left || right) sum += d3;
                    break;
                case 2:
                    if(left && right) sum += s2; 
                    else if(left || right) sum += d2; 
                    break;
                default:
                    if(k > 6) sum += s6;
                    break;
                }
                j += k;
            }
        }
    }

    // 위쪽 오른쪽사선
    for(int i=BOARD_SIZE-1;i>=0;--i)
    {
        for(int j=0;j<BOARD_SIZE && i-j>=0;++j)
        {
            left = right = false;
            if(board[i-j][j] == color)
            {
                int k = 1;
                while(board[i-j-k][j+k] == color) ++k;
                if(j != 0 && board[i-j+1][j-1] != enemy) left = true;
                if(i-j-k+1 != 0 && board[i-j-k][j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; 
                    break;
                case 5:
                    if(left && right) sum += s5; 
                    else if(left || right) sum += d5; 
                    break;
                case 4:
                    if(left && right) sum += s4; 
                    else if(left || right) sum += d4; 
                    break;
                case 3:
                    if(left && right) sum += s3; 
                    else if(left || right) sum += d3;
                    break;
                case 2:
                    if(left && right) sum += s2; 
                    else if(left || right) sum += d2; 
                    break;
                default:
                    if(k > 6) sum += s6;
                    break;
                }
                j += k;
            }
        }
    }
    // 위쪽 오른쪽 사선
    for(int i=1;i<BOARD_SIZE;++i)
    {
        for(int j=BOARD_SIZE-1;j>=0 && i+18-j<BOARD_SIZE;--j)
        {
            left = right = false;
            if(board[j][i+18-j] == color)
            {
                int k = 1;
                while(board[j-k][i+18-j+k] == color) ++k;
                if(j != BOARD_SIZE-1 && board[j+1][i+18-j-1] != enemy) left = true;
                if(i+18-j+k != BOARD_SIZE && board[j-k][i+18-j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; 
                    break;
                case 5:
                    if(left && right) sum += s5; 
                    else if(left || right) sum += d5; 
                    break;
                case 4:
                    if(left && right) sum += s4;
                    else if(left || right) sum += d4; 
                    break;
                case 3:
                    if(left && right) sum += s3;
                    else if(left || right) sum += d3; 
                    break;
                case 2:
                    if(left && right) sum += s2; 
                    else if(left || right) sum += d2; 
                    break;
                default:
                    if(k > 6) sum += s6; 
                    break;
                }
                j -= k;
            }
        }
    }

    // score 빼기 수직
    for(int i=0;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[i][j] == enemy)
            {
                int k = 1;
                while(board[i][j+k] == enemy) ++k;
                if(j != 0 && board[i][j-1] != color) left = true;
                if(j+k != BOARD_SIZE && board[i][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; 
                    break;
                case 5:
                    if(left && right) sum -= s5; 
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4; 
                    else if(left || right) sum -= d4; 
                    break;
                case 3:
                    if(left && right) sum -= s3;
                    else if(left || right) sum -= d3; 
                    break;
                case 2:
                    if(left && right) sum -= s2;
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6; 
                    break;
                }
                j += k;
            }
        }
    }

    // score 빼기 수평
    for(int i=0;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j][i] == enemy)
            {
                int k = 1;
                while(board[j+k][i] == enemy) ++k;
                if(j != 0 && board[j-1][i] != color) left = true;
                if(j+k != BOARD_SIZE && board[j+k][i] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6;
                    break;
                case 5:
                    if(left && right) sum -= s5; 
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4;
                    else if(left || right) sum -= d4; 
                    break;
                case 3:
                    if(left && right) sum -= s3; 
                    else if(left || right) sum -= d3; 
                    break;
                case 2:
                    if(left && right) sum -= s2; 
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6; 
                    break;
                }
                j += k;
            }
        }
    }

    // 아래쪽 오른쪽 사선 score 빼기
    for(int i=0;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE && j+i<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j][j+i] == enemy)
            {
                int k = 1;
                while(board[j+k][j+i+k] == enemy) ++k;
                if(j != 0 && board[j-1][j+i-1] != color) left = true;
                if(j+i+k != BOARD_SIZE && board[j+k][j+i+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; 
                    break;
                case 5:
                    if(left && right) sum -= s5; 
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4; 
                    else if(left || right) sum -= d4; 
                    break;
                case 3:
                    if(left && right) sum -= s3; 
                    else if(left || right) sum -= d3; 
                    break;
                case 2:
                    if(left && right) sum -= s2; 
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6;
                    break;
                }
                j += k;
            }
        }
    }
    // 위쪽 오른쪽 사선 score 빼기
    for(int i=1;i<BOARD_SIZE;++i)
    {
        for(int j=0;j<BOARD_SIZE && j+i<BOARD_SIZE;++j)
        {
            left = right = false;
            if(board[j+i][j] == enemy)
            {
                int k = 1;
                while(board[j+i+k][j+k] == enemy) ++k;
                if(j != 0 && board[j+i-1][j-1] != color) left = true;
                if(j+i+k != BOARD_SIZE && board[j+i+k][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; 
                    break;
                case 5:
                    if(left && right) sum -= s5;
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4; 
                    else if(left || right) sum -= d4; 
                    break;
                case 3:
                    if(left && right) sum -= s3; 
                    else if(left || right) sum -= d3; 
                    break;
                case 2:
                    if(left && right) sum -= s2; 
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6; 
                    break;
                }
                j += k;
            }
        }
    }

    // 위쪽 오른쪽 사선 score 빼기
    for(int i=BOARD_SIZE-1;i>=0;--i)
    {
        for(int j=0;j<BOARD_SIZE && i-j>=0;++j)
        {
            left = right = false;
            if(board[i-j][j] == enemy)
            {
                int k = 1;
                while(board[i-j-k][j+k] == enemy) ++k;
                if(j != 0 && board[i-j+1][j-1] != color) left = true;
                if(i-j-k+1 != 0 && board[i-j-k][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; 
                    break;
                case 5:
                    if(left && right) sum -= s5; 
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4; 
                    else if(left || right) sum -= d4; 
                    break;
                case 3:
                    if(left && right) sum -= s3;
                    else if(left || right) sum -= d3;
                    break;
                case 2:
                    if(left && right) sum -= s2;
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6;
                    break;
                }
                j += k;
            }
        }
    }
    // 위쪽 오른쪽 사선 score 빼기
    for(int i=1;i<BOARD_SIZE;++i)
    {
        for(int j=BOARD_SIZE-1;j>=0 && i+18-j<BOARD_SIZE;--j)
        {
            left = right = false;
            if(board[j][i+18-j] == enemy)
            {
                int k = 1;
                while(board[j-k][i+18-j+k] == enemy) ++k;
                if(j != BOARD_SIZE-1 && board[j+1][i+18-j-1] != color) left = true;
                if(i+18-j+k != BOARD_SIZE && board[j-k][i+18-j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; 
                    break;
                case 5:
                    if(left && right) sum -= s5; 
                    else if(left || right) sum -= d5; 
                    break;
                case 4:
                    if(left && right) sum -= s4; 
                    else if(left || right) sum -= d4;
                    break;
                case 3:
                    if(left && right) sum -= s3; 
                    else if(left || right) sum -= d3; 
                    break;
                case 2:
                    if(left && right) sum -= s2; 
                    else if(left || right) sum -= d2; 
                    break;
                default:
                    if(k > 6) sum -= s6; 
                    break;
                }
                j -= k;
            }
        }
    }

    return sum;
}

int alpha_beta_min(int alpha, int beta, int depth, int color){
	int enemy = 1;
	if(color == 1) enemy = 2;
	if(depth == 0) return evaluate(color);

	for(int i=0;i<BOARD_SIZE*BOARD_SIZE;++i)
	{
		if(board[i/BOARD_SIZE][i%BOARD_SIZE] == 'T'){
			for(int j=i+1;j<BOARD_SIZE*BOARD_SIZE;++j)
			{
				if(board[j/BOARD_SIZE][j%BOARD_SIZE] == 'T'){
						if(enemy == 1){
						board[i/BOARD_SIZE][i%BOARD_SIZE] = 'B';
						board[j/BOARD_SIZE][j%BOARD_SIZE] = 'B';
					}else{
						board[i/BOARD_SIZE][i%BOARD_SIZE] = 'W';
						board[j/BOARD_SIZE][j%BOARD_SIZE] = 'W';
					}
					int score = alpha_beta_min(alpha,beta,depth-1,color);
					board[i/BOARD_SIZE][i%BOARD_SIZE] = 'T';
					board[j/BOARD_SIZE][j%BOARD_SIZE] = 'T';

					if(score <= alpha) { return alpha; }
					if(score < beta) { beta = score; }
				}
			}
		}
	}

	return beta;
}

int alpha_beta_max(int alpha, int beta, int depth, int color){
	int enemy = 1;
	if(color == 1) enemy = 2;
  int maxscore = -999999999;
	if(depth == 0) return evaluate(enemy);
	
	for(int i=0; i<BOARD_SIZE*BOARD_SIZE; i++){
		if(board[i/BOARD_SIZE][i%BOARD_SIZE] == 'T'){
			for(int j=i+1; j<BOARD_SIZE*BOARD_SIZE; j++){
				if(board[j/BOARD_SIZE][j%BOARD_SIZE] == 'T'){
					if(color == 1){
					board[i/BOARD_SIZE][i%BOARD_SIZE] = 'B';
					board[j/BOARD_SIZE][j%BOARD_SIZE] = 'B';
				}else{
					board[i/BOARD_SIZE][i%BOARD_SIZE] = 'W';
					board[j/BOARD_SIZE][j%BOARD_SIZE] = 'W';
				}
				int score = alpha_beta_min(alpha,beta,depth-1,color);
				board[i/BOARD_SIZE][i%BOARD_SIZE] = 'T';
				board[j/BOARD_SIZE][j%BOARD_SIZE] = 'T';

				if(score >= beta) { return beta; }
					if(score > alpha) { alpha = score; }
					if(depth == ori_depth)
					{
						if(maxscore < score)
						{
							maxscore = score;
							abx1 = i/BOARD_SIZE;
							aby1 = i%BOARD_SIZE;
							abx2 = j/BOARD_SIZE;
							aby2 = j%BOARD_SIZE;
						}
					}
				}
			}
		}
	}

	return 1;
}


int
main ()
{
	char ip[20] ;
	int port = 0 ;
	char color[10] ;
	char opp_color[10];
	char stone;
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
		first = draw_and_read("K10");
		parsing_and_draw(board, "K10", color);
		stone = 'B';
	}
	else{
		ai_color = 0;
		strcpy(opp_color, "black");
		first = draw_and_read("");
		stone = 'W';
	}

	if (first == 0x0) {
		fprintf(stderr, "Error!\n");
		exit(EXIT_FAILURE);
	}
	printf("Read %s from server.\n", first) ;
	parsing_and_draw(board, first, opp_color);

	while (1) {
		set_possible();
		print_board();

		alpha_beta_max(a,b,ori_depth, ai_color);
		board[abx1][aby1]= stone;
		board[abx2][aby2]= stone;

		printf("%d %d %d %d\n", abx1, aby1, abx2, aby2);
		print_board();
		char c_aby1;
		char c_aby2;
		if('A'+ aby1 >= 'I') c_aby1 = 'A'+ aby1+1;
		else c_aby1 = 'A'+ aby1;
		if('A'+ aby2 >= 'I') c_aby2 = 'A'+ aby2+1;
		else c_aby2 = 'A'+ aby2;

		snprintf(wbuf, 10, "%c%02d:%c%02d", c_aby1, 19-abx1, c_aby2, 19-abx2);
		printf("%s\n", wbuf);
		

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