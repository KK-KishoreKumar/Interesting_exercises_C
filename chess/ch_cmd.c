#include "chess.h"
int command_check(BOARD (*board)[SIZE], char *s) {
	int i1, j1, i2, j2, piece;
	castling ^= castling;
	piece = *s++;
	j1 = *s++ - 'a';
	i1 = SIZE - (*s++ - '0');
	j2 = *s++ - 'a';
	i2 = SIZE - (*s++ - '0');
	
	if(board[i1][j1].player != player) {
	    puts("error: you are trying to move an empty/enemy piece.");
		return 0;
	}
	if(board[i1][j1].type != piece) {
		printf("error: you tried to move '%c', but '%c' is on position '%c%c'.\n"
				, piece, board[i1][j1].type, (j1+'a'), (-i1+SIZE+'0'));
		return 0;
	}
	per ^= per;
	per++;
	switch(piece) {
		case 'P': 	if(!pawn_check(board, i1, j1, i2, j2)) return 0; break;
		case 'S':	if(!knight_check(board, i1, j1, i2, j2)) return 0; break;	
		case 'K':	if(!king_check(board, i1, j1, i2, j2)) return 0; break;	
		case 'L':	if(!bishop_check(board, i1, j1, i2, j2)) return 0; break;
		case 'T':	if(!rook_check(board, i1, j1, i2, j2)) return 0; break;
		case 'D':	if(!queen_check(board, i1, j1, i2, j2)) return 0; break;
	}
	
	if(board[i2][j2].type == 'K') {
		printf("\nPlayer %d has won!\n", player+1);
		free_danger(board);
		exit(EXIT_SUCCESS);
	}
	
	if(is_still_mate(board, i1, j1, i2, j2)) {
		puts("error: move unavailable.");	// this is in case if it's still check, or if it is going to be checkmate after the move
		return 0;
	}
	
	return 1;
}

int pawn_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	if(j1!=j2) {	
		if(absj > 1) {
			if(per) printf("error: pawn can only move vertical. (diagonally is exeption for eating enemy piece).\n");
			return 0;
		}
		else if(!board[i2][j2].type) {
			if(per) puts("error: pawn can move diagonally only to eat enemys piece.");
			return 0;
		}
	}
	if((player == 0) ? i1 < i2 : i1 > i2) {
		if(per) puts("error: pawn can't go backwards!");
		return 0;
	}
	if(board[i1][j1].state == 0 ? absi>2 : absi>1) {
		if(per) puts("error: pawn can move 2 steps forward if first move, and 1 step if not.");
		return 0;
	}
	if(!board[i1][j1].state && absi == 2) {
		if(i1 > i2 ? board[i1-1][j1].type  : board[i1+1][j1].type ) {
			if(per) puts("error: another piece is blocking the path.");
			return 0;
		}
	}
	if(j1 == j2 && board[i2][j2].type) {
		if(per) puts("error: another piece is blocking the path.");
		return 0;
	}
	
	return 1;
}

int knight_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	if(absi < 1 || absi > 2 || absj < 1 || absj > 2) {
		if(per) puts("error: knight can only move in 'L'-shape.");
		return 0;
	}
	if(!((absi == 1 && absj ==2) || (absi == 2 && absj == 1))) {
		if(per) puts("error: knight can only move in 'L'-shape. ");
		return 0;
	} 
	return 1;
}

int king_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj, k, enemy = board[i1][j1].player^1;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	if(board[i2][j2].danger[enemy] != 0) {
		if(per) printf("error: Destination '%c%c' is under attack.\n", (j2+'a'), (-i2+SIZE+'0') );
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	//Checks if castling move first
	if (!check && i1 == i2 && absj == 2 && !board[i1][j1].state) {
		if(j1 < j2) {
			if(board[i1][SIZE-1].type == 'T' && !board[i1][SIZE-1].state 
				&& board[i1][SIZE-1].player == board[i1][j1].player && !board[i1][SIZE-1].danger[enemy]) {
				for(k = 1; k <= 2; k++) {
					if(board[i1][j1+k].type || board[i1][j1+k].danger[enemy]) {
						if(per) puts("error: path for castling move is blocked / under attack.");
						return 0;
					}
				}
			}
			else {
				if(per) puts("error: castling unavailable.");
				return 0;
			}
		}
		else {
			if(board[i1][0].type == 'T' && !board[i1][0].state
				&& board[i1][0].player == board[i1][j1].player && !board[i1][0].danger[enemy]) {
				for(k = 1; k < j1; k++) {
					if(board[i1][j1-k].type	|| board[i1][j1-k].danger[enemy]) {
						if(per) puts("error: path for castling move is blocked / under attack.");
						return 0;
					}
				}
			}
			else {
				if(per) puts("error: castling unavailable.");
				return 0;
			}
		}
		castling = 1;
	}
	else if(absi>1 || absj>1) {
		if(per) puts("error: king can only move one step in any direction.");
		return 0;
	}
	return 1;
}

int bishop_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj, p, k;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	if(absi != absj) {
		if(per) puts("error: bishop can only move diagonally.");
		return 0;
	}
	/*checks if there is an obsticle in diagonal path*/
	for(k = 1, p = absi; k < p; k++) {
			if(i1 > i2) {
				if(j1 < j2 ? board[i1 - k][j1+k].type : board[i1 - k][j1-k].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
				}
			}
			else if(j1 < j2 ? board[i1 + k][j1+k].type : board[i1 + k][j1-k].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
			}
	}
	
	return 1;
}

int rook_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj, p, k;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	if((i1!=i2)&&(j1!=j2)) {
		if(per) puts("error: rook can only move vertically or horizontally");
		return 0;
	}
	for(k = 1, p = (i1 == i2) ? absj : absi; k < p; k++) 
		if(i1 == i2) {
			if(j1 > j2 ? board[i1][j1-k].type : board[i1][j1+k].type) {
				if(per) puts("error: another piece is blocking the path.");
				return 0;
			}
		}
		else if(i1 > i2 ? board[i1-k][j1].type : board[i1+k][j1].type) {
				if(per) puts("error: another piece is blocking the path.");
				return 0;
		}
	return 1;
}

int queen_check(BOARD (*board)[SIZE], int i1, int j1, int i2, int j2) {
	int absi, absj, p, k;
	if(board[i2][j2].player == board[i1][j1].player) {
		if(per) puts("error: friendly piece is blocking the path.");
		return 0;
	}
	absi = abs(i1 - i2);
	absj = abs(j1 - j2);
	if(absi == absj) {
		for(k = 1, p = absi; k < p; k++) 
			if(i1 > i2) {
				if(j1 < j2 ? board[i1 - k][j1+k].type : board[i1 - k][j1-k].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
				}
			}
			else if(j1 < j2 ? board[i1 + k][j1+k].type : board[i1 + k][j1-k].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
			}
	}
	else if((i1==i2)^(j1==j2)) {
		for(k = 1, p = (i1 == i2) ? absj : absi; k < p; k++) 
			if(i1 == i2) {
				if(j1 > j2 ? board[i1][j1-k].type : board[i1][j1+k].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
				}
			}
			else if(i1 > i2 ? board[i1-k][j1].type : board[i1+k][j1].type) {
					if(per) puts("error: another piece is blocking the path.");
					return 0;
			}
	}
	else {
		if(per) puts("error: queen can move vertically, horizontally or diagonally.");
		return 0;
	}
	return 1;
}

/*Checks if syntax of given command is correct, returns 1 if it is, and 0 if it's not*/
int syntax_check(char *p) {
	if(*p != 'S' && *p != 'P' && *p != 'D' && *p != 'L' && *p != 'K' && *p != 'T') return 0;
	if(*++p < 'a' || *p > 'h') return 0;
	if(*++p < '1' || *p > '8') return 0;
	if(*++p < 'a' || *p > 'h') return 0;
	if(*++p < '1' || *p > '8') return 0;
	return 1;
}