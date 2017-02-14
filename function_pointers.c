/*Example of pointer to function usage in C programming language.
	Idea taken from K&R Programming in C book, but i made a more modern
	approach that is easier to understand with simpler algorithm.
	(ex. 5-14,15,16,17 applied)*/
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int get_words(char *[], int);
int get_word(char *, int); 					// returns 1  if '\n' is seen
void bubble_sort(void *[], int, int (*)(void *, void *));
void swap(void *[], int i, int);
void print_words(char *[], int);
int numcmp(char *, char *); 			// compares 2 strings nummericaly
int mystrcmp(char *, char *);

#define MAX_WORDS 13
#define MAX_LENGTH 21

int decreasing = 0;      // 0 if increasing, 1 if decreasing   -r flag 
int numeric = 0;          // 1 if numeric sort   -n flag 
int fold = 0;            	  // 1 if not case-sensitive   -f flag 
int directory = 0;        // 1 if directory sort   -d flag 

int main(int argc, char *argv[]) 
{
	char *words[MAX_WORDS];
	int n, i;
	
	while(--argc > 0) {
		++argv;
		if((*argv)[0] == '-') {
			for(i = 1; (*argv)[i] ; i++)
				switch((*argv)[i]) {
					case 'n' : numeric = 1;	   break;
					case 'r' : decreasing = 1; break;
					case 'f' : fold = 1;		   break;
					case 'd' : directory = 1;   break;
					default : puts("usage: ./a.out -dfnr");
								   return EXIT_FAILURE;
				}
		}
		else {
			puts("usage: ./a.out -dfnr");
			return EXIT_FAILURE;
		}
	}
	
	puts("Type text:");
	n = get_words(words, MAX_WORDS);
	
	puts("Sorted:");

	bubble_sort((void **) words, n,  
		(int (*)(void *, void *)) (numeric ? numcmp : mystrcmp));
	
	/*int (*)(void *, void *) part is explicit conversion of functions, kinda like 
		if i would have written (double) 3 /2 expression. */
		
	print_words(words, n);
	
	return EXIT_SUCCESS;
}

void bubble_sort(void *A[], int n, int (*func) (void *, void *)) 
{
	int i, j, sorted;
	
	for(i = 0; i < n; i++) {
		sorted = 1;
		for(j = 0; j < n - 1; j++) 
			switch(decreasing) {
					case 0 : if((*func)(A[j], A[j+1]) > 0) { 
									swap( A,  j,  j+1);
									sorted = 0;
								 } break;
					case 1 : if((*func)(A[j], A[j+1]) < 0) { 
									swap( A,  j,  j+1);
									sorted = 0;
								 } break;		
			}
			
		if(sorted) 
			return;
	}
}
void swap(void *A[], int i, int j)
{
	void *temp;
	
	temp = A[i];
	A[i] = A[j];
	A[j] = temp;
}

int mystrcmp(char *s1, char *s2) 
{
	//i avoided white chars in get_word function anyway.
	
	while( fold ? (tolower(*s1) == tolower(*s2)) : *s1 == *s2) {
		if(!*s1) 
			return 0;
		s1++;
		s2++;
		if(directory) {
			while(!isalpha(*s1) && !isdigit(*s1) && *s1)  
				s1++; 
			while(!isalpha(*s2) && !isdigit(*s2) && *s2)
				s2++;
		}
	}
	return  fold ? (tolower(*s1) - tolower(*s2)) : *s1 - *s2;
}

int numcmp(char *s1, char *s2)
{
	double v1, v2;
	
	v1 = atof(s1);
	v2 = atof(s2);
	
	if(v1 > v2) 
		return 1;
	else if(v1 < v2) 
		return -1;
	else 
		return 0;
}

int get_words(char *words[], int n) 
{
	int i = 0;
	char *p;
	
	do {
		p = malloc(sizeof(char)*MAX_LENGTH);
		if(get_word(p, MAX_LENGTH)) {
			words[i++] = p;
			break;
		}
		words[i] = p;
	} while(++i < n);
	
	return i;
}

int get_word(char *word, int n) 
{
	while(isspace(*word = getchar())) //avoids white spaces until first non-white char 
		if(*word == '\n') {
			*word = '\0';
			return 1;
		}
	
	while(--n > 0) {
		if(isspace(*word)) 
			break;
		*++word = getchar();		
	}
	char c = *word;
	*word = '\0';
	return c == '\n';
}

void print_words(char *A[], int n) 
{
	while(--n >= 0) 
		printf((n > 0) ? "%s " : "%s\n", *A++);
}