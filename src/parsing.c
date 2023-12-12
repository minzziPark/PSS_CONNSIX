#include <stdio.h>
#include <string.h>

void parsing_redstone(char (*board)[19], char * redstone){
	char col1_c = '\0';
	char col2_c = '\0';
	char col3_c = '\0';
	char col4_c = '\0';
	char col5_c = '\0';
	int col1 = -1, col2 = -1, col3 = -1, col4 = -1, col5 = -1;
	int row1 = -1, row2 = -1, row3 = -1, row4 = -1, row5 = -1;

	int result = sscanf(redstone, "%c%2d:%c%2d:%c%2d:%c%2d:%c%2d", &col1_c, &row1, &col2_c, &row2, &col3_c, &row3, &col4_c, &row4, &col5_c, &row5);

	printf("%d\n", result);
	if (result >= 10){
		if ('a' <= col5_c && col5_c <= 'h') {
			col5 = col5_c - 'a' ;
		} else if ('A' <= col5_c && col5_c <= 'H') {
			col5 = col5_c - 'A' ;
		} else if ('j' <= col5_c && col5_c <= 't') {
			col5 = col5_c - 'a' - 1 ;
		} else if ('J' <= col5_c && col5_c <= 'T') {
			col5 = col5_c - 'A' - 1 ;
		}
		row5 = 19-row5;
		board[row5][col5] = 'R';
	}
	if (result >= 8){
		if ('a' <= col4_c && col4_c <= 'h') {
			col4 = col4_c - 'a' ;
		} else if ('A' <= col4_c && col4_c <= 'H') {
			col4 = col4_c - 'A' ;
		} else if ('j' <= col4_c && col4_c <= 't') {
			col4 = col4_c - 'a' - 1 ;
		} else if ('J' <= col4_c && col4_c <= 'T') {
			col4 = col4_c - 'A' - 1 ;
		}
		row4 = 19-row4;
		board[row4][col4] = 'R';
	}
	if (result >= 6){
		if ('a' <= col3_c && col3_c <= 'h') {
			col3 = col3_c - 'a' ;
		} else if ('A' <= col3_c && col3_c <= 'H') {
			col3 = col3_c - 'A' ;
		} else if ('j' <= col3_c && col3_c <= 't') {
			col3 = col3_c - 'a' - 1 ;
		} else if ('J' <= col3_c && col3_c <= 'T') {
			col3 = col3_c - 'A' - 1 ;
		}
		row3 = 19-row3;
		board[row3][col3] = 'R';
	}
	if (result >= 4){
		if ('a' <= col2_c && col2_c <= 'h') {
			col2 = col2_c - 'a' ;
		} else if ('A' <= col2_c && col2_c <= 'H') {
			col2 = col2_c - 'A' ;
		} else if ('j' <= col2_c && col2_c <= 't') {
			col2 = col2_c - 'a' - 1 ;
		} else if ('J' <= col2_c && col2_c <= 'T') {
			col2 = col2_c - 'A' - 1 ;
		}
		row2 = 19-row2;
		board[row2][col2] = 'R';
	}
	if (result >= 2){
		if ('a' <= col1_c && col1_c <= 'h') {
			col1 = col1_c - 'a' ;
		} else if ('A' <= col1_c && col1_c <= 'H') {
			col1 = col1_c - 'A' ;
		} else if ('j' <= col1_c && col1_c <= 't') {
			col1 = col1_c - 'a' - 1 ;
		} else if ('J' <= col1_c && col1_c <= 'T') {
			col1 = col1_c - 'A' - 1 ;
		}
		row1 = 19-row1;
		board[row1][col1] = 'R';
	}
}

void parsing_and_draw(char (*board)[19], char * stone, char * color){
	char col1_c = '\0';
	char col2_c = '\0';
	int col1 = -1, col2 = -1;
	int row1 = -1, row2 = -1;

	if (sscanf(stone, "%c%2d:%c%2d", &col1_c, &row1, &col2_c, &row2) == 4){
		if ('a' <= col2_c && col2_c <= 'h') {
			col2 = col2_c - 'a' ;
		} else if ('A' <= col2_c && col2_c <= 'H') {
			col2 = col2_c - 'A' ;
		} else if ('j' <= col2_c && col2_c <= 't') {
			col2 = col2_c - 'a' - 1 ;
		} else if ('J' <= col2_c && col2_c <= 'T') {
			col2 = col2_c - 'A' - 1 ;
		}
		row2 = 19-row2;
		if(strcmp(color, "black") == 0){
			board[row2][col2] = 'B';
		}else{
			board[row2][col2] = 'W';
		}
	}

	if ('a' <= col1_c && col1_c <= 'h') {
			col1 = col1_c - 'a' ;
		} else if ('A' <= col1_c && col1_c <= 'H') {
			col1 = col1_c - 'A' ;
		} else if ('j' <= col1_c && col1_c <= 't') {
			col1 = col1_c - 'a' - 1 ;
		} else if ('J' <= col1_c && col1_c <= 'T') {
			col1 = col1_c - 'A' - 1 ;
		}
		row1 = 19-row1;
		if(strcmp(color, "black") == 0){
			board[row1][col1] = 'B';
		}else{
			board[row1][col1] = 'W';
		}

}
