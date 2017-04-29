/*This program translates a text from serbian to english using AVL tree structure. 
	'recnik.txt' is vocabulary text file that contains serbian - english word pairs.
	'original.txt' is file that contains sentences in serbian. 
	'prevod.txt' is output file which will have sentences translated to english.
(This was a homework from my college FTN - Novi Sad)
Autor: Danilo Novakovic
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORD 21
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef struct vocabulary_st {
	char key[MAX_WORD];
	char val[MAX_WORD];
	int used;
} TYPE;
typedef struct node_st {
	TYPE inf;
	int height;
	struct node_st *pleft, *pright;
} NODE;

FILE *fopen_s(const char *, const char *);
int is_alpha(char);
char read_words(FILE *, TYPE *);
NODE *read_file(FILE *, NODE *);
char to_lower(char c);
char to_upper(char c);
void translate(FILE *, FILE *, NODE *);
NODE *find_key(NODE *, char *);
void print_node(FILE *, NODE *);
void print_tree(FILE *, NODE *);
int count_unused(NODE *);
NODE *delete_used(NODE *);
NODE *find_unused(NODE *);


NODE *make_node (TYPE);
int height(NODE *);
int get_balance(NODE *);
NODE *left_rotate(NODE *);
NODE *right_rotate(NODE *);
NODE *insert(NODE *, TYPE);
NODE *find_min_node(NODE *);
NODE *delete_node(NODE *, TYPE);
void *delete_tree(NODE *);

int main(int nb_args, const char **args){
	if(nb_args != 3) {
		printf("USAGE: %s VOCABULARY_FILE FILE_TO_TRANSLATE\n", args[0]);
		exit(EXIT_FAILURE);
	}
	NODE *proot = NULL;	
	FILE *pin = fopen_s(args[1], "r");
	proot = read_file(pin, proot);
	fclose(pin);
	
	pin = fopen_s(args[2], "r");
	FILE *pout = fopen_s("prevod.txt", "w");
	translate(pin, pout, proot);
	fclose(pin);
	fclose(pout);
	
	proot = delete_used(proot);
	
	print_tree(stdout, proot);
	
	proot = delete_tree(proot); //returns NULL 
	
	return EXIT_SUCCESS;
}
int count_unused(NODE *proot) {
	if(proot!=NULL) {
		if(proot->inf.used) 
			return count_unused(proot->pleft) +
						count_unused(proot->pright);
		else 
			return 1 + count_unused(proot->pleft) 	+ 
							count_unused(proot->pright);
	}
	else return 0;
}
NODE *delete_used(NODE *proot){
	int n = count_unused(proot), i;
	NODE *ptemp;
	for(i = 0; i < n; i++) {
		ptemp = find_unused(proot);
		if(ptemp != NULL) 
			proot = delete_node(proot, ptemp->inf);
		else break; 
	}
	return proot;
}
NODE *find_unused(NODE *proot) {
	if(proot != NULL) {	
		NODE *ptemp = proot;
		if(ptemp->inf.used != 1) return ptemp;
		
		ptemp = find_unused(proot->pleft);
		if(ptemp!=NULL) return ptemp;
		
		ptemp = find_unused(proot->pright);
		if(ptemp!=NULL) return ptemp;
	}
	return NULL;
}

FILE *fopen_s(const char *name, const char *mode) {
	FILE *pf = fopen(name, mode);
	if(pf == NULL) {
		printf("ERROR: File '%s' could not be opened.\n", name);
		exit(13);
	}
	return pf;
}
int is_alpha(char c) {
	if((c >='a' && c<='z') || (c>='A' && c<='Z')) return 1;
	else return 0;
}
char read_words(FILE *pin, TYPE *temp) {
	char c;
	int i = 0, word = 0;
	do{
		c = fgetc(pin);
		if(is_alpha(c)) {
			if(!word) {
				do{
					temp->key[i++] = c;
				}while(is_alpha(c = fgetc(pin)));
				temp->key[i] = '\0';
			}
			else {
				do{
					temp->val[i++] = c;
				}while(is_alpha(c = fgetc(pin)));
				temp->val[i] = '\0';
			}
			i = 0;
			word++;
		}
	}while(word != 2 &&  i < MAX_WORD && c != EOF); 
	return c;
}
NODE *read_file(FILE *pin, NODE *proot){
	TYPE temp;
	temp.used = 0;
	
	while(read_words(pin, &temp)!=EOF) {
		proot = insert(proot, temp);
	}
	return proot;
}
char to_lower(char c) {
	if(c >= 'A' && c <= 'Z') c += 32;
	return c;
}
char to_upper(char c) {
	if(c >= 'a' && c <= 'z') c -= 32;
	return c;
}
void translate(FILE *pin, FILE *pout, NODE *proot) {
	int i = 0;
	char buffer[MAX_WORD], original[MAX_WORD];
	char c;
	NODE *temp; 
	
	do {
		c = fgetc(pin);
		if(is_alpha(c)) {
			do {
				original[i] = c;
				buffer[i++] = to_lower(c);
			}while(is_alpha(c = fgetc(pin)));
			original[i] = buffer[i] = '\0';
			i = 0;
			temp = find_key(proot, buffer);
			if(temp != NULL) {
				if(original[i] >= 'A' && original[i] <= 'Z')  
					fprintf(pout, "%c", to_upper(temp->inf.val[i++]));
				while(temp->inf.val[i] != '\0') {
					fprintf(pout, "%c", temp->inf.val[i++]);
				}
				i = 0;
			}
		}
		if(c != EOF) fprintf(pout, "%c", c);
	}while(c != EOF);
}
NODE *find_key(NODE *proot, char *buffer) {
	NODE *pcurr = proot;
	int equality;
	while(pcurr != NULL) {
		equality = strcmp(pcurr->inf.key, buffer);
		if(equality == 0) {
			pcurr->inf.used = 1;
			break;
		}
		else if(equality > 0) pcurr = pcurr->pleft;
		else pcurr = pcurr->pright;
	}
	return pcurr;
}
void print_node(FILE *pout, NODE *proot) {
	fprintf(pout, "%s - %s\n\r", proot->inf.key, proot->inf.val);
}
void print_tree(FILE *pout, NODE *proot) {
	if(proot!=NULL) {
		print_tree(pout, proot->pleft);
		print_node(pout, proot);
		print_tree(pout, proot->pright);
	}
}
NODE *make_node(TYPE inf) {
	NODE *pnew = malloc(sizeof(NODE));
	if(pnew == NULL) {
		puts("ERROR: Out of RAM!");
		exit(7);
	}
	pnew->height = 1;
	pnew->pleft = pnew->pright = NULL;
	pnew->inf = inf;
	return pnew;
}
int height(NODE *proot){
	if(proot == NULL) return 0;
	else return proot->height;
}
int get_balance(NODE *proot) {
	return height(proot->pleft) - height(proot->pright);
}
NODE *left_rotate(NODE *x) {
	NODE *y = x->pright;
	NODE *z = y->pleft;
	
	y->pleft = x;
	x->pright = z;
	
	x->height = MAX(height(x->pleft), height(x->pright)) + 1;
	y->height = MAX(height(y->pleft), height(y->pright)) + 1;
	
	return y;
}
NODE *right_rotate(NODE *x) {
	NODE *y = x->pleft;
	NODE *z = y->pright;
	
	y->pright = x;
	x->pleft = z;
	
	x->height = MAX(height(x->pleft), height(x->pright)) + 1;
	y->height = MAX(height(y->pleft), height(y->pright)) + 1;
	
	return y;
}
NODE *insert(NODE *proot, TYPE inf) {
	if(proot == NULL) 	
		return make_node(inf);
	else if(strcmp(proot->inf.key, inf.key) > 0) 
		proot->pleft = insert(proot->pleft, inf);
	else 
		proot->pright = insert(proot->pright, inf);
	
	proot->height = MAX(height(proot->pleft), height(proot->pright)) + 1;
	
	int balance = get_balance(proot);
	
	if(balance < -1) {
		if(strcmp(proot->pright->inf.key, inf.key) < 0)
			proot = left_rotate(proot);
		else {
			proot->pright = right_rotate(proot->pright);
			proot = left_rotate(proot);
		}
	}
	else if(balance > 1) {
		if(strcmp(proot->pleft->inf.key, inf.key) > 0)
			proot = right_rotate(proot);
		else {
			proot->pleft = left_rotate(proot->pleft);
			proot = right_rotate(proot);
		}
	}
	return proot;
}
NODE *find_min_node(NODE *pnode) {
	NODE *pcurr = pnode; 
	while(pcurr->pleft!=NULL) {
		pcurr = pcurr->pleft;
	}
	return pcurr;
}
NODE *delete_node(NODE *proot, TYPE inf) {
	if(proot == NULL) return proot;
	else if(strcmp(proot->inf.key, inf.key) > 0) 
		proot->pleft = delete_node(proot->pleft, inf);
	else if(strcmp(proot->inf.key, inf.key) < 0)
		proot->pright = delete_node(proot->pright, inf);
	else {
		// if one or none of them are NULL
		if((proot->pleft == NULL) || (proot->pright == NULL)){
			NODE *ptemp = (proot->pleft != NULL) ? proot->pleft :
																			proot->pright;
			//both are NULL
			if(ptemp == NULL) {
				ptemp = proot;
				proot = NULL;
			}
			//one of them is NULL
			else {
				*proot = *ptemp;
			} //copy content of the non empty child
			
			free(ptemp);
		}
		//2 children case
		else {
			NODE *ptemp = find_min_node(proot->pright);
			strcpy(proot->inf.key, ptemp->inf.key);
			strcpy(proot->inf.val, ptemp->inf.val);
			proot->inf.used = ptemp->inf.used;
			proot->pright = delete_node(proot->pright, ptemp->inf);
		}
	}
	if(proot == NULL) return proot;
	
	proot->height = MAX(height(proot->pleft), height(proot->pright)) + 1;
	
	int balance = get_balance(proot);
	
	if(balance < -1) {
		if(get_balance(proot->pright) <= 0)
			proot = left_rotate(proot);
		else {
			proot->pright = right_rotate(proot->pright);
			proot = left_rotate(proot);
		}
	}
	else if(balance > 1) {
		if(get_balance(proot->pleft) >= 0)
			proot = right_rotate(proot);
		else {
			proot->pleft = left_rotate(proot->pleft);
			proot = right_rotate(proot);
		}
	}
	return proot;
}
void *delete_tree(NODE *proot){
		if(proot!=NULL) {
			delete_tree(proot->pleft);
			delete_tree(proot->pright);
			free(proot);
		}
	    return NULL;
}