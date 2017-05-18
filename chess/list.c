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
#include "list.h"
NODE *make_node(MOVE inf) {
	NODE *pnew = malloc(sizeof(NODE));
	if(pnew == NULL){
		puts("ERROR: Out of RAM!");
		exit(EXIT_FAILURE);
	}
	pnew->pnext = pnew->pprev = NULL;
	pnew->inf = inf;
	return pnew;
}
/*This is some kind of mixture of list and stack which should work
	good for my /undo and /redo commands*/
void list_push(NODE **pcurr, MOVE inf) {
	NODE *pnew = make_node(inf);
	if(*pcurr == NULL) {
		*pcurr = pnew;
		return;
	}
	else if((*pcurr)->pnext == NULL) {
		(*pcurr)->pnext = pnew;
		pnew->pprev = *pcurr;
	}
	else {
		(*pcurr)->pnext->inf = inf;
		free(pnew);
	}
	*pcurr = (*pcurr)->pnext;
	delete_fnodes(pcurr);
}
//deletes all nodes after pcurr node (hence the name delete forward nodes)
void delete_fnodes(NODE **pcurr) {
	if((*pcurr)->pnext != NULL) {
		NODE *ptemp = (*pcurr)->pnext;
		NODE *pnext;
		(*pcurr)->pnext = NULL;
		while(ptemp != NULL) {
			pnext = ptemp->pnext;
			ptemp->pnext = ptemp->pprev = NULL;
	
			free(ptemp);
			ptemp = pnext;
		}
	}
}
NODE *find_start(NODE *pcurr) {
	if(pcurr!=NULL) {
		while(pcurr->pprev != NULL)
			pcurr = pcurr->pprev;
	}
	return pcurr;
}
void print_list(NODE *pcurr) {
	NODE *start = find_start(pcurr);
	if(start!=NULL){
		FILE *pout = fopen("command_history.txt", "w");
		if(pout == NULL) {
			puts("error: failed to create 'command_history.txt' output file.");
			exit(EXIT_FAILURE);
		}
		while(start != NULL) {
			fprintf(pout, "Player %d: %c%c%d%c%d\r\n", start->inf.player1 + 1,
																					start->inf.type1,
																					start->inf.j1 + 'a',
																					SIZE - start->inf.i1,
																					start->inf.j2 + 'a',
																					SIZE - start->inf.i2);
			start = start->pnext;
		}
		fclose(pout);
	}
}