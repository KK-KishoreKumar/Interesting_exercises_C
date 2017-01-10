/* This program converts a string of positive hexadecimal digits (including an optional 0x or 0X)
	into its equivalent decimal value. The allowable digits are 0 through 9, a through f, 
	and A through F, and one dot allowed. (This is my modification of the exercise 2-3 from
	the book Programming in C 2nd Edition by Dennis Ritchie and Brian Kernighan)*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_BUFFER 20

int find_dot(char *);
int is_upper(char);
int is_digit(char);
int is_hexa(char);

void print_error(char);
int get_hexa(char *, int );
double htof(char *, int);

int main(){
	char buffer[MAX_BUFFER];
	
	printf("Hexadecimal: ");
	int n = get_hexa(buffer, MAX_BUFFER);
	printf("Decimal: %lf\n",	htof(buffer, n));
	
	return EXIT_SUCCESS;
}

/*Expects parray to be string with a null terminator ("\0").
	Returns index of the element with dot in it. Returns -1 if not found.*/
int find_dot(char *parray){
	int i = -1;
	if(parray!=NULL)
		while(parray[++i] != '.')
			if(parray[i] == '\0'){
				i = -1;
				break;
			}
	return i;
}
int is_upper(char c){
	if(c >= 'A' && c <= 'Z') return 1;
	else return 0;
}
int is_digit(char c){
	if(c >= '0' && c <= '9') return 1;
	else return 0;
}
int is_hexa(char c){
	if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) return 1;
	else return 0;
}
void print_error(char c){
	printf("ERROR: '%c' is not a hexidecimal number.", c);
	exit(EXIT_FAILURE);
}
/*Scans hexadecimal number (in form of string) from stdin and if the user input is not
	hexidecimal it will exit with an appropriate error message. Return value is array length.*/
int get_hexa(char *parray, int n){
	int i = 0, dot_number = 0;
	if(parray != NULL){
			char c = getchar();		
			
			if(c == '0'){
				c = getchar();
				if(c != 'x' && c != 'X' && !is_hexa(c)) print_error(c);
				else if(c == 'x' || c == 'X') c = getchar();
			}
			while(c != EOF && i < n){
				if(is_hexa(c)) parray[i++] = c;
				else if(c == '\n' || c == 32 || c == '\t'){
					parray[i] = '\0';
					break;
				}
				else if(c == '.' && dot_number <= 1){
					dot_number++;
					parray[i++] = c;
				}
				else print_error(c);
				c = getchar();
			}
	}
	return i;
}
/*	2AB.F8 for example is converted to decimal by following order:
	(Hex: 1 - 9 the same. A = 10, B = 11, C = 12, D = 13, E = 14, F = 15);
	
	pre dot: 2*(16^2) + A*(16^1) + B*(16^0) = 683 
	post dot: F*(16^(-1)) + 8*(16^(-2)) = 96875
	result = pre dot + post dot = 683.96875 
*/
double htof(char *parray, int n){
	int i, j = find_dot(parray), dg = 0;
	double val = 0;
	j == -1 ? (i = n-1) : (i = j - 1);
	if(j != 0){
		do{
			if(is_digit(parray[i]))		val += (parray[i] - '0')*pow(16, dg++);
			else if(is_upper(parray[i]))	val += (parray[i] - 'A' + 10)*pow(16, dg++);
			else	val += (parray[i] - 'a' + 10)*pow(16, dg++);
		}while(--i >= 0);
	}
	if(j != -1){
		dg = 0;
		while(++j < n){
			if(is_digit(parray[j]))	val += (parray[j] - '0') * pow(16, --dg);
			else if(is_upper(parray[j]))	val += (parray[j] - 'A' + 10) * pow(16, --dg);	
			else	val += (parray[j] - 'a' + 10) * pow(16, --dg);
		}
	}
	return val;
}