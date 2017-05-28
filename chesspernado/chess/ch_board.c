/*
	Chess in C programming language.
    Copyright (C) 2017  Danilo Novaković

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "chess.h"
void boardcpy(BOARD (*b1)[SIZE], BOARD (*b2)[SIZE]) {
	int i, j;
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			b1[i][j].type = b2[i][j].type;
			b1[i][j].player = b2[i][j].player;
			b1[i][j].state = b2[i][j].state;
			b1[i][j].danger[0] = b2[i][j].danger[0];
			b1[i][j].danger[1] = b2[i][j].danger [ 1];
		}
	}
}
void board_free(BOARD (*b1)[SIZE]) {
	free_danger(b1);
	free(b1);
}
void free_danger(BOARD (*b1)[SIZE]) {
	int i, j;
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			free(b1[i][j].danger);
		}
	}
}
void init_board(BOARD (*board)[SIZE]) {
	int i, j;

	for(i ^= i; i < SIZE; i++) {
		board[0][i].player = board[1][i].player = 1;
		board[1][i].type = board[6][i].type = 'P';
		board[6][i].player = board[7][i].player = 0;	
		
		for(j ^= j; j < SIZE; j++) {
			board[i][j].state = 0;
			board[i][j].danger = (int *) calloc(2, sizeof(int));
		}
	}
	board[0][0].type = board[0][7].type = board[7][0].type = board[7][7].type =  'R' ;
	board[0][1].type = board[0][6].type = board[7][1].type = board[7][6].type = 'N' ;
	board[0][2].type = board[0][5].type = board[7][2].type = board[7][5].type = 'B' ;
	board[0][3].type = board[7][3].type = 'Q' ;
	board[0][4].type = board[7][4].type = 'K' ;
	
	for(i = 2; i < SIZE - 2; i++) {
		for(j ^= j; j < SIZE; j++) {
			board[i][j].type = 0;
			board[i][j].player = -1;
		}
	}

}
void print_matrix(BOARD (*board)[SIZE]) {
	//printf("\33[2J");  				// cls (version 1)
	system("tput reset");			// cls ((better) version 2) - resets terminal also
	int i, j, k = 0;
	printf("\n\t    ");
	for(i ^= i; i < SIZE; printf("%c ", 'a' + i++));
		
	printf("\n\t    ");
	for(i ^= i; i < SIZE; i++) 
		printf("--");
	
	putchar('\n');
	for(i ^= i; i < SIZE; i++) {
		printf(CL_RESET "\t%d | ", SIZE - i);
		for(j ^= j; j < SIZE; j++) {
			switch(board[i][j].type) {
				case 0 : printf((k&1) == 1 ? BG_YELLOW  "  " : BG_WHITE  "  "); break;
				case 'K' : case 'B' : case 'N' : case 'Q' : case 'R' : case 'P':
					printf((k&1) == 1 ? BG_YELLOW : BG_WHITE);
					printf(board[i][j].player == 1 ? RED "%c " CL_RESET : BLUE  "%c " CL_RESET , board[i][j].type); break;
			}
			k ^= 1;
		}
		k ^= 1;
		printf(CL_RESET "| %d\n", SIZE - i);
	}
	printf("\t    ");
	for(i ^= i; i < SIZE; i++) 
		printf("--");
	printf("\n\t    ");
	for(i ^= i; i < SIZE; printf("%c ", 'a' + i++));
	puts("\n");
}

void usage_tip(void) {
	printf("Usage ex.:  \x1b[31;1mP\x1b[35;1ma2\x1b[32;1ma4\x1b[0m" 
	" (\x1b[31;1mtype\x1b[0m, \x1b[35;1mfrom\x1b[0m (column/row) - \x1b[32;1mto\x1b[0m(column/row))\n\x1b[33;1m*CASE SENSITIVE*\x1b[0m\n");
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
void print_state(BOARD (*board)[SIZE]) {
	int i, j;
	for(i = 0; i < SIZE; i++) {
		printf("\t");
		for(j = 0; j < SIZE; j++) {
			printf("%2d ", board[i][j].state);
		}
		puts("");
	}
	
}