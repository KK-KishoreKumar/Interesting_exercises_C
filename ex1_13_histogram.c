/*This program prints out horizontal histogram consisting of lengths of the words to stdout. 
	(Exercise 1-13 K&R Programming in C  2nd Edition)*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_WORDS 15
#define MAX_FREQS 3
#define IN 1
#define OUT 0

int main(){
	int lengths[MAX_WORDS], nb_words = -1, state = OUT, max_freq = 0, i;
	int frequency[MAX_FREQS];
	char c;
	
	/*Sets new subarrays to zero so i don't accidently get some unwanted results*/
	for(i = 0; i < MAX_FREQS; i++) frequency[i] = 0;
	for(i = 0; i < MAX_WORDS; i++) lengths[i] = 0;
	puts("**Word-length historgraph in C**");
	printf("Input words: ");
	
	/*Analizes length of each word char by char*/
	while((c = getchar())!= '\n' && c != EOF){
		while((c >= 'a' && c<='z') || (c >= 'A' && c <= 'Z')){
			if(!state){ 
				nb_words++;
				if(nb_words > MAX_WORDS){ 
					puts("ERROR:  Maximum number of words exceeded.");
					exit(EXIT_FAILURE);
				}
				state = IN;
			}
			lengths[nb_words]++;
			c = getchar();
		}
		if(c=='\n' || c==EOF) break;
		state = OUT;
	} 
	
	/*Calculates how frequent each word appears based on specific class (I chose to calculate based on the class at line #51 ).*/
	for(i = 0; i <= nb_words; i++){
		if(lengths[i] >= 1 && lengths[i] <= 4){
			frequency[0]++;
			if(frequency[0] > max_freq) max_freq = frequency[0];
		}
		else if(lengths[i] >= 5 && lengths[i] <= 7){
			frequency[1]++;
			if(frequency[1] > max_freq) max_freq = frequency[1];
		}
		else {
			frequency[2]++;
			if(frequency[2] > max_freq) max_freq = frequency[2];
		}
	}
	/*I print this so the user knows whats going on*/
	puts("Classes");
	puts("--------");
	puts("1 - 4");
	puts("5 - 7");
	puts("8 - Inf");
	puts("--------");
	putchar('\n');
	
	/*Prints the histogram to the stdout*/
	for(i = max_freq; i > 0; i--){
		printf("%2d |", i);
		(frequency[0] >= i) ? printf("  *|") : printf("   |");
		(frequency[1] >= i) ? printf("  *|") : printf("   |");	
		(frequency[2] >= i) ? printf("  *|") : printf("   |");
		putchar('\n');
	}
	printf("     1	 5   8\n");
					     
	return EXIT_SUCCESS;
}