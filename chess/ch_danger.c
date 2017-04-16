#include "chess.h"
void update_danger(BOARD (*board)[SIZE]) {
	int i, j, p;
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			p = board[i][j].player;
			switch(board[i][j].type) {
				case 'P': update_pawn(board, i, j, p); break;
				case 'S': update_knight(board, i, j, p); break;
			}
		}
	}
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			if(board[i][j].type == 'K' && board[i][j].player != player  
												&& board[i][j].danger[player] != 0) {
				if(checkmate(board, i, j)) {
					//print_danger(board, player);
					printf("Checkmate!\nPlayer %d has won!\n", player+1);
					exit(EXIT_SUCCESS);
				}
				printf("Check!\nKing is under attack!\n");
				check = 1;
				goto CONT;
			}
		}
	}
CONT:;
	//puts("after move:");
	//print_danger(board, player);
}
void update_pawn(BOARD (*board)[SIZE], int i, int j, int p) {
	if(p) {
		if(i+1 < SIZE) {
			if(j+1 < SIZE) board[i+1][j+1].danger[p] = 'P';
			if(j-1 >= 0) board[i+1][j-1].danger[p] = 'P';
		}
	}
	else {
		if(i-1 >= 0) {
			if(j+1 < SIZE) board[i-1][j+1].danger[p] = 'P';
			if(j-1 >= 0) board[i-1][j-1].danger[p] = 'P';
		}
	}
}
void update_knight(BOARD (*board)[SIZE], int i, int j, int p) {
	if(i+1 < SIZE) {
		if(j+2<SIZE) board[i+1][j+2].danger[p] = 'S';
		if(j-2>=0) board[i+1][j-2].danger[p] = 'S';
	}
	if(i-1 >= 0) {
		if(j+2<SIZE) board[i-1][j+2].danger[p] = 'S';
		if(j-2>=0) board[i-1][j-2].danger[p] = 'S';
	}
	if(i+2 < SIZE) {
		if(j+1<SIZE) board[i+2][j+1].danger[p] = 'S';
		if(j-1>=0) board[i+2][j-1].danger[p] = 'S';
	}
	if(i-2 >= 0) {
		if(j+1<SIZE) board[i-2][j+1].danger[p] = 'S';
		if(j-1>=0) board[i-2][j-1].danger[p] = 'S';
	}			
}
int checkmate(BOARD (*board)[SIZE], int i, int j) {
	//Current player is the one attacking (player+1)%2s king
	//Checks if king can move 
	if(i-1>=0) {
		if(j-1>=0) 
			if(board[i-1][j-1].player != player 
				&& board[i-1][j-1].danger[player] == 0) return 0;
		if(j+1<SIZE) 
			if(board[i-1][j+1].player != player 
				&& board[i-1][j+1].danger[player] == 0) return 0;
		if(board[i-1][j].player != player 
			&& board[i-1][j].danger[player] == 0) return 0;
	}
	if(i+1<SIZE) {
		if(j-1>=0) 
			if(board[i+1][j-1].player != player 
				&& board[i+1][j-1].danger[player] == 0) return 0;
		if(j+1<SIZE) 
			if(board[i+1][j+1].player != player 
				&& board[i+1][j+1].danger[player] == 0) return 0;
		if(board[i+1][j].player != player 
			&& board[i+1][j].danger[player] == 0) return 0;
	}
	if(j-1>=0) 
		if(board[i][j-1].player != player 
			&& board[i][j-1].danger[player] == 0) return 0;
	if(j+1<SIZE) 
		if(board[i][j+1].player != player 
			&& board[i][j+1].danger[player] == 0) return 0;
	/*Add test if other figures can destroy the piece that is a threat to king 
	or block its path later */
	return 1;
}
void rm_danger(BOARD (*board)[SIZE], int piece, int i, int j) {
	int p, k;
	switch(board[i][j].type) {
		case 'P': 
			if(player) {
				if(i+1 < SIZE) {
					if(j+1 < SIZE) board[i+1][j+1].danger[player] = 0;
					if(j-1 >= 0) board[i+1][j-1].danger[player] = 0;
				}
			}
			else {
				if(i-1 >= 0) {
					if(j+1 < SIZE) board[i-1][j+1].danger[player] = 0;
					if(j-1 >= 0) board[i-1][j-1].danger[player] = 0;
				}
			}
		break;
		case 'S':
			if(i+1 < SIZE) {
				if(j+2<SIZE) board[i+1][j+2].danger[player] = 0;
				if(j-2>=0) board[i+1][j-2].danger[player] = 0;
			}
			if(i-1 >= 0) {
				if(j+2<SIZE) board[i-1][j+2].danger[player] = 0;
				if(j-2>=0) board[i-1][j-2].danger[player] = 0;
			}
			if(i+2 < SIZE) {
				if(j+1<SIZE) board[i+2][j+1].danger[player] = 0;
				if(j-1>=0) board[i+2][j-1].danger[player] = 0;
			}
			if(i-2 >= 0) {
				if(j+1<SIZE) board[i-2][j+1].danger[player] = 0;
				if(j-1>=0) board[i-2][j-1].danger[player] = 0;
			}
		break;
	}
	//puts("after free:");
	//print_danger(board, player);
}
void rm_piece(BOARD *piece) {
	piece->type = 0;
	piece->player = -1;
	piece->state = 0;
}