/* My humble alghorithm for finding prime numbers in certain interval*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned main(int nb_arg, char *args[]) {
    FILE *pout = fopen("primes.txt", "w");
	if(nb_arg < 3) {
		puts("usage: <program_name> <lower_bound> <higher_bound>");
		return EXIT_FAILURE;
    }
    unsigned upper_bound = atoi(args[2]),  lower_bound = atoi(args[1]);
	if(upper_bound < lower_bound) {
		puts("error: <higher_bound> must be higher then <lower_bound>");
		return EXIT_FAILURE;
	}
    unsigned max_test = (unsigned) sqrt((double)upper_bound) + 1;
	printf("max_test = %u\n", max_test);
	unsigned primes[max_test];
	unsigned *left, *right = primes;
    *right++ = 2;
    *right++ = 3;
    *right++ = 5;
     *right++ = 7;
     *right = 11;
//TODO: Find max_test primes  
    unsigned temp, truth;
    unsigned square;
	left = primes;
	while(left <= right) {
		if(lower_bound < *left) fprintf(pout, "%u\n", *left);
		left++;
	}
	if(lower_bound < *right) lower_bound = (*right)+2;
	for(temp = lower_bound; temp < upper_bound; temp+=2) {
        square = (unsigned)sqrt((double)temp) + 1;
        truth ^= truth;
		truth++;	
        for(left = &primes[1]; *left < square; left++) {
		   if(left > right) break;
		   if(temp%(*left) == 0) { 
                truth ^= truth;
                break;
            }
        }
        if(truth) {
            if(temp < max_test) *++right = temp;
			fprintf(pout, "%u\n", temp);
        }
    }
	fclose(pout);
	
    return EXIT_SUCCESS;
}
