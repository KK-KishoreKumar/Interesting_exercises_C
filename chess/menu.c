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
#include "menu.h"
//returns 1 if person wants to exit the program
int print_menu() {
	system("tput reset");
	puts("\x1b[32;4;1m### Chess ###\x1b[0m");
	puts("\x1b[35;1m Author: Danilo Novakovic FTN Novi Sad\x1b[0m");
	puts("");
	char  option;
	do {		
		puts("\x1b[4mMain Menu\x1b[0m");
		puts("\t1. Start");
		puts("\t2. Rules | Help");
		puts("\x1b[31;1m\tx  - exit\x1b[0m");
		
		printf("\n\x1b[4mOption\x1b[0m: ");
		scanf(" %c", &option);
		switch(option) {
			case '2': chess_rules(); break;
			case 'x': case 'X': return 1; break;
		}
		system("tput reset");
	}while(option != '1');
	return 0;
}
void chess_rules() {
	FILE *pin = fopen("chess_manual.txt", "r");
	if(pin == NULL) {
		puts("error: failed to open chess_manual.txt");
		exit(EXIT_FAILURE);
	}
	system("tput reset");
	char c = 0;
	while((c = fgetc(pin))!=EOF) {
		putchar(c);
	}
	putchar('\n');
	do{
		printf("\nReturn? (Y/n) : ");
		scanf(" %c", &c);
		
	}while(c != 'y' && c != 'Y' );
	fclose(pin);
}