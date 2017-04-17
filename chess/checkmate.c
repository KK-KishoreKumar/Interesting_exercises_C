#include "chess.h"
//Current player is the one attacking (player+1)%2s king
int checkmate(BOARD (*board)[SIZE], int i, int j) {
	int p1 = (player+1)%2;
	if(can_kmov(board, i, j, p1)) return 0;
	
	/*Add test if other figures can destroy the piece that is a threat to king 
	or block its path later */
	return 1;
}

//p1  is player whose king is attacked
int can_kmov(BOARD (*board)[SIZE], int i, int j, int p1) {
	int p2 = (p1+1)%2;	// the one attacking the king
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