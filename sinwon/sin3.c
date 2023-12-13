#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    {'E','E','E','E','B','E','E','E','E','E','B','E','E','E','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','W','E','W','E','E','E','E','E'},
    {'E','E','E','E','B','B','E','E','E','E','E','E','W','W','E','E','E','E','E'},
    {'E','E','E','E','B','E','E','E','E','E','E','E','E','W','E','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','W','E','E','E','E'},
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','W','E','E','E'},
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
		int score;
} Move;

int ai_color = 0; // 0->black, 1->white
int ori_depth = 2;
int a = -999999;
int b =  999999;
int abx1,aby1,abx2,aby2;

int direction_x[4] = {1, 0, 1, 1};
int direction_y[4] = {0, 1, -1, 1};

int defensive_score1 = 0;
int defensive_score2 = 0;
int attack_score1 = 0;
int attack_score2 = 0;

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


	// "|"
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
									sum += s6; //�s��
									break;
							case 5:
									if(left && right) sum += s5; //����
									else if(left || right) sum += d5; //����
									break;
							case 4:
									if(left && right) sum += s4; //���|
									else if(left || right) sum += d4; //���|
									break;
							case 3:
									if(left && right) sum += s3; //���T
									else if(left || right) sum += d3; //���T
									break;
							case 2:
									if(left && right) sum += s2; //���G
									else if(left || right) sum += d2; //���G
									break;
							default:
									if(k > 6) sum += s6; //�s��
									break;
							}
							j += k;
					}
			}
	}

	// "-"
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
                    sum += s6; //�s��
                    break;
                case 5:
                    if(left && right) sum += s5; //����
                    else if(left || right) sum += d5; //����
                    break;
                case 4:
                    if(left && right) sum += s4; //���|
                    else if(left || right) sum += d4; //���|
                    break;
                case 3:
                    if(left && right) sum += s3; //���T
                    else if(left || right) sum += d3; //���T
                    break;
                case 2:
                    if(left && right) sum += s2; //���G
                    else if(left || right) sum += d2; //���G
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }

	// "\" bot part
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
                    sum += s6; //�s��
                    break;
                case 5:
                    if(left && right) sum += s5; //����
                    else if(left || right) sum += d5; //����
                    break;
                case 4:
                    if(left && right) sum += s4; //���|
                    else if(left || right) sum += d4; //���|
                    break;
                case 3:
                    if(left && right) sum += s3; //���T
                    else if(left || right) sum += d3; //���T
                    break;
                case 2:
                    if(left && right) sum += s2; //���G
                    else if(left || right) sum += d2; //���G
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
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
                    sum += s6; //�s��
                    break;
                case 5:
                    if(left && right) sum += s5; //����
                    else if(left || right) sum += d5; //����
                    break;
                case 4:
                    if(left && right) sum += s4; //���|
                    else if(left || right) sum += d4; //���|
                    break;
                case 3:
                    if(left && right) sum += s3; //���T
                    else if(left || right) sum += d3; //���T
                    break;
                case 2:
                    if(left && right) sum += s2; //���G
                    else if(left || right) sum += d2; //���G
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }

    // "\" top part
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
                    sum += s6; //�s��
                    break;
                case 5:
                    if(left && right) sum += s5; //����
                    else if(left || right) sum += d5; //����
                    break;
                case 4:
                    if(left && right) sum += s4; //���|
                    else if(left || right) sum += d4; //���|
                    break;
                case 3:
                    if(left && right) sum += s3; //���T
                    else if(left || right) sum += d3; //���T
                    break;
                case 2:
                    if(left && right) sum += s2; //���G
                    else if(left || right) sum += d2; //���G
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
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
                    sum += s6; //�s��
                    break;
                case 5:
                    if(left && right) sum += s5; //����
                    else if(left || right) sum += d5; //����
                    break;
                case 4:
                    if(left && right) sum += s4; //���|
                    else if(left || right) sum += d4; //���|
                    break;
                case 3:
                    if(left && right) sum += s3; //���T
                    else if(left || right) sum += d3; //���T
                    break;
                case 2:
                    if(left && right) sum += s2; //���G
                    else if(left || right) sum += d2; //���G
                    break;
                default:
                    if(k > 6) sum += s6; //�s��
                    break;
                }
                j -= k;
            }
        }
    }

    // "|"
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }

    // "-"
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }

    // "\" bot part
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }

    // "\" top part
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
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
                    sum -= s6; //�s��
                    break;
                case 5:
                    if(left && right) sum -= s5; //����
                    else if(left || right) sum -= d5; //����
                    break;
                case 4:
                    if(left && right) sum -= s4; //���|
                    else if(left || right) sum -= d4; //���|
                    break;
                case 3:
                    if(left && right) sum -= s3; //���T
                    else if(left || right) sum -= d3; //���T
                    break;
                case 2:
                    if(left && right) sum -= s2; //���G
                    else if(left || right) sum -= d2; //���G
                    break;
                default:
                    if(k > 6) sum -= s6; //�s��
                    break;
                }
                j -= k;
            }
        }
    }

    return sum;
}

int is_valid_move(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == 'E';
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

int main() {
		board[BOARD_SIZE / 2][BOARD_SIZE / 2] = 'B';
		set_possible();
    print_board(); 

		alpha_beta_max(a,b,ori_depth,1);
		board[abx1][aby1]='B';
		board[abx2][aby2]='B';
		printf("%d %d %d %d\n", abx1, aby1, abx2, aby2);
		print_board(); 
		
		
    return 0;
}
