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
//Current player is the one whose king is being attacked
int checkmate(BOARD (*board)[SIZE], int king_i, int king_j) {
	int atk_i, atk_j;
	int i, j;
	
	if(can_kmov(board, king_i, king_j, board[king_i][king_j].player)) return 0;
	
	//Creates copy of the current matrix
	BOARD (*b2)[SIZE] = malloc(sizeof(BOARD [SIZE][SIZE]));
	if(b2 == NULL) exit(EXIT_FAILURE);
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			b2[i][j].danger = (int *) malloc(2*sizeof(int));
			b2[i][j].type = board[i][j].type;
			b2[i][j].player = board[i][j].player;
			b2[i][j].state = board[i][j].state;
			b2[i][j].danger[0] = board[i][j].danger[0];
			b2[i][j].danger[1] = board[i][j].danger[1];
		}
	}
	
	find_attacker(b2, &atk_i, &atk_j, king_i, king_j);
	b2[atk_i][atk_j].state = 2;
	update_state(b2, atk_i, atk_j, king_i, king_j);
	
	int k, p;
	int k_player = b2[king_i][king_j].player;
	//WARNING: Code below is super slow (facepalm may be imminent). 
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			if(b2[i][j].player != k_player) continue;
			for(k ^= k; k < SIZE; k++) {
				for(p ^= p; p < SIZE; p++) {
					if(b2[k][p].state == 2) {
						switch(b2[i][j].type) {
							case 'P': 	if(pawn_check(b2, i, j, k, p)) 
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
							case 'N':	if(knight_check(b2, i, j, k, p))
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
							case 'K':	if(king_check(b2, i, j, k, p)) 
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
							case 'B':	if(bishop_check(b2, i, j, k, p)) 
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
							case 'R':	if(rook_check(b2, i, j, k, p)) 
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
							case 'Q':	if(queen_check(b2, i, j, k, p)) 
												if(!is_still_mate(b2, i, j, k, p)) goto LEAVE; 
								break; 
						}
					}
				}		
			}
		}
	}
	board_free(b2);
	return 1;
LEAVE:;
	board_free(b2);
	return 0;
}
int is_still_mate(BOARD (*board)[SIZE], int i1, int j1, int i2 , int j2) {
	int i, j;
	BOARD (*b2)[SIZE] = malloc(sizeof(BOARD [SIZE][SIZE]));
	if(b2 == NULL) exit(EXIT_FAILURE);
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			b2[i][j].danger = (int *) calloc(2,sizeof(int));
			b2[i][j].state = board[i][j].state;
			b2[i][j].type = board[i][j].type;
			b2[i][j].player = board[i][j].player;
		}
	}
	b2[i2][j2].type = b2[i1][j1].type;
	b2[i2][j2].state = 1;
	b2[i2][j2].player = b2[i1][j1].player;
	rm_piece(&b2[i1][j1]);
	update_danger(b2);
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			if(b2[i][j].type == 'K' && b2[i][j].player == player) {
				if(b2[i][j].danger[player^1]) {
					board_free(b2);
					return 1;
				}
				else {
					board_free(b2);
					return 0;
				}
			}
		}
	}
	board_free(b2);	// this is only gonna be done if no king is present
	return 0;			// which shouldn't happen ever, but just in case of error i wrote it anyway
}
void update_state(BOARD (*b2)[SIZE], int i, int j, int king_i, int king_j) {
	switch(b2[i][j].type) {
		case 'B': us_bishop(b2, i, j, king_i, king_j); break;
		case 'R': us_rook(b2, i, j, king_i, king_j); break;
		case 'Q': us_bishop(b2, i, j, king_i, king_j);
					   us_rook(b2, i, j, king_i, king_j); 
		break;
	}
}
void us_rook(BOARD(*board)[SIZE], int i, int j, int king_i, int king_j) {
	int k;
	int subi = i - king_i;
	int subj = j - king_j;
	if(!((subi == 0) ^ (subj == 0))) return;
	if(subi > 0)  {		
		for(k = 1; i - k >= 0; k++) {		
			board[i-k][j].state = 2;
			if(board[i-k][j].type) break;
		}
	}
	else if(subi < 0) {
		for(k = 1; i + k < SIZE; k++) {	
			board[i+k][j].state = 2;
			if(board[i+k][j].type) break;
		}
	}
	else if(subj > 0) {
		for(k = 1; j - k >= 0; k++) {	
			board[i][j-k].state = 2;
			if(board[i][j-k].type) break;
		}
	}
	else if(subj < 0) {
		for(k = 1; j + k < SIZE; k++) {	
			board[i][j+k].state = 2;
			if(board[i][j+k].type) break;
		}
	}	
}
void us_bishop(BOARD (*board)[SIZE], int i, int j, int king_i, int king_j) {
	int k;
	int subi = i - king_i;
	int subj = j - king_j;
	if(!subi || !subj) return;
	if(subi > 0) {
		if(subj > 0) {
			for(k = 1; i - k >= 0 && j - k >= 0; k++) {	
					board[i-k][j-k].state = 2;
					if(board[i-k][j-k].type) break;
			}
		}
		else {
			for(k = 1; i - k >= 0 && j + k < SIZE; k++) {	
					board[i-k][j+k].state = 2;
					if(board[i-k][j+k].type) break;
			}
		}
	}
	else{
		if(subj > 0) {
			for(k = 1; i + k < SIZE && j - k >= 0; k++) {	
					board[i+k][j-k].state = 2;
					if(board[i+k][j-k].type) break;
			}
		}
		else {
			for(k = 1; i + k < SIZE && j + k < SIZE; k++) {	
					board[i+k][j+k].state = 2;
					if(board[i+k][j+k].type) break;
			}
		}
	}
}
void find_attacker(BOARD (*board)[SIZE], int *atk_i, int *atk_j, int king_i, int king_j) {
	per ^= per;
	for(*atk_i = 0; *atk_i < SIZE; (*atk_i)++) {
		for(*atk_j = 0; *atk_j < SIZE; (*atk_j)++) {
			switch(board[*atk_i][*atk_j].type) {
				case 'P': 	if(pawn_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;
				case 'N':	if(knight_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;	
				case 'K':	if(king_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;	
				case 'B':	if(bishop_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;
				case 'R':	if(rook_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;
				case 'Q':	if(queen_check(board, *atk_i, *atk_j, king_i, king_j)) return; break;
			}
		}
	}
}

//p1  is player whose king is attacked
int can_kmov(BOARD (*board)[SIZE], int i, int j, int p1) {
	int p2 = p1^1;	// the one attacking the king
	if(i-1>=0) {
		if(j-1>=0) 
			if(board[i-1][j-1].player != p1 && board[i-1][j-1].danger[p2] == 0) 
				return 1;
		if(j+1<SIZE) 
			if(board[i-1][j+1].player != p1 && board[i-1][j+1].danger[p2] == 0) 
				return 1;
		if(board[i-1][j].player != p1 && board[i-1][j].danger[p2] == 0) 
			return 1;
	}
	if(i+1<SIZE) {
		if(j-1>=0) 
			if(board[i+1][j-1].player != p1 && board[i+1][j-1].danger[p2] == 0) 
				return 1;
		if(j+1<SIZE) 
			if(board[i+1][j+1].player != p1 && board[i+1][j+1].danger[p2] == 0) 
				return 1;
		if(board[i+1][j].player != p1 && board[i+1][j].danger[p2] == 0) 
			return 1;
	}
	if(j-1>=0) 
		if(board[i][j-1].player != p1  && board[i][j-1].danger[p2] == 0) 
			return 1;
	if(j+1<SIZE) 
		if(board[i][j+1].player != p1 && board[i][j+1].danger[p2] == 0) 
			return 1;
	return 0;
}