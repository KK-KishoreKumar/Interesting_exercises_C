/*Chess made in C programming language (serbian piece names). 
 K - kralj (king), 
 D - dama(queen), 
 T - top (rook), 
 L - lovac (bishop), 
 S - skakac/konj (knight), 
 P - pesak (pawn)
 Autor: Danilo Novakovic */
#include "list.h"
#include "chess.h"
#include "redo_undo.h"
#include "menu.h"
int winner = 0;
int player = 0;
int check = 0;
int castling = 0;
int per = 1;	//if !0 then error msg will be print, else it will not.

int main () {
	if(print_menu()) return EXIT_SUCCESS;
	
	int en_passant = 0;
	int i = 0, j = 0, i1, i2, j1, j2, k, p;
	int epi = 0, epj = 0;
	int pieces[2], enemy = player^1;
	pieces[0] = pieces[1] = 16;
	BOARD board [SIZE][SIZE];
	
	init_board(board);
	print_matrix(board);
	usage_tip();
	char buffer[MAX_BUFFER], promotion;
	
	MOVE new_move;
	NODE *pcurr = NULL;
	
	do{
		i ^= i;
		
		//Asks for command input until valid
		do {
			printf(player == 0 ? BLUE "Player 1: " CL_RESET : RED  "Player 2: "  CL_RESET);
			fgets(buffer, MAX_BUFFER, stdin);
			if(buffer[0] == '/') {
				if(!strncmp(buffer, "/end", 4) || !strncmp(buffer, "/exit",5))  {
					print_list(pcurr);
					free_danger(board);
					exit(EXIT_SUCCESS);
				}
				if(!strncmp(buffer, "/ff", 3)) {
					printf("\nPlayer %d won!\n", player == 0 ? 2 : 1);
					print_list(pcurr);
					free_danger(board);
					exit(EXIT_SUCCESS);
				}
				if(!strncmp(buffer, "/undo", 5)) {
					if(undo(board, &pcurr)) puts("error: undo unavailable.");
					else print_matrix(board);
					continue;
				}
				if(!strncmp(buffer, "/redo", 5)) {
					if(redo(board, &pcurr)) puts("error: redo unavailable");
					else print_matrix(board);
					continue;
				}
				if(!strncmp(buffer, "/help", 5)) {
					chess_rules();
					print_matrix(board);
					continue;
				}
			}
			i = syntax_check(buffer);
			if(!i) {
				usage_tip();
				continue;
			}
			i = command_check(board, buffer);
		} while(!i);
		
		new_move.i1 = i1 = SIZE - (buffer[2] - '0');
		new_move.j1 = j1 = buffer[1] - 'a';
		new_move.i2 = i2 = SIZE - (buffer[4] - '0');
		new_move.j2 = j2 = buffer[3] - 'a';
		new_move.type1 = buffer[0];
		new_move.type2 = board[i2][j2].type;
		new_move.player1 = player;
		new_move.player2 = board[i2][j2].player;
		new_move.state1 = board[i1][j1].state;
		new_move.state2 = board[i2][j2].state;
		
		list_push(&pcurr, new_move);
		
		if(winner) break;
		
		if(en_passant) {
			en_passant = 0;
			if(board[i1][j1].type == 'P'  && i2 == epi && j2 == epj) {
				rm_piece(&board[i2 == 5 ? 4 : 3][j2]);
			}
			else if(!board[epi][epj].type) board[epi][epj].state = 0;
		}
		
		if(board[i1][j1].type == 'P' && abs(i1-i2) == 2) {
			epi = i1-i2 == 2 ? i1-1 : i1+1;
			epj = j1;
			board[epi][epj].state = 1;					// i would set player, but i am afraid i'm going to allow some function to do
			en_passant = 1;								// something it shouldnt be able to. Overall changing state only is much safer.
		}
		
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
			castling ^= castling;
		}
		
		print_matrix(board);
		
		if(board[i2][j2].type == 'P' && (player == 0 ? i2 == 0 : i2 == SIZE - 1)) {
			promotion ^= promotion;
			printf("Promotion!\n");
			do {
				printf("With what piece (D/S/L/T) do you wish to exchange pawn at '%c%c': ", buffer[3], buffer[4]);
				scanf(" %c", &promotion);
			} while(promotion != 'D' && promotion != 'S' && promotion != 'L' && promotion != 'T');
			board[i2][j2].type = promotion;
			print_matrix(board);
		}
		
		rm_danger_all(board, player);
		update_danger(board);
			
		player = enemy;
		enemy = player^1;
		
		//Checks if king is attacked. If it is then it is either checkmate or check.
		for(i ^= i; i < SIZE; i++) {
			for(j ^= j; j < SIZE; j++) {
				if(board[i][j].type == 'K' && board[i][j].player == player) {
					if(board[i][j].danger[enemy]) {
						if(checkmate(board, i, j)) {
							puts("Checkmate!");
							winner = player == 0 ? 2 : 1;
						}
						else {
							printf("Check!\nKing is under attack!\n");
							check = 1;
						}
					}	//Checks if it is stalemate if king isn't attacked. (bad version, needs upgrade :P)
					else	if(stalemate(board)) {
								puts("Stalemate!\nIt's a draw!");
								print_list(pcurr);
								free_danger(board);
								return EXIT_SUCCESS;
					}
					goto CONT;
				}
			}
		}
		
CONT:;	
	}while(!winner);
	
	printf("\nPlayer %d won!\n", winner);
	print_list(pcurr);
	free_danger(board);
	
	return EXIT_SUCCESS;
}



