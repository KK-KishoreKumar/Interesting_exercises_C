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
typedef struct move_st {
	int type1;
	int type2;
	int i1;	// from row
	int j1;	// from column
	int i2;	// to row
	int j2;	// to column
	int state1;
	int state2;
	int player1;
	int player2;
} MOVE;
typedef struct node_st {
	MOVE inf;
	struct node_st *pnext;
	struct node_st *pprev;
} NODE;

NODE *make_node(MOVE);
void list_push(NODE **, MOVE);
void delete_fnodes(NODE **);
NODE *find_start(NODE *);
void print_list(NODE *);