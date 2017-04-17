#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define SIZE 8
#define MAX_BUFFER 7
#define RED "\x1b[33;3;1m"
#define BLUE  "\x1b[37;3;1m"
#define BG_WHITE "\x1b[40m"
#define BG_YELLOW "\x1b[41m"
#define CL_RESET  "\x1b[0m"
/*NOTE: macro names above do not match the actuall color because i'm still experimenting 
	with what colors match the best, an i don't want to change macro name everytime i try 
	out new color*/
	
typedef struct board_st{
	int type;	// type = {K,T,S,D,L,0}
	int player;	// player = {0, 1}
	int state;	/* 0 - hasn't moved. 
						1 - has moved during game. */
	int *danger;	//danger[player] = 0  (player cannot attacking this piece)
						//danger[player] = 1 (player can attack this piece)
} BOARD;

//global variables
extern int winner;
extern int player;
extern int check;
extern int castling;
extern int per;

//print functions
void print_matrix(BOARD (*board)[SIZE]);
void init_board(BOARD (*board)[SIZE]);
void usage_tip(void);
void print_danger(BOARD (*board)[SIZE], int k);

//syntax and command check functions
int syntax_check(char *);
int command_check(BOARD (*board)[SIZE], char *);
int pawn_check(BOARD (*board)[SIZE], int, int, int, int);
int knight_check(BOARD (*board)[SIZE], int, int, int, int);
int king_check(BOARD (*board)[SIZE], int, int, int, int);
int rook_check(BOARD (*board)[SIZE], int, int, int, int);
int bishop_check(BOARD (*board)[SIZE], int, int, int, int);
int queen_check(BOARD (*board)[SIZE], int, int, int, int);

//danger remove/update functions
void rm_piece(BOARD *piece);
void rm_danger(BOARD (*board)[SIZE], int, int, int);
void rm_pawn(BOARD (*board)[SIZE], int, int);
void rm_knight(BOARD (*board)[SIZE], int, int);
void rm_king(BOARD (*board)[SIZE], int, int);
void rm_bishop(BOARD (*board)[SIZE], int, int);
void rm_rook(BOARD (*board)[SIZE], int, int);
void rm_queen(BOARD (*board)[SIZE], int, int);

void update_danger(BOARD (*board)[SIZE]);
void update_pawn(BOARD (*board)[SIZE], int, int, int);
void update_knight(BOARD (*board)[SIZE], int, int, int);
void update_king(BOARD (*board)[SIZE], int, int, int);
void update_bishop(BOARD (*board)[SIZE], int, int, int);
void update_rook(BOARD (*board)[SIZE], int, int, int);
void update_queen(BOARD (*board)[SIZE], int, int, int);

//checkmate/stalemate functions
int checkmate(BOARD (*board)[SIZE], int, int);
int can_kmov(BOARD (*board)[SIZE], int, int, int);