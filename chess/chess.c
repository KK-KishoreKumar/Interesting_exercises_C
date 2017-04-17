/*Chess made in C programming language (serbian piece names). 
 K - kralj (king), 
 D - dama(queen), 
 T - top (rook), 
 L - lovac (bishop), 
 S - skakac/konj (knight), 
 P - pesak (pawn)
 Autor: Danilo Novakovic */
 
#include "chess.h"
int winner = 0;
int player = 0;
int check = 0;
int castling = 0;
int per = 1;	//if !0 then error msg will be print, else it will not.

int main () {
	int i = 0, j = 0, i1, i2, j1, j2, k, p;
	int pieces[2], enemy;
	pieces[0] = pieces[1] = 16;
	BOARD board [SIZE][SIZE];
	
	init_board(board);
	print_matrix(board);
	usage_tip();
	char buffer[MAX_BUFFER], promotion;
	do{
		i = 0;
		enemy = (player+1)%2;
		
		//Asks for command input until valid
		do {
			printf(player == 0 ? BLUE "Player 1: " CL_RESET : RED  "Player 2: "  CL_RESET);
			fgets(buffer, MAX_BUFFER, stdin);
			if(!strncmp(buffer, "/end", 4)) 
				exit(EXIT_SUCCESS);
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
		
		i1 = SIZE - (buffer[2] - '0');
		j1 = buffer[1] - 'a';
		i2 = SIZE - (buffer[4] - '0');
		j2 = buffer[3] - 'a';
		
		rm_danger(board, buffer[0], i1, j1);
	
		//[i1][j1] piece is being moved to [i2][j2] spot
		if(board[i2][j2].player == enemy ) 
			pieces[enemy]--;
		board[i2][j2].type = board[i1][j1].type;
		board[i2][j2].player = board[i1][j1].player;
		board[i2][j2].state = 1;	
		rm_piece(&board[i1][j1]);

		//castling is being done if called and conditions are fullfiled
		if(castling) {
			p = j1 < j2 ? SIZE - 1 : 0;
			k = j1 < j2 ? j2 - 1 : j2 + 1;
			board[i2][k].type = board[i2][p].type;
			board[i2][k].player = board[i2][p].player;
			board[i2][k].state = 1;
			rm_piece(&board[i2][p]);
			castling = 0;
		}
		
		print_matrix(board);
		
		promotion = 0;
		if(board[i2][j2].type == 'P' && (player == 0 ? i2 == 0 : i2 == SIZE - 1)) {
			printf("Promotion!\n");
			do {
				printf("With what piece (D/S/L/T) do you wish to exchange pawn at '%c%c': ", buffer[3], buffer[4]);
				scanf(" %c", &promotion);
			} while(promotion != 'D' && promotion != 'S' && promotion != 'L' && promotion != 'T');
			board[i2][j2].type = promotion;
			print_matrix(board);
		}
		
		update_danger(board);
		
		//Checks if king is attacked. If it is then it is either checkmate or check.
		for(i = 0; i < SIZE; i++) {
			for(j = 0; j < SIZE; j++) {
				if(board[i][j].type == 'K' && board[i][j].player != player  && board[i][j].danger[player]) {
					if(checkmate(board, i, j)) {
						printf("Checkmate!\nPlayer %d has won!\n", player+1);
						exit(EXIT_SUCCESS);
					}
					printf("Check!\nKing is under attack!\n");
					check = 1;
					goto CONT;
				}
			}
		}
		
		//Checks if it is stalemate if king isn't attacked.
		if(pieces[enemy] == 1 && !can_kmov(board, i, j, enemy)) {
			puts("Stalemate!\nIt's a draw!");
			return EXIT_SUCCESS;
		}
		
	CONT:;
		player = enemy;
	}while(!winner);
	
	printf("\nPlayer %d won!\n", winner);
	
	return EXIT_SUCCESS;
}



