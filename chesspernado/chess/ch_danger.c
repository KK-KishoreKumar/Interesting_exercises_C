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

//update functions
void update_danger(BOARD (*board)[SIZE]) {
	int i, j, p;
	for(i ^= i; i < SIZE; i++) {
		for(j ^= j; j < SIZE; j++) {
			p = board[i][j].player;
			switch(board[i][j].type) {
				case 'P': update_pawn(board, i, j, p); break;
				case 'N': update_knight(board, i, j, p); break;
				case 'K': update_king(board, i, j, p); break;
				case 'B': update_bishop(board, i, j, p); break;
				case 'R': update_rook(board, i, j, p); break;
				case 'Q': update_queen(board, i, j, p); break;
			}
		}
	}
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
		if(j+2<SIZE) board[i+1][j+2].danger[p] = 'N';
		if(j-2>=0) board[i+1][j-2].danger[p] = 'N';
	}
	if(i-1 >= 0) {
		if(j+2<SIZE) board[i-1][j+2].danger[p] = 'N';
		if(j-2>=0) board[i-1][j-2].danger[p] = 'N';
	}
	if(i+2 < SIZE) {
		if(j+1<SIZE) board[i+2][j+1].danger[p] = 'N';
		if(j-1>=0) board[i+2][j-1].danger[p] = 'N';
	}
	if(i-2 >= 0) {
		if(j+1<SIZE) board[i-2][j+1].danger[p] = 'N';
		if(j-1>=0) board[i-2][j-1].danger[p] = 'N';
	}		
}
void update_king(BOARD (*board)[SIZE], int i, int j, int p) {
	if(i-1>=0) {
		if(j-1>=0) board[i-1][j-1].danger[p] = 'K';
		if(j+1<SIZE) board[i-1][j+1].danger[p] = 'K';
		board[i-1][j].danger[p] = 'K';
	}
	if(i+1<SIZE) {
		if(j-1>=0) board[i+1][j-1].danger[p] = 'K';
		if(j+1<SIZE) board[i+1][j+1].danger[p] = 'K';
		board[i+1][j].danger[p] = 'K';
	}
	if(j-1>=0) board[i][j-1].danger[p] = 'K';
	if(j+1<SIZE) board[i][j+1].danger[p] = 'K';
}
void update_bishop(BOARD (*board)[SIZE], int i, int j, int p) {
	int k;
	for(k = 1; i - k >= 0; k++) {	
		if(j - k >= 0) {
			board[i-k][j-k].danger[p] = 'B';
			if(board[i-k][j-k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i - k >= 0; k++) {	
		if(j + k < SIZE) {
			board[i-k][j+k].danger[p] = 'B';
			if(board[i-k][j+k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j - k >= 0) {
			board[i+k][j-k].danger[p] = 'B';
			if(board[i+k][j-k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j + k < SIZE) {
			board[i+k][j+k].danger[p] = 'B';
			if(board[i+k][j+k].type != 0) break;
		}
		else break;
	}
}
void update_rook(BOARD (*board)[SIZE], int i, int j, int p) {
	int k;
	for(k = 1; i - k >= 0; k++) {		
		board[i-k][j].danger[p] = 'R';
		if(board[i-k][j].type != 0) break;
	}
	for(k = 1; j - k >= 0; k++) {	
		board[i][j-k].danger[p] = 'R';
		if(board[i][j-k].type != 0) break;
	}
	for(k = 1; j + k < SIZE; k++) {	
		board[i][j+k].danger[p] = 'R';
		if(board[i][j+k].type != 0) break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		board[i+k][j].danger[p] = 'R';
		if(board[i+k][j].type != 0) break;
	}
}
void update_queen(BOARD (*board)[SIZE], int i, int j, int p) {
	int k; 
	for(k = 1; i - k >= 0; k++) {	
		if(j - k >= 0) {
			board[i-k][j-k].danger[p] = 'Q';
			if(board[i-k][j-k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i - k >= 0; k++) {	
		if(j + k < SIZE) {
			board[i-k][j+k].danger[p] = 'Q';
			if(board[i-k][j+k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j - k >= 0) {
			board[i+k][j-k].danger[p] = 'Q';
			if(board[i+k][j-k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j + k < SIZE) {
			board[i+k][j+k].danger[p] = 'Q';
			if(board[i+k][j+k].type != 0) break;
		}
		else break;
	}
	for(k = 1; i - k >= 0; k++) {		
		board[i-k][j].danger[p] = 'Q';
		if(board[i-k][j].type != 0) break;
	}
	for(k = 1; j - k >= 0; k++) {	
		board[i][j-k].danger[p] = 'Q';
		if(board[i][j-k].type != 0) break;
	}
	for(k = 1; j + k < SIZE; k++) {	
		board[i][j+k].danger[p] = 'Q';
		if(board[i][j+k].type != 0) break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		board[i+k][j].danger[p] = 'Q';
		if(board[i+k][j].type != 0) break;
	}
}

//remove functions
void rm_danger(BOARD (*board)[SIZE], int piece, int i, int j) {
	switch(board[i][j].type) {
		case 'P': rm_pawn(board, i, j); break;
		case 'N': rm_knight(board, i, j); break;
		case 'K': rm_king(board, i, j); break;
		case 'B': rm_bishop(board, i, j); break;
		case 'R': rm_rook(board, i, j); break;
		case 'Q': rm_queen(board, i, j); break;
	}
}
void rm_pawn(BOARD (*board)[SIZE], int i, int j) {
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
}
void rm_knight(BOARD (*board)[SIZE], int i, int j) {
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
}
void rm_king(BOARD (*board)[SIZE], int i, int j) {
	if(i-1>=0) {
		if(j-1>=0) board[i-1][j-1].danger[player] = 0;
		if(j+1<SIZE) board[i-1][j+1].danger[player] = 0;
		board[i-1][j].danger[player] = 0;
	}
	if(i+1<SIZE) {
		if(j-1>=0) board[i+1][j-1].danger[player] = 0;
		if(j+1<SIZE) board[i+1][j+1].danger[player] = 0;
		board[i+1][j].danger[player] = 0;
	}
	if(j-1>=0) board[i][j-1].danger[player] = 0;
	if(j+1<SIZE) board[i][j+1].danger[player] = 0;
}
void rm_bishop(BOARD (*board)[SIZE], int i, int j) {
	int k;
	for(k = 1; i - k >= 0; k++) {	
		if(j - k >= 0) {
			board[i-k][j-k].danger[player] = 0;
		}
		else break;
	}
	for(k = 1; i - k >= 0; k++) {	
		if(j + k < SIZE) {
			board[i-k][j+k].danger[player] = 0;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j - k >= 0) {
			board[i+k][j-k].danger[player] = 0;
		}
		else break;
	}
	for(k = 1; i + k < SIZE; k++) {	
		if(j + k < SIZE) {
			board[i+k][j+k].danger[player] = 0;
		}
		else break;
	}
}
void rm_rook(BOARD (*board)[SIZE], int i, int j) {
	int k;
	for(k = 1; i - k >= 0; k++) {		
		board[i-k][j].danger[player] = 0;
	}
	for(k = 1; j - k >= 0; k++) {	
		board[i][j-k].danger[player] = 0;
	}
	for(k = 1; j + k < SIZE; k++) {	
		board[i][j+k].danger[player] = 0;
	}
	for(k = 1; i + k < SIZE; k++) {	
		board[i+k][j].danger[player] = 0;
	}
}
void rm_queen(BOARD (*board)[SIZE], int i, int j) {
	rm_bishop(board, i, j);
	rm_rook(board, i, j);
}
void rm_piece(BOARD *piece) {
	piece->type = 0;
	piece->player = -1;
	piece->state = 0;
}
void rm_danger_all(BOARD (*board)[SIZE], int p) {
	int i, j;
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			board[i][j].danger[p] ^= board[i][j].danger[p];
		}
	}
}