#include "chess.h"
void print_matrix(BOARD (*board)[SIZE]) {
	//printf("\33[2J");  				// cls (version 1)
	system("tput reset");			// cls ((better) version 2) - resets terminal also
	int i, j, k = 0;
	printf("\n\t    ");
	for(i = 0; i < SIZE; printf("%c ", 'a' + i++));
		
	printf("\n\t    ");
	for(i = 0; i < SIZE; i++) 
		printf("--");
	
	putchar('\n');
	for(i = 0; i < SIZE; i++) {
		printf(CL_RESET "\t%d | ", SIZE - i);
		for(j = 0; j < SIZE; j++) {
			switch(board[i][j].type) {
				case 0 : printf(k%2 == 1 ? BG_YELLOW  "  " : BG_WHITE  "  "); break;
				case 'K' : case 'L' : case 'S' : case 'D' : case 'T' : case 'P':
					printf(k%2 == 1 ? BG_YELLOW : BG_WHITE);
					printf(board[i][j].player == 1 ? RED "%c " CL_RESET 
														: BLUE  "%c " CL_RESET , board[i][j].type); break;
			}
			k = (k+1)%2;
		}
		k = (k+1)%2;
		printf(CL_RESET "| %d\n", SIZE - i);
	}
	printf("\t    ");
	for(i = 0; i < SIZE; i++) 
		printf("--");
	printf("\n\t    ");
	for(i = 0; i < SIZE; printf("%c ", 'a' + i++));
	puts("\n");
}
void init_board(BOARD (*board)[SIZE]) {
	int i, j;
	
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			board[i][j].state = 0;
			board[i][j].danger = (int *) calloc(2, sizeof(int));
		}
	}
	for(i = 0; i < SIZE; i++) {
		board[0][i].player = 1;
	}
	board[0][0].type = 'T';
	board[0][1].type = 'S';
	board[0][2].type = 'L';
	board[0][3].type = 'D';
	board[0][4].type = 'K';
	board[0][5].type = 'L';
	board[0][6].type = 'S';
	board[0][7].type = 'T';
	for(j = 0; j < SIZE; j++) {
		board[1][j].type = 'P';
		board[1][j].player = 1;
	}
	for(i = 2; i < SIZE - 2; i++) {
		for(j = 0; j < SIZE; j++) {
			board[i][j].type = 0;
			board[i][j].player = -1;
		}
	}
	for(j = 0; j < SIZE; j++) {
		board[6][j].type = 'P';
		board[6][j].player = 0;
	}
	for(i = 0; i < SIZE; i++) {
		board[7][i].player = 0;
	}
	board[7][0].type = 'T' ;
	board[7][1].type = 'S' ;
	board[7][2].type = 'L' ;
	board[7][3].type = 'D' ;
	board[7][4].type = 'K' ;
	board[7][5].type = 'L' ;
	board[7][6].type = 'S' ;
	board[7][7].type = 'T' ;
}
void usage_tip(void) {
	printf("usage example: Pa2a3 (first letter of piece, from (column/row) - to(column/row) *case sensitive*)\n");
}
//Used for debuging purpose 
void print_danger(BOARD (*board)[SIZE], int k) {
	int i, j;
	if(k == 0 || k == 1) {
		printf("Player %d:\n", k+1);
		for(i = 0; i < SIZE; i++) {
			printf("\t");
			for(j = 0; j < SIZE; j++) {
				printf("%2d ", board[i][j].danger[k]);
			}
			puts("");
		}
	}
}