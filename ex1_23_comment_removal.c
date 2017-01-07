/*This program removes comments from a file (.c , .txt, etc.) (Exercise 1-23 from K&R Programming in C 2nd Edition)*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define MAX_NAME 21
#define IN 1
#define IN_SECOND 2
#define OUT 0

void gets_s(char *string, int n){
	char c;
	int i = -1;
	while((c = getchar()) !=EOF && c != '\n'){
		string[++i] = c;
	}
}
FILE *fopen_s(char *name, char *mode){
	FILE *pin = fopen(name, mode);
	if(pin == NULL){
		perror("ERROR");
		exit(1);
	}
	return pin;
}

int main(){
	char file_name[MAX_NAME];
	size_t data_in_file;
	
	printf("Name of file: ");
	gets_s(file_name, MAX_NAME);
	
	FILE *pin = fopen_s(file_name, "r");
	
	fseek(pin, 0, SEEK_END);
	long file_size = ftell(pin);
	rewind(pin);
	
	char *buffer = (char *) malloc(sizeof(char)*file_size);
	if(buffer == NULL){
		perror("ERROR");
		exit(2);
	}
	
	data_in_file = fread(buffer, 1, file_size, pin);
	
	if(data_in_file!=file_size){
		perror("ERROR");
		exit(3);
	}
	
	fclose(pin);

	FILE *pout = fopen_s(file_name, "w");
	
	puts("Removing comments...");
	
	int i = 0, state = OUT;
	while(i < data_in_file && buffer[i] != '\0'){
		if(state == OUT && buffer[i] == '/' && i + 1 < data_in_file && buffer[i + 1] == '*'){ 
			i = i + 2;
			state = IN;
		}
		else if(state == IN && buffer[i] == '*' && i + 1 < data_in_file && buffer[i + 1] == '/'){
			i = i + 2;
			state = OUT;
		}
		else if(state == OUT && buffer[i] == '/' && i + 1 < data_in_file && buffer[i + 1] == '/'){
			i = i + 2;
			state = IN_SECOND;
		}
		if(state == IN_SECOND && buffer[i] == '\n') {
			i++;
			state = OUT;
		}
		if(state == OUT) fputc(buffer[i], pout);
		i++;
	}
	fclose(pout);

	puts("Program has finished successfully.");
	
	return EXIT_SUCCESS;
}