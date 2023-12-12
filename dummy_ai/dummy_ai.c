#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/connsix.h"
#include "../include/parsing.h"

char board[19][19] = {'E'};

char wbuf[10] ;	

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
		strcpy(opp_color, "black");
		first = draw_and_read("") ;
	}

	if (first == 0x0) {
		fprintf(stderr, "Error!\n") ;
		exit(EXIT_FAILURE) ;
	}
	printf("Read %s from server.\n", first) ;
	parsing_and_draw(board, first, opp_color);

	char hor1 = '\0' ;
	char hor2 = '\0' ;
	int ver1 = 0 ;
	int ver2 = 0 ;
	srand(time(0x0)) ;

	while (1) {
		for(int i=0; i<19; i++){
			for(int j=0; j<19; j++){
				printf("%c ", board[i][j]);
			}
			printf("\n");
		}
		hor1 = (rand() % 19) + 'A' ;
		ver1 = (rand() % 19) + 1 ;
			
		hor2 = (rand() % 19) + 'A' ;
		ver2 = (rand() % 19) + 1 ;
		
		snprintf(wbuf, 10, "%c%02d:%c%02d", hor1, ver1, hor2, ver2) ;
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