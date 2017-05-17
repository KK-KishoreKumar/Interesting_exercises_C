#include "list.h"
#include "chess.h"
#include "redo_undo.h"

int undo(BOARD (*board)[SIZE], NODE **pcurr) {
	if(*pcurr != NULL) {
		int i = 0;
		MOVE inf;
		NODE *ptest = *pcurr;
		while(ptest!=NULL && i != 2) {
			ptest = ptest->pprev;
			i++;
		}
		if(i == 2) {
			while(--i >= 0) {							
				inf = (*pcurr)->inf;
				
				*pcurr = (*pcurr)->pprev;
				
				board[inf.i1][inf.j1].type = inf.type1;
				board[inf.i1][inf.j1].state = inf.state1;
				board[inf.i1][inf.j1].player = inf.player1;
				
				board[inf.i2][inf.j2].type = inf.type2;
				board[inf.i2][inf.j2].state = inf.state2;
				board[inf.i2][inf.j2].player = inf.player2;
			}
			return 0;
		}
	}
	return 1; 
}

int redo(BOARD (*board)[SIZE], NODE **pcurr) {
	if(*pcurr != NULL) {
		int i = 0;
		MOVE inf;
		NODE *ptest = (*pcurr)->pnext;
		
		while(ptest!=NULL && i != 2) {
			ptest = ptest->pnext;
			i++;
		}
		if(i == 2) {
			while(--i >= 0) {
				inf = (*pcurr)->pnext->inf;
				*pcurr = (*pcurr)->pnext;
			
				board[inf.i2][inf.j2].type = inf.type1;
				board[inf.i2][inf.j2].state = inf.state1;
				board[inf.i2][inf.j2].player = inf.player1;
				rm_piece(&board[inf.i1][inf.j1]);
				print_matrix(board);
			}
			return 0;
		}
	}
	return 1; 
}