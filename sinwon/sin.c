#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

char board[19][19] = {"EEEEBBBEBBEEEEEEEE","EEEEEEEEEEEEEEEEEE",
"EEEEEEEEEEEEEEEEEE","EEEEEEEEWWWWWWEEEE","BEEEEEEEEEEEEEEEEE",
"EBEEEEEEEEEEEEEEEE","EEBEEEEEEEWEEEEEEE","EEEBEEEEEEEEEEEEEE",
"EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE",
"EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE",
"EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE","EEEEEEEEEEEEEEEEEE",
"EEEEEEEEEEEEEEEEEE","EEEEEEEEEEBBEEEEEE"}; 

int ai_color = 0;//0->black, 1->white


int direction_x[4] = {1,0,1,1};
int direction_y[4] = {0,1,-1,1};



char set_color(int which_color){
    if(ai_color == which_color){
        return ai_color? 'W':'B';
    }
    else{
        return ai_color? 'B':'W';
    }
}

static void print_board() {
	printf("   ABCDEFGHJKLMNOPQRST\n") ;
	for (int ver = 19; ver > 0; ver--) {
		printf("%2d ", ver) ;
		for (int hor = 0; hor < 19; hor++)
			printf("%c", board[ver-1][hor]) ;
		printf(" %d\n", ver);
	}
	printf("   ABCDEFGHJKLMNOPQRST\n") ;
}

int evaluate_4_way(int x, int y, char eval_color){
    int score = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            int nx = x+j*direction_x[i];
            int ny = y+j*direction_y[i];

            if(nx < 0 || nx > 18 || ny < 0 || ny > 18){//19X19 범위 초과
                break;
            }
            else if(board[nx][ny] != eval_color){
                break;
            }
            score++;

        }
    }
    return score;
}

int evaluate_board(){
    int score = 0;
    for(int i=0; i<19; i++){
        for(int j=0; j<19; j++){
            if(board[i][j] == 'B'){
                score += evaluate_4_way(i,j,'B');
            }
            else if(board[i][j] == 'W'){
                score -= evaluate_4_way(i,j,'W');
            }
        }
    }
    return score;
}

int minimax(int depth, int my_turn){
    int best_score = -100000;

    if(depth == 0){
        return evaluate_board();
    }

    for(int i=0; i<19; i++){
        for(int j=0; j<19; j++){
            if(board[i][j] == 'E'){
                board[i][j] = my_turn? set_color(ai_color): set_color(!ai_color);
                int candidate_score = minimax(depth-1,my_turn? 0:1);
                board[i][j] = 'E';

                if(candidate_score > best_score){
                    best_score = candidate_score;
                }
            }
        }
    }
    return best_score;
}

void find_best_position(){
    int best_score = -100000;
    int best_position[2];

    for(int i=0; i<19; i++){
        printf("i == %d\n",i);
        for(int j=0; j<19; j++){
            if(board[i][j] == 'E'){
                board[i][j] = set_color(ai_color);
                //int candidate_score = minimax(2, 1);
                int candidate_score = minimax(1, 0);
                board[i][j] = 'E';

                if(candidate_score > best_score){
                    best_score = candidate_score;
                    best_position[0] = i;
                    best_position[1] = j;
                }
            }
        }
    }

    printf("board[%d,%d]",best_position[0]+1,best_position[1]+1);
}

/*
0->E
1->B
2->W
3->R
*/
int main(){
    char color[10] = "black";

    if (strcmp(color, "black") == 0) 
		board[9][9] = 'B';
	else
		ai_color = 1;

    print_board();
    find_best_position();
    print_board();

    return 0;
}