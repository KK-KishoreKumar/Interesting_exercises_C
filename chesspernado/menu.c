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
*/
#include "menu.h"
#include "tutorial.h"
//returns 1 if person wants to exit the program
int print_menu() {
	system("tput reset");
	char  option;
	do {		
		puts("\n\x1b[32;4;1m### Chesspernado ###\x1b[0m");
		puts("\x1b[35;1m Author: Danilo Novakovic FTN Novi Sad\x1b[0m");
		puts("");
		puts("\x1b[37;4mMain Menu\x1b[0m");
		puts("\t\x1b[37m1. New Game\x1b[0m");
		puts("\t\x1b[37m2. Rules | Help\x1b[0m");
		puts("\t\x1b[37m3. Difficulty\x1b[0m");
		puts("\t\x1b[32;1m4. Tutorial\x1b[0m");
		puts("\t\x1b[33;1m5. BONUS\x1b[0m: Standard PvP Chess (2 players)");
		puts("\x1b[31;1m\tx  - Exit\x1b[0m");
		
		printf("\n\x1b[37;4mOption\x1b[0m: ");
		scanf(" %c", &option);
		getchar();
		switch(option) {
			case '2': chesspernado_rules(); break;
			case '3': set_difficulty(); break;
			case '4': start_tutorial(); end = 0; break;
			case '5': system("./chess/a.out");  break;
			case 'x': case 'X': return 1; break;
		}
		system("tput reset");
	}while(option != '1');
	return 0;
}
void set_difficulty() {
	char c;
	system("tput reset");
	puts("Select difficulty:");
	puts("\t\x1b[32;1m1. Easy\x1b[0m");
	puts("\t\x1b[31;1m2. Hard\x1b[0m");
	do{
		printf("\n\x1b[37;4mOption\x1b[0m: ");
		scanf(" %c", &c);
		getchar();
	}while(c != '1' && c!='2');
	
	difficulty = c - '0';
	puts("Difficulty set successfully!");
	wait(2);
}
void chesspernado_rules() {
	FILE *pin = fopen("chesspernado_manual.txt", "r");
	if(pin == NULL) {
		puts("error: failed to open chesspernado_manual.txt");
		exit(EXIT_FAILURE);
	}
	system("tput reset");
	char c = 0, prev = 0;
	unsigned char alpha= 0;
	while((c = fgetc(pin))!=EOF) {
		if(c!='~') {
			if(prev == '@' && (alpha&1)) {
				switch(c) {
					case '1': printf("\x1b[31;1m"); continue; break;
					case '2': printf("\x1b[32;1m"); continue; break;
					case '3': printf("\x1b[33;1m"); continue; break;
					case '4': printf("\x1b[34;1m"); continue; break;
					case '5': printf("\x1b[35;1m"); continue; break;
					case '6': printf("\x1b[36;1m"); continue; break;
					case '7': printf("\x1b[37;1m"); continue; break;
				}
			}
			if(c=='@') {
				printf((alpha&1) == 0 ? "\x1b[35;1m" : "\x1b[0m");
				alpha^=1;
				prev = c;
				continue;
			}
			putchar(c);
			prev = c;
		}
		else {
			do{
				printf("\nContinue (Y/n) ? ");
				scanf("%c", &c);
				if(c != '\n') getchar();
			}while(c!='y' && c!='Y' && c!='n' && c!='N' && c!='\n'); 
			if(c == 'n' || c == 'N') return;
			system("tput reset");
		}
	}
	putchar('\n');
	do{
		printf("\nReturn? (Y/n) : ");
		scanf("%c", &c);
		if(c!='\n') getchar();
	}while(c != 'y' && c != 'Y' && c!='\n');
	fclose(pin);
}