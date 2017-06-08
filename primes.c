/* My humble alghorithm for finding prime numbers in certain interval.
   It prints aprox 125 milion primes per minute. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned main(int nb_arg, char *args[]) {
	FILE *pout = fopen("primes.txt", "w");
	if(nb_arg < 3) {
		puts("usage: <program_name> <lower_bound> <higher_bound>");
		return EXIT_FAILURE;
    	}
	unsigned upper_bound = strtoul(args[2], NULL, 10),  lower_bound = strtoul(args[1], NULL, 10);
	if(upper_bound < lower_bound) {
		puts("error: <higher_bound> must be higher then <lower_bound>");
		return EXIT_FAILURE;
	}
	unsigned max_test = (unsigned) sqrt((double)upper_bound) + 1;
	unsigned primes[max_test];
	unsigned *left, *right = primes;
	*right++ = 2;
	*right++ = 3;
	*right++ = 5;
 	*right++ = 7;
	*right = 11;
	
//TODO: Find max_test primes  
    unsigned temp;
	unsigned char truth;
   	unsigned square;
	left = primes;
	while(left <= right) {
		if(lower_bound < *left) fprintf(pout, "%u\n", *left);
		left++;
	}
	if(lower_bound < *right) lower_bound = (*right)+2;
	else {
		//TODO: Setup min needed test primes in array 
		for(temp = (*right)+2; temp < lower_bound; temp+=2) {
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
				else break; // if curr prime is >= max test prime then end the loop
			}
		}
	}
	/*
	TODO: Test prime numbers from <lower_bound> to <upper_bound>
	note: Before start of loop i change number to odd number (if it is even).
	(this way i double the speed of alghorithm by checking only odd numbers (i+=2), instead of waisting time on even numbers 
	(all even numbers are divisable by 2)
	*/
	if((lower_bound & 1) == 0) lower_bound++; 
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
