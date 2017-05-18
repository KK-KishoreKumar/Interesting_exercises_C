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
int stalemate(BOARD (*board)[SIZE]) {
	int i, j, enemy = player ^1;
	per = 0;
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			if(board[i][j].player == player) {
				switch(board[i][j].type) {
					case 'K' : if(can_kmov(board, i, j, player)) return 0; break; 
					case 'P':  if(can_pmov(board, i, j, player)) return 0; break;
					case 'S': if(can_smov(board, i, j, player)) return 0; break;
					case 'L': if(can_bmov(board, i, j, player)) return 0; break;
					case 'T': if(can_rmov(board, i, j, player)) return 0; break;
					case 'D': if(can_bmov(board, i, j, player) || can_rmov(board, i, j, player)) return 0; break;
				}
			}
		}
	}
	return 1;
}
int can_pmov(BOARD (*board)[SIZE], int i1, int j1, int p1) {
	int i2, j2 = j1;
	i2 = (p1 == 1) ? ( i1 + 1) : (i1 - 1);
	if(!pawn_check(board,i1,j1,i2,j2)) 
		return 0;
	return !is_still_mate(board, i1, j1, i2, j2);
}
int can_smov(BOARD (*board)[SIZE], int i, int j, int p) {
	if(i+1 < SIZE) {
		if(j+2<SIZE) if(knight_check(board, i, j, i+1, j+2) && !is_still_mate(board, i, j, i+1, j+2)) return 1;
		if(j-2>=0) if(knight_check(board, i, j, i+1, j-2) && !is_still_mate(board, i, j, i+1, j-2)) return 1;
	}
	if(i-1 >= 0) {
		if(j+2<SIZE) if(knight_check(board, i, j, i-1, j+2) && !is_still_mate(board, i, j, i-1, j+2)) return 1;
		if(j-2>=0) if(knight_check(board, i, j, i-1, j-2) && !is_still_mate(board, i, j, i-1, j-2)) return 1;
	}
	if(i+2 < SIZE) {
		if(j+1<SIZE) if(knight_check(board, i, j, i+2, j+1) && !is_still_mate(board, i, j, i+2, j+1)) return 1;
		if(j-1>=0) if(knight_check(board, i, j, i+2, j-1) && !is_still_mate(board, i, j, i+2, j-1)) return 1;
	}
	if(i-2 >= 0) {
		if(j+1<SIZE) if(knight_check(board, i, j, i-2, j+1) && !is_still_mate(board, i, j, i-2, j+1)) return 1;
		if(j-1>=0) if(knight_check(board, i, j, i-2, j-1) && !is_still_mate(board, i, j, i-2, j-1)) return 1;
	}		
	return 0;
}
// for bishop and rook i only need to check 1 spot, since these functions only test if piece can move at all.
int can_bmov(BOARD (*board)[SIZE], int i, int j, int p) {
	if(i-1 >= 0) {
		if(j+1 < SIZE) if(bishop_check(board, i, j, i-1, j+1) && !is_still_mate(board, i, j, i-1, j+1)) return 1;
		if(j-1 >= 0) if(bishop_check(board, i, j, i-1, j-1) && !is_still_mate(board, i, j, i-1, j-1)) return 1;
	}
	if(i+1 < SIZE) {
		if(j+1 < SIZE) if(bishop_check(board, i, j, i+1, j+1) && !is_still_mate(board, i, j, i+1, j+1)) return 1;
		if(j-1 >= 0) if(bishop_check(board, i, j, i+1, j-1) && !is_still_mate(board, i, j, i+1, j-1)) return 1;
	}
	return 0;
}
int can_rmov(BOARD (*board)[SIZE], int i, int j, int p) {
	if(j-1>=0) if(rook_check(board, i, j, i, j-1) && !is_still_mate(board, i, j, i, j-1)) return 1;
	if(j+1 < SIZE) if(rook_check(board, i, j, i, j+1) && !is_still_mate(board, i, j, i, j+1)) return 1;
	if(i-1>=0) if(rook_check(board, i, j, i-1, j) && !is_still_mate(board, i, j, i-1, j)) return 1;
	if(i+1 < SIZE) if(rook_check(board, i, j, i+1, j) && !is_still_mate(board, i, j, i+1, j)) return 1;
	return 0;
}