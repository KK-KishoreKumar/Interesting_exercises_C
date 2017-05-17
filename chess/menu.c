#include "menu.h"
//returns 1 if person wants to exit the program
int print_menu() {
	system("tput reset");
	puts("### Chess ###");
	puts("Author: Danilo Novakovic FTN Novi Sad");
	puts("");
	char  option;
	do {		
		puts("Main Menu");
		puts("\t1. Start");
		puts("\t2. Rules | Help");
		puts("\tx  - exit");
		
		printf("\noption: ");
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