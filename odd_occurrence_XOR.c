/*This is exercise from Codility that finds 1 element with odd time occurrence 
	in an array where other elements occur 1 time via XOR.
	Time complexity: O(n);
	Space complexity: O(1);
*/
#include <stdio.h>

/*Idea: 1001 ^ 1001  <=> 0000  (Two same annul each other)
			1001 ^ 0110 <=> 1111 
			1111 ^ 1001 <=> 0110 
			conclusion: ((1001^1001)^0110 <=> 1001^(1001^0110))  <=> XOR is associative.
*/
int getOddOccurrence(int ar[], int n){
	int i;
	int res = 0;
	for(i = 0; i < n; res = res ^ ar[i++]); //first res = res ^ar[i] is done and then i++ after
	return res;
}

int main(){
	//int ar[] = {2, 3, 5, 4, 5, 2, 4, 3, 5, 2, 4, 4, 5, 7, 2}; <= works aswell.
	int ar[] = { 9, 3, 9, 3, 9, 7, 9};
	int n = sizeof(ar)/sizeof(ar[0]);
	printf("n = %d , size of ar = %lu, size of ar[0] = %lu\n", n , sizeof(ar), sizeof(ar[0]));
	printf("ans = %d\n", getOddOccurrence(ar, n));
	return 0;
}