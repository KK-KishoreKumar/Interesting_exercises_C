/* Tic-tac-toe game made in C programming language. 
	Autor: Danilo Novakovic */
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 3

int main() {
	char A[MAX_SIZE][MAX_SIZE];
	unsigned *locked = calloc(9, sizeof(unsigned));
	unsigned i, j, turns;
	unsigned player = 0;
	char c = '1';
	unsigned temp = 0, winner = 0;
	for(i = 0; i < MAX_SIZE; i++) {
		for(j = 0; j < MAX_SIZE; j++) {
			A[i][j] = c++;
		}
	}
	//Initializes matrix with values from 1 to 9
	for(i = 0; i < MAX_SIZE; i++) {
			printf("\n\t");
			for(j = 0; j < MAX_SIZE; j++) {
				printf((j+1 < MAX_SIZE) ? "| %c " : "| %c |" , A[i][j]);
			}
	}
	puts("\n");
		
	for(turns = 0; turns < 9; turns++) {
		//Places 'X' or 'O' on the appropriate position in matrix
		player = ++player % 2;
		do{	
			do{
				printf((player == 1) ? "Player 1 (X) turn: " : "Player 2 (O) turn: ");
				scanf(" %c", &c);
			}while(c <= '0' || c > '9'); 		
			temp = c - '0' - 1;
		}while(locked[temp]);
		locked[temp] = 1;
		
		A[temp/3][temp%3] = (player == 1) ? 'X' : 'O';
		
		for(i = 0; i < MAX_SIZE; i++) {
			printf("\n\t");
			for(j = 0; j < MAX_SIZE; j++) {
				printf((j+1 < MAX_SIZE) ? "| %c " : "| %c |" , A[i][j]);
			}
		}
		puts("\n");
	
		//Checks diagonals
		if((A[0][0] == A[1][1] && A[1][1] == A[2][2])||(A[0][2] == A[1][1] && A[1][1] == A[2][0])) 		
			winner = (player == 1) ? 1 : 2;
		else {
		//Checks rows and columns
			for(i = 0; i < MAX_SIZE; i++) {
				if((A[i][0] == A[i][1] && A[i][1] == A[i][2]) || (A[0][i] == A[1][i] && A[1][i] == A[2][i])) {
					winner = (player == 1) ? 1 : 2;
					break;
				}
			}
		}
		if(winner) {
			puts(winner == 1 ? "Player 1 has won!" : "Player 2 has won!");
			return 0;
		}
	}
	puts("It's a draw!");
	return 0;
}