/*Chess made in C programming language (serbian piece names). 
 K - kralj (king), D - dama(queen), T - top (rook), L - lovac (bishop), S - skakac/konj (knight), P - pesak (pawn)
 Autor: Danilo Novakovic */
#include "chess.h"
int winner = 0;
int player = 0;
int check = 0;
int castling = 0;
int per = 1;	//if !0 then error msg will be print, else it will not.

int main () {
	int i = 0, j = 0;
	BOARD board [SIZE][SIZE];
	
	init_board(board);
	print_matrix(board);
	usage_tip();
	char buffer[MAX_BUFFER], promotion;
	do{
		i = 0;
		do {
			printf(player == 0 ? BLUE "Player 1: " CL_RESET : RED  "Player 2: "  CL_RESET);
			fgets(buffer, MAX_BUFFER, stdin);
			if(!strncmp(buffer, "/ff", 3)) {
				printf("\nPlayer %d won!\n", player == 0 ? 2 : 1);
				exit(EXIT_SUCCESS);
			}
			i = syntax_check(buffer);
			if(!i) {
				usage_tip();
				continue;
			}
			i = command_check(board, buffer);
		} while(!i);
		print_matrix(board);
		
		promotion = 0;
		i = SIZE - (buffer[4] - '0');
		j = buffer[3] - 'a';
		if(board[i][j].type == 'P' && (player == 0 ? i == 0 : i == SIZE - 1)) {
			printf("Promotion!\n");
			do {
				printf("With what piece (D/S/L/T) do you wish to exchange pawn at '%c%c': ", buffer[3], buffer[4]);
				scanf(" %c", &promotion);
			} while(promotion != 'D' && promotion != 'S' && promotion != 'L' && promotion != 'T');
			board[i][j].type = promotion;
			print_matrix(board);
		}
		update_danger(board);
		player = (player+1)%2;
	}while(!winner);
	
	printf("\nPlayer %d won!\n", winner);
	
	return EXIT_SUCCESS;
}



