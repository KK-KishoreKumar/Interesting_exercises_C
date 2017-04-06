/*Exercise from Codility. Finds max number of successive zeros in
	binary version of integer (zeros between ones only (ex.1 : 1001000 => max = 2).
	(000 is not counted because it isn't between ones in ex.1).
	Time complexity:  O(n)
	Space complexity: O(1)
*/
#include <stdio.h>

int main(){
	int n, local_max, max;
	do{
		local_max = max = 0;
		printf("Number = ");
		scanf(" %d", &n);
		getchar(); //just in case you type in 1 letter ('k' for example) to avoid infinite loop
						//eats '\n' (newline) character otherwise. 
		
		/*Idea:  1001 & 0001 <=> 0001  (the last right binary digit of 1001 (number 9) is 1) 
					 1001>>=1   <=>  0100 (moves all bin digits to the right by 1 spot)
					 0100 & 0001 <=> 0000  (the last right binary digit of 0100 (number 4) is 0)
					etc. until 0000 (which is 0)
					
					(<=> just a math sign that means is equivalent)
		*/
		while((n&1)==0){
			n>>=1;
		}
		while(n > 0){
			if(n & 1){
				if(local_max > max) max = local_max;
				local_max = 0;
				printf("1");
			}
			else{
				local_max++;
				printf("0");
			}
			n >>= 1;
		}
		printf("\nMax number of successive zeros is  %d.\n", max);

	}while(n >= 0); //This program will run as long as you type positive intigers
	
	return 0;
}