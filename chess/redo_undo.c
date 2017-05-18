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