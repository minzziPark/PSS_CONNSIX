#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/connsix.h"

char wbuf[10] ;	

char block[10];
char win[10];
char win_possible[10];

char* possible_random_point(){
	char* point = (char*)malloc(10);

	char hor1 = (rand() % 19) + 'A' ;
	if(hor1 == 'I') hor1++;
	int ver1 = (rand() % 19) + 1 ;
		
	snprintf(point, 10, "%c%02d", hor1, ver1);

	memset(point, 0, 10);
	while(get_stone_at(point) != 'E'){
		hor1 = (rand() % 19) + 'A' ;
		if(hor1 == 'I') hor1++;
		ver1 = (rand() % 19) + 1 ;
		memset(point, 0, 10);
		snprintf(point, 10, "%c%02d", hor1, ver1);
	}

	return point;
}

void calcul_possible_position(int i, int j, char color_mine){
	/*
	i: 알파벳, j: 숫자
	*/
	int cnt_B = 0, cnt_W = 0, cnt_E = 0, cnt_R = 0;
	char hor = '\0';
	int ver = 0;
	char wbuf[10];
	char position[10];

	//왼쪽 아래방향
	memset(position, 0, 10);
	for(int k=0; k<6; k++){
		ver = j-k;
		if(i <= 7) hor = i + 'A';
		else hor = i+1 + 'A';
		memset(wbuf, 0, 10);
		snprintf(wbuf, 10, "%c%02d", hor, ver);
		char result = get_stone_at(wbuf);
		// printf("%c\n", result);
		if(result == 'B'){
			cnt_B++;
		}else if(result == 'W'){
			cnt_W++;
		}else if(result == 'E'){
			cnt_E++;
			if(cnt_E > 3) break;
			strcat(position, wbuf);
		}else if(result == 'R'){
			cnt_R++;
		}
	}
	if(color_mine == 'W'){
		if(cnt_W == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_B == 4 && cnt_E == 2){
			char num1[3], num2[3];
			strncpy(num1, position+1, 2);
			num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

			strncpy(num2, position+4, 2);
			num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
			// printf("%d\n", atoi(num1)-atoi(num2));
			if(atoi(num1)-atoi(num2) > 1)
				strcpy(block, position);
		}
	}else{
		if(cnt_B == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_W == 4 && cnt_E == 2){
			char num1[3], num2[3];
			strncpy(num1, position+1, 2);
			num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

			strncpy(num2, position+4, 2);
			num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
			// printf("%d\n", atoi(num1)-atoi(num2));
			if(atoi(num1)-atoi(num2) > 1)
				strcpy(block, position);
		}
	}	

	//오른쪽방향
	memset(position, 0, 10);
	cnt_B = 0, cnt_W = 0, cnt_E = 0, cnt_R = 0;
	for(int k=0; k<6; k++){
		if(i+k <= 7) hor = i+k + 'A';
		else hor = i+k+1 + 'A';
		ver = j;
		memset(wbuf, 0, 10);
		snprintf(wbuf, 10, "%c%02d", hor, ver);
		char result = get_stone_at(wbuf);
		if(result == 'B'){
			cnt_B++;
		}else if(result == 'W'){
			cnt_W++;
		}else if(result == 'E'){
			cnt_E++;
			if(cnt_E > 3) break;
			strcat(position, wbuf);
		}else if(result == 'R'){
			cnt_R++;
		}
	}
	if(color_mine == 'W'){
		if(cnt_W == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_B == 4 && cnt_E == 2){
			char alpha1, alpha2;
			alpha1 = position[0];
			alpha2 = position[3];
			if(alpha2-alpha1 > 1)
				strcpy(block, position);
		}
	}else{
		if(cnt_B == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_W == 4 && cnt_E == 2){
			char alpha1, alpha2;
			alpha1 = position[0];
			alpha2 = position[3];
			if(alpha2-alpha1 > 1)
				strcpy(block, position);
		}
	}	

	//대각선 오른쪽 방향
	memset(position, 0, 10);
	cnt_B = 0, cnt_W = 0, cnt_E = 0, cnt_R = 0;
	for(int k=0; k<6; k++){
		ver = j-k;
		if(i+k <= 7) hor = i+k + 'A';
		else hor = i+k+1 + 'A';
		memset(wbuf, 0, 10);
		snprintf(wbuf, 10, "%c%02d", hor, ver);
		char result = get_stone_at(wbuf);

		if(result == 'B'){
			cnt_B++;
		}else if(result == 'W'){
			cnt_W++;
		}else if(result == 'E'){
			cnt_E++;
			if(cnt_E > 3) break;
			strcat(position, wbuf);
		}else if(result == 'R'){
			cnt_R++;
		}
	}

	if(color_mine == 'W'){
		if(cnt_W == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_B == 4 && cnt_E == 2){
			char num1[3], num2[3];
			strncpy(num1, position+1, 2);
			num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

			strncpy(num2, position+4, 2);
			num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
			if(atoi(num1)-atoi(num2) > 1)
				strcpy(block, position);
		}
	}else{
		if(cnt_B == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_W == 4 && cnt_E == 2){
			char num1[3], num2[3];
			strncpy(num1, position+1, 2);
			num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

			strncpy(num2, position+4, 2);
			num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
			if(atoi(num1)-atoi(num2) > 1)
				strcpy(block, position);
		}
	}	

	//대각선 왼쪽 방향
	memset(position, 0, 10);
	cnt_B = 0, cnt_W = 0, cnt_E = 0, cnt_R = 0;
	
	for(int k=0; k<6; k++){
		ver = j-k;
		if(i+5-k <= 8) hor = i+5-k-1 + 'A';
		else {
			hor = i+5-k + 'A';			
		}
		memset(wbuf, 0, 10);
		snprintf(wbuf, 10, "%c%02d", hor, ver);
		char result = get_stone_at(wbuf);
		if(result == 'B'){
			cnt_B++;
		}else if(result == 'W'){
			cnt_W++;
		}else if(result == 'E'){
			cnt_E++;
			if(cnt_E > 3) break;
			strcat(position, wbuf);
		}else if(result == 'R'){
			cnt_R++;
		}
	}
	if(color_mine == 'W'){
		if(cnt_W == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_B == 4 && cnt_E == 2){
			char alpha1, alpha2;
			alpha1 = position[0];
			alpha2 = position[3];
			if(alpha1-alpha2 > 1){
				if(alpha1 == 'H' && alpha2 == 'J' || alpha1 == 'J' && alpha2 == 'H') ;
				else strcpy(block, position);
			}
			// printf("되나유? %s\n", position);
		}
	}else{
		if(cnt_B == 4 && cnt_E == 2){
			strcpy(win, position);
			return;
		}else if(cnt_W == 4 && cnt_E == 2){
			char alpha1, alpha2;
			alpha1 = position[0];
			alpha2 = position[3];
			if(alpha1-alpha2 > 1){
				if(alpha1 == 'H' && alpha2 == 'J' || alpha1 == 'J' && alpha2 == 'H') ;
				else strcpy(block, position);
			}
		}
	}	


	//(오른쪽 아래방향)
	if(i >= 10){
		memset(position, 0, 10);
		cnt_B = 0, cnt_W = 0, cnt_E = 0, cnt_R = 0;
		for(int k=0; k<6; k++){
			ver = j-k;
			hor = i+5+1 + 'A';
			memset(wbuf, 0, 10);
			snprintf(wbuf, 10, "%c%02d", hor, ver);
			char result = get_stone_at(wbuf);

			if(result == 'B'){
				cnt_B++;
			}else if(result == 'W'){
				cnt_W++;
			}else if(result == 'E'){
				cnt_E++;
				if(cnt_E > 3) break;
				strcat(position, wbuf);
			}else if(result == 'R'){
				cnt_R++;
			}
		}

		if(color_mine == 'W'){
			if(cnt_W == 4 && cnt_E == 2){
				strcpy(win, position);
				return;
			}else if(cnt_B == 4 && cnt_E == 2){
				char num1[3], num2[3];
				strncpy(num1, position+1, 2);
				num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

				strncpy(num2, position+4, 2);
				num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
				if(atoi(num1)-atoi(num2) > 1)
				strcpy(block, position);
			}
		}else{
			if(cnt_B == 4 && cnt_E == 2){
				strcpy(win, position);
				return;
			}else if(cnt_W == 4 && cnt_E == 2){
				char num1[3], num2[3];
				strncpy(num1, position+1, 2);
				num1[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.

				strncpy(num2, position+4, 2);
				num2[2] = '\0'; // 문자열 끝에 null 문자를 추가해야 합니다.
				if(atoi(num2)-atoi(num1) > 1)
				strcpy(block, position);
			}
		}	
	}
}

void find_block_win_position(char color_mine){
	//i: 알파벳, j: 숫자
	for(int j=19; j>=6; j--){
		for(int i=1; i<=14; i++){
			calcul_possible_position(i, j, color_mine);
			// printf("여긴되나?\n");
			if(strlen(win) != 0) return;
		}
	}

}

int
main ()
{
	char ip[20] ;
	int port = 0 ;
	char color[10] ;
	char color_mine;
	memset(block, 0, 10);
	memset(win, 0, 10);
	printf("Enter ip: ") ;
	scanf("%s", ip) ;
	printf("Enter port number: ") ;
	scanf("%d", &port) ;
	printf("Enter player color: ") ;
	scanf("%s", color) ;

	char * redstones = lets_connect(ip, port, color) ;
	if (redstones == 0x0) {
		fprintf(stderr, "Error!\n") ;
		exit(EXIT_FAILURE) ;
	}
	printf("Received %s redstones.\n", redstones) ;

	char * first ;
	if (strcmp(color, "black") == 0){ 
		first = draw_and_read("K10") ;
		color_mine = 'B';
	}	
	else{
		first = draw_and_read("") ;
		color_mine = 'W';
	}

	if (first == 0x0) {
		fprintf(stderr, "Error!\n") ;
		exit(EXIT_FAILURE) ;
	}
	printf("Read %s from server.\n", first) ;

	char hor1 = '\0' ;
	char hor2 = '\0' ;
	int ver1 = 0 ;
	int ver2 = 0 ;
	char ver1_c[2] = {};
	char ver2_c[2] = {};
	srand(time(0x0)) ;

	while (1) {
		/* 변경 위치 */
		memset(win, 0, 10);
		memset(block, 0, 10);
		find_block_win_position(color_mine);
		ver1_c[0] = '\0';
		ver2_c[0] = '\0';

		if(strlen(win) != 0){
			hor1 = win[0];
			strncpy(ver1_c, win+1, 2);
			hor2 = win[3];
			strncpy(ver2_c, win+4, 2);
			
			memset(wbuf, 0, 10);
			snprintf(wbuf, 10, "%c%c%c:%c%c%c", hor1, ver1_c[0], ver1_c[1], hor2, ver2_c[0], ver2_c[1]) ;
			// printf("%s\n",wbuf);
		}else if(strlen(block) != 0){
			// printf("여기는 되나용?\n");
			hor1 = block[0];
			strncpy(ver1_c, block+1, 2);
			hor2 = block[3];
			strncpy(ver2_c, block+4, 2);

			memset(wbuf, 0, 10);
			snprintf(wbuf, 10, "%c%c%c:%c%c%c", hor1, ver1_c[0], ver1_c[1], hor2, ver2_c[0], ver2_c[1]) ;
			// printf("%s\n",wbuf);
		}else{
			hor1 = (rand() % 19) + 'A' ;
			if(hor1 == 'I') hor1++;
			ver1 = (rand() % 19) + 1 ;
				
			hor2 = (rand() % 19) + 'A' ;
			if(hor2 == 'I') hor2++;
			ver2 = (rand() % 19) + 1 ;
			memset(wbuf, 0, 10);
			snprintf(wbuf, 10, "%c%02d:%c%02d", hor1, ver1, hor2, ver2) ;
		}

		char * rbuf = draw_and_read(wbuf) ;
		if (rbuf == 0x0) {
			printf("Error!\n") ;
			break ;
		}
		printf("Read %s from server.\n", rbuf) ;

		if (strcmp(rbuf, "WIN") == 0 || strcmp(rbuf, "LOSE") == 0 || strcmp(rbuf, "TIE") == 0)
			break ;
	}

	return 0 ;
}
