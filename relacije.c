/*This program checks Reflexity, Simetry, Antisemtry, Transitivity of relation and also checks if it is a function. 
	Language: Serbian
	Autor: Danilo Novakovic
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_SIZE 20
typedef struct { int x, y;} Relacije;
void convert(char *, int *, int *);
void convertRelacije(Relacije *, int *); 
void refleksivnost(Relacije *, int *, int, int);
void simetricnost_antisimetricnost(Relacije *, int);
void tranzitivnost(Relacije *,  int);
void funkcija(Relacije *, int);

int main(){
    Relacije relacije[MAX_SIZE];
    int nA = 0, n = 0;
	char strA[MAX_SIZE], relacija[MAX_SIZE];
	int A[MAX_SIZE];
	
	printf("Unesite elemente skupa A: "); 
	fgets(strA, MAX_SIZE, stdin);
	convert(strA, A, &nA);
    
    printf("Relacija:");
    convertRelacije(relacije, &n);
    //for(int i = 0; i<n; i++) printf("(%d, %d)\n", relacije[i].x, relacije[i].y);
    refleksivnost(relacije, A, nA, n);    
    simetricnost_antisimetricnost(relacije, n);
    tranzitivnost(relacije,  n);
    funkcija(relacije, n);
    
    exit(0);
}
void funkcija(Relacije *rel, int n){
    int tacnost = 1, i, j;
    for(i = 0; i<n-1; i++){
        for(j = i + 1; j<n; j++){
            if(rel[i].x == rel[j].x){
                tacnost = 0;
                goto Q;
            }
        }
    
    }
    Q: (tacnost == 1) ? (printf("Jeste funkcija\n")) : (printf("Nije funkcija\n"));
}

void tranzitivnost(Relacije *rel, int n){
    int i, j, k, tacnost = 1;
    for(i=0; i<n; i++){
        if(rel[i].x != rel[i].y)       //(x,y)
            for(j = 0; j < n; j++){
                if((rel[j].x == rel[i].y) != rel[j].y){    //(y,z) 
                    for(k =0; k < n; k++)
                        if((rel[k].x == rel[i].x) && (rel[k].y == rel[j].y)){ //(x,z)
                            goto P;
                        }
                    tacnost = 0;
                    goto L;   
                }         
            }
    P:;
    }
            
    L: (tacnost == 1) ? (printf("Jeste tranzitivna\n")) : (printf("Nije tranzitivna\n"));
    
}
void simetricnost_antisimetricnost(Relacije *rel,  int n){
    int i, j, antisimetricnost = 1, tacnost = 1;
    for(i = 0; i < n; i++){
        if(rel[i].x != rel[i].y){
            for(j=0; j< n; j++){
                if((rel[j].x == rel[i].y)&&(rel[j].y == rel[i].x)){
                    tacnost = 2;
                    break;
                }    
            }
            if(tacnost == 2){ 
                antisimetricnost = 0;
                tacnost = 1;
            }    
            else{ 
                tacnost = 0;
                break;
            }    
        }
    }
    (tacnost == 1) ? (printf("Jeste simetricna\n")) : (printf("Nije simetricna\n"));
    (antisimetricnost == 0) ? (printf("Nije antisimetricna\n")) : (printf("Jeste antisimetricna\n"));
}

void refleksivnost(Relacije *rel, int *A, int nA, int n){
    int i, j, k, pomocni[nA], br = 0, ponavljaSe = 0, m = 0;
    for(i=0; i<n; i++){
        for(j=0; j<nA; j++){
            if((rel[i].x == A[j]) && (rel[i].y == A[j])){
                ponavljaSe = 0;
                pomocni[m++] = A[j];
                if(i>0)
                    for(k=0;k<(m-1);k++){
                        if(pomocni[k]==A[j]){
                            ponavljaSe = 1;
                            break;
                        }    
                    }
                if(ponavljaSe == 0)    
                    br++;
            }    
        }
    }
   (br == nA) ? printf("Jeste refleksivna\n") : printf("Nije refleksivna\n");
}

void convertRelacije(Relacije *rel, int *pn){
      char znak = '1';
      int i = 0, j = 0, negativno = 0, br = 0;
      while((znak = getchar())!='\n'){
        if(znak == '-')
            negativno = 1;
        while(znak >= '0' && znak <= '9'){
            br = pow(10, j)*br + (znak - '0');
            j = 1; 
            znak = getchar();
        }        
      
        if(j){
            (i%2==0) ? (rel[*pn].x = pow(-1,negativno)*br) : (rel[(*pn)++].y = pow(-1,negativno)*br);
            if(negativno) negativno ^= negativno; 
            i++;
        }
        j ^= j;
        br ^= br;       
      }
}
void convert(char *strA, int *A, int *pnA){
    int i = 0, j = 0, br = 0, negativno = 0;
    while(strA[i]!='\0'){
        if(strA[i] == '-')
            negativno = 1;
        while(strA[i]>='0' && strA[i]<='9'){
           br = pow(10, j)*br + (strA[i] - '0');
           j = 1;
           i++; 
        }
        if (j){
            A[(*pnA)++] = pow(-1, negativno)*br;
            negativno = 0;
        }	    
        j ^= j;
        br ^= br;
        i++;      
    }
} 
