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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define SIZE 8
#define MAX_BUFFER 7
#define RED "\x1b[33;1m"
#define BLUE  "\x1b[37;1m"
#define BG_WHITE "\x1b[40m"
#define BG_YELLOW "\x1b[41m"
#define CL_RESET  "\x1b[0m"
/*NOTE: macro names above do not match the actuall color because i'm still experimenting 
	with what colors match the best, and i don't want to change macro name everytime i try 
	out new color*/
	
typedef struct board_st{
	int type;	// type = {K,T,S,D,L,0}
	int player;	// player = {0, 1}
	int state;	/* 0 - hasn't moved. 
						1 - has moved during game. 
						2 - is attacking / danger to king*/
	int *danger;	//danger[player] = 0  (player can't attack this piece)
						//danger[player] = 1 (player can attack this piece)
} BOARD;

//global variables
extern int winner;
extern int player;
extern int check;
extern int castling;
extern int per;

//basic board functions (print, init, copy, free)
void print_matrix(BOARD (*)[SIZE]);
void init_board(BOARD (*)[SIZE]);
void usage_tip(void);
void print_danger(BOARD (*)[SIZE], int );
void print_state(BOARD (*)[SIZE]);
void board_free(BOARD (*)[SIZE]);
void boardcpy(BOARD (*)[SIZE], BOARD (*)[SIZE]); 
void free_danger(BOARD (*)[SIZE]);

//syntax and command check functions
int syntax_check(char *);
int command_check(BOARD (*)[SIZE], char *);
int pawn_check(BOARD (*)[SIZE], int, int, int, int);
int knight_check(BOARD (*)[SIZE], int, int, int, int);
int king_check(BOARD (*)[SIZE], int, int, int, int);
int rook_check(BOARD (*)[SIZE], int, int, int, int);
int bishop_check(BOARD (*)[SIZE], int, int, int, int);
int queen_check(BOARD (*)[SIZE], int, int, int, int);

//danger remove/update functions
void rm_piece(BOARD *);
void rm_danger(BOARD (*)[SIZE], int, int, int);
void rm_danger_all(BOARD(*)[SIZE], int);
void rm_pawn(BOARD (*)[SIZE], int, int);
void rm_knight(BOARD (*)[SIZE], int, int);
void rm_king(BOARD (*)[SIZE], int, int);
void rm_bishop(BOARD (*)[SIZE], int, int);
void rm_rook(BOARD (*)[SIZE], int, int);
void rm_queen(BOARD (*)[SIZE], int, int);

void update_danger(BOARD (*)[SIZE]);
void update_pawn(BOARD (*)[SIZE], int, int, int);
void update_knight(BOARD (*)[SIZE], int, int, int);
void update_king(BOARD (*)[SIZE], int, int, int);
void update_bishop(BOARD (*)[SIZE], int, int, int);
void update_rook(BOARD (*)[SIZE], int, int, int);
void update_queen(BOARD (*)[SIZE], int, int, int);

//checkmate functions
int checkmate(BOARD (*)[SIZE], int, int);
int can_kmov(BOARD (*)[SIZE], int, int, int);
void find_attacker(BOARD (*)[SIZE], int *, int *, int, int);
void update_state(BOARD (*)[SIZE], int, int, int, int);
void us_bishop(BOARD (*)[SIZE], int, int, int, int);
void us_rook(BOARD (*)[SIZE], int, int, int, int);
int is_still_mate(BOARD (*)[SIZE], int, int, int, int);

//stalemate
int stalemate(BOARD (*)[SIZE]);
int can_pmov(BOARD (*)[SIZE], int, int, int);
int can_smov(BOARD (*)[SIZE], int, int, int);
int can_bmov(BOARD (*)[SIZE], int, int, int);
int can_rmov(BOARD (*)[SIZE], int, int, int);