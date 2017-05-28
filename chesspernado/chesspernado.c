/*
	chesspernado
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
 
 R - suicide rook
 B - bishop 
 N - knight 
 P - pawn
 K - happy king
 */
#include "chesspernado.h"
#include "waves.h"
#include "ai_movement.h"
#include "special_moves.h"
#include "menu.h"
#define MAX_NAME 25
int player = 0;
int per = 1;	//if !0 then error msg will be print, else it will not.
int end = 0;
int difficulty = 1;  // 1 - easy (default);  2 - hard

int main () {
	if(print_menu())
		return 0;
	int i = 0, i1, i2, j1, j2;
	int wave = 1, moves = 0;
	int points = 0;
	player = 0;
	per = 1;
	int enemy = player^1;
	char buffer[MAX_BUFFER];
	char *s;
	BOARD board [HEIGHT][WIDTH];
	
	TYPE inf;
	NODE *pcurr = NULL;
	
	init_board(board);
	
	//Initial board setup
	board_setup(board);
	
	end = 0; 
			
	do {
SKIPWV:
		system("tput reset");
		printf("\x1b[35;1mWAVE %d!\x1b[0m\nBattle is about to beginn...\n", wave);
		
		wait(2);
		if(wave < 3)
			generate_wave1(board, (wave<<difficulty)+2); 
		else if(wave < 5)
			generate_wave2(board, 3+difficulty, wave%3+difficulty);
		else if(wave > 10) 
			generate_wave2(board, 9, 5+difficulty);
		else if(wave%5 == 0)
			generate_wave1(board, wave+ 3 + difficulty);
		else
			generate_wave2(board, random_number(4,7+difficulty), random_number(2,3+difficulty));
			
		do{
			if(end) break;
			
			print_matrix(board);
			printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
			do {
				i = 0;
				printf(BLUE "Player 1: " CL_RESET);
				fgets(buffer, MAX_BUFFER, stdin);
				if(buffer[0] == '/') {
					if(!strncmp(buffer, "/skip", 5)) {
						boardcpy(inf.board, board);
						inf.points = points;
						list_push(&pcurr, inf);
						goto SKIP;
					}	
					else if(!strncmp(buffer, "/undo",5)) {
						if(!undo(board, &pcurr, &points)) {
							puts("error: undo unavailable.");
						}
						else {
							print_matrix(board);
							printf("\n\x1b[35;1mScore\x1b[0m: %d\n\n", points);
						}
						continue;
					}
					else if(!strncmp(buffer, "/revive", 7)) {
						if(!revive(board, &points)) {
							puts("error: revive unavailable");
						}
						else {
							print_matrix(board);
							printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
						}
						continue;
					}
					else if(!strncmp(buffer, "/buy", 4)) {
						buy(board, &points);
						print_matrix(board);
						printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
						continue;
					}
					else if(!strncmp(buffer, "/help", 5)) {
						chesspernado_rules();
						print_matrix(board);
						printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
						continue;
					}
					else if(!strncmp(buffer, "/end", 4) || !strncmp(buffer, "/exit",5))  {
						exit(EXIT_SUCCESS);
					}	
					//2 ifs below are cheat codes meant for those who like to exploit games
					else if(!strncmp(buffer, "/HESOYAM", 8)) {
						points+=1000;
						print_matrix(board);
						printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
						continue;
					}
					else if(!strncmp(buffer, "/wvskip", 7)) {
						wave++;
						delete_full_list(&pcurr);
						rm_wave(board);
						goto SKIPWV;
					}
				}
				i = syntax_check(buffer);
				if(!i) {
					usage_tip();
					continue;
				}
				i = command_check(board, buffer);
			}while(!i);
			
			s = &buffer[1];
			j1 = *s++ - 'a';
			i1 = *s++ - '0';
			if(*s >= '0' && *s <='2') i1 = HEIGHT - (i1*10 + (*s++ - '0'));
			else i1 = HEIGHT - i1;
			
			j2 = *s++ - 'a';
			i2 = *s++ - '0';
			
			if(*s >= '0' && *s <='2') 	i2 = HEIGHT - (i2*10 + (*s++ - '0'));
			else i2 = HEIGHT - i2;
		
			boardcpy(inf.board, board);
			inf.points = points;
			list_push(&pcurr, inf);
			
			if(board[i2][j2].player == enemy) {
				switch(board[i2][j2].type) {
					case 'p': points+=6; break;
					case 'b': points+=12; break;
				}
			}
			
			mov_figure(board, i1, j1, i2, j2);

SKIP:;			
			suicide_rook(board, player);
			
			print_matrix(board);
			printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
			if(is_empty(board, enemy))
				break;
			
			puts("AI turn...");
			wait(1);
			
			if(is_empty(board, player) || is_full(board)) 
				end++;
			else if(!ai_move(board, &points)) {
				if(wave < 5) generate_wave1(board, 4 - wave);
				else generate_wave1(board, 3);
			}
			
			if(end) {
				print_matrix(board);
				do{
					printf("AI has achieved victory, do you want to atempt undo of your last move (y/n) ? ");
					scanf(" %c", &buffer[0]);
					getchar();
					switch(buffer[0]) {
						case 'y' : case 'Y': if(undo(board, &pcurr, &points)) end ^= end; 
													else puts("error: undo unavailable");
													buffer[0] = 'y'; 
						break;
						case 'N': buffer[0] = 'n'; break;
					}
				}while(buffer[0]!='y' && buffer[0]!='n');
			}
		}while(!end); 
		
		delete_full_list(&pcurr);
		
		if(!end) {
			//prepeare for new wave
			moves = 3;
			printf("\x1b[35;1mWave %d has finished!\x1b[0m\n", wave);
			do{
				printf("You have \x1b[35;1m%d \x1b[0m moves left until new wave.\n", moves);
				do {
					i = 0;
					printf(BLUE "Player 1: " CL_RESET);
					fgets(buffer, MAX_BUFFER, stdin);
					if(buffer[0] == '/') {
						if(!strncmp(buffer, "/end", 4) || !strncmp(buffer, "/exit",5))  {
							exit(EXIT_SUCCESS);
						}
						if(!strncmp(buffer, "/skip", 5)) {
							printf("Are you sure you want to skip? (y/n) : ");
							scanf(" %c", &buffer[0]);
							getchar();
							if(buffer[0] == 'y' || buffer[0] == 'Y') 
								goto SKIP2;
							continue;
						}
						if(!strncmp(buffer, "/revive", 7)) {
							if(!revive(board, &points)) {
								puts("error: revive unavailable");
							}
							else {
								print_matrix(board);
								printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
								printf("You have \x1b[35;1m%d \x1b[0m moves left until new wave.\n", moves);
							}
							continue;
						}
						if(!strncmp(buffer, "/buy", 4)) {
							buy(board, &points);
							print_matrix(board);
							printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
							continue;
						}
						if(!strncmp(buffer, "/help", 5)) {
							chesspernado_rules();
							print_matrix(board);
							printf("\n\x1b[35;1mScore\x1b[0m: %d\n\n", points);
							printf("You have \x1b[35;1m%d \x1b[0m moves left until new wave.\n", moves);
							continue;
						}
						if(!strncmp(buffer, "/undo",5)) {
							if(!undo(board, &pcurr, &points)) {
								puts("error: undo unavailable.");
							}
							else {
								print_matrix(board);
								printf("\n\x1b[35;1mScore\x1b[0m: %d\n\n", points);
								printf("You have \x1b[35;1m%d \x1b[0m moves left until new wave.\n", moves);
								moves++;
							}
							continue;
						}
					}
					i = syntax_check(buffer);
					if(!i) {
						usage_tip();
						continue;
					}
					i = command_check(board, buffer);
				}while(!i);
				
				boardcpy(inf.board, board);
				inf.points = points;
				list_push(&pcurr, inf);
				
				s = &buffer[1];
				j1 = *s++ - 'a';
				i1 = *s++ - '0';
				if(*s >= '0' && *s <='2') i1 = HEIGHT - (i1*10 + (*s++ - '0'));
				else i1 = HEIGHT - i1;
				
				j2 = *s++ - 'a';
				i2 = *s++ - '0';
				
				if(*s >= '0' && *s <='2') 	i2 = HEIGHT - (i2*10 + (*s++ - '0'));
				else i2 = HEIGHT - i2;
				
				mov_figure(board, i1, j1, i2, j2);
		
				print_matrix(board);
				printf("\x1b[35;1mScore\x1b[0m: %d\n\n", points);
			}while(--moves > 0);
SKIP2:;
			delete_full_list(&pcurr);
			wave++;
		}
	}while(!end);
	
	print_matrix(board);

	puts("\n\t\t\x1b[35;1mGAME OVER!\x1b[0m");
	printf("\n\x1b[35;1mScore\x1b[0m: %d\n\n", points);
	printf("Name: ");
	char name[MAX_NAME];
	fgets(name, MAX_NAME, stdin);
	
	FILE *pout = fopen("scoreboard.txt", "a");
	if(pout == NULL) {
		printf("error: failed to open scoreboard.txt\n");
		return EXIT_FAILURE;
	}
	fprintf(pout, "name: %s | points: %d | wave: %d\r\n", name, points, wave);
	fclose(pout);
	
	return EXIT_SUCCESS;
}



