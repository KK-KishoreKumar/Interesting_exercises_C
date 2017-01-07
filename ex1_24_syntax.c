/*This program checks for rudimentary syntax errors like unbalanced parentheses, brackets, braces, quotes,
	both single and double, escape sequences, and comments  in a file given as argument in command line (last part not needed 
	but i did it to create bigger challange for myself so i can learn more from this exercise) (Excercise 1-24 in K&R Programming in C 2nd edition)*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_ENUM 5
#define MAX_NAME 21
#define IN 1
#define IN_SEC 2
#define OUT 0

enum {PARENTHESES, BRACKETS, BRACES, SINGLE_QUOTES, DOUBLE_QUOTES};

typedef struct node_st{
		int row, column;
		char c;
		struct node_st *pnext;
} NODE;

typedef struct table_st{
	NODE **parray;
} TABLE;

void init_table(TABLE *ptable){
	int i;
	ptable->parray = malloc(MAX_ENUM*sizeof(NODE *));
	if(ptable->parray==NULL){
		perror("ERROR");
		exit(12);
	}
	for(i = 0; i < MAX_ENUM; i++) ptable->parray[i] = NULL; 
}
NODE *make_node(char c, int row, int column){
	NODE *pnew = malloc(sizeof(NODE));
	if(pnew==NULL){
		perror("ERROR");
		exit(13);
	}
	pnew->pnext = NULL;
	pnew->row = row;
	pnew->column = column;
	pnew->c = c;
	return pnew;
}
void free_first(TABLE *ptable, int i){
	NODE *ptemp = ptable->parray[i];
	ptable->parray[i] = ptemp->pnext;
	
	ptemp->pnext = NULL;
	free(ptemp);
}
void add_end(TABLE *ptable, NODE *pnew, int i){
	NODE *pcurr = ptable->parray[i];
	while(pcurr->pnext!=NULL)	pcurr = pcurr->pnext;
	pcurr->pnext = pnew;
}
void delete_elem(TABLE *ptable, NODE *pnew){
	if(pnew!=NULL){
		switch(pnew->c){
		case ')':	(ptable->parray[PARENTHESES] == NULL)	?	printf("%d:%d: error: expected '(' before ')' token.\n", pnew->row, pnew->column)
																							:	free_first(ptable,  PARENTHESES);
						break;
		case ']':	(ptable->parray[BRACKETS] == NULL) ?	printf("%d:%d: error: expected '[' before ']' token.\n", pnew->row, pnew->column)
																					 :	free_first(ptable, BRACKETS);
						break;
		case '}' :	(ptable->parray[BRACES] == NULL)	? printf("%d:%d: error: expected '{' before '}' token.\n", pnew->row, pnew->column) 
																					: free_first(ptable, BRACES);
						break;
		}
	}
}
void add(TABLE *ptable, NODE *pnew){
	if(pnew!=NULL){
		switch(pnew->c){
			case  '(' :	(ptable->parray[PARENTHESES] == NULL) ?	(ptable->parray[PARENTHESES] = pnew) 
																								:	add_end(ptable, pnew, PARENTHESES);
							break;
			case  '[' :	(ptable->parray[BRACKETS] == NULL) ? (ptable->parray[BRACKETS] = pnew)
																						 : 	add_end(ptable, pnew, BRACKETS);
							break;
			case  '{' :	(ptable->parray[BRACES] == NULL) ? (ptable->parray[BRACES] = pnew) 
																					:	add_end(ptable, pnew, BRACES);
							break;
			case  '\'' :	(ptable->parray[SINGLE_QUOTES] == NULL) ? (ptable->parray[SINGLE_QUOTES] = pnew) 
																									:	free_first(ptable, SINGLE_QUOTES);
							break;
			case  '\"' :	(ptable->parray[DOUBLE_QUOTES] == NULL) ? (ptable->parray[DOUBLE_QUOTES] = pnew)
																										:	free_first(ptable, DOUBLE_QUOTES);
							break;
		}
	}
}
int is_recognized(char c){
	if(c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '\"' || c == '\'') return 1;
	else return 0;
}
int is_escape_sequence(char c){
	if(c == 't' ||	c == '\'' || c == '\"' ||  c == '?' || c == 'a' || c == 'b' || c == 'f' || c == 'r' || c == 'v') return 1;
	else return 0;
}
/*decides wheter the character is closing or opening character. If it's opening then it will be added to the table, if it's closing it will try 
	to find it's corresponding opening character (see line #140 for more details on idea)*/
void add_remove(TABLE *ptable, NODE *pnew){
	if(pnew->c == '(' || pnew->c == '[' || pnew->c == '{' || pnew->c == '\'' || pnew->c == '\"') add(ptable, pnew);
	else if(pnew->c == ')' || pnew->c == ']' || pnew->c == '}') delete_elem(ptable, pnew);
	else free(pnew);
}
FILE *fopen_s(char *name, char *mode){
	FILE *pin = fopen(name, mode);
	if(pin == NULL){
		perror("ERROR");
		exit(1);
	}
	return pin;
}
/*Checks if there is still anything left in the table. If so then it means that there was no corresponding closing brackets for that element
	and as a result, error will be printed*/
void check_and_free(TABLE *ptable){
	int i = 0;
	while(i < MAX_ENUM){
		while(ptable->parray[i]!=NULL){
			printf("%d:%d: error: expected '%c' token to be closed with its matching pair.\n", ptable->parray[i]->row, ptable->parray[i]->column, 
			ptable->parray[i]->c);																																
			
			free_first(ptable, i);
		}
		i++;
	}
}
/*Expects 2nd argument ([1]) to be the name of the program you want to check errors from*/
int main(int nb_arg, char **arg){
	if(nb_arg!=2){
		printf("USAGE: %s IN_FILENAME\n", arg[0]);
		exit(666);
	}
	TABLE table;
	init_table(&table);
	
	char c, comment[3];
	comment[2] = '\0';
	
	FILE *pin = fopen_s(arg[1], "r");	
	int row = 1, column = 1, state = OUT, rowcpy, columncpy;
	
	//	Idea: if /* is found then the code up to */ will be ignored, and if */ isn't found then say error. If // then ignore until newline.
	/*If not inside the comment then state will have value OUT and the elements recognized will be put in linked-list table.
		If the corresponding closing character is found then the element in table will be deleted.  If closing character is found 
		but his corresponding opening element isn't in the table then error  will be printed out.*/
	NODE *ptemp;
	while((c = fgetc(pin)) != EOF){
		if(c == '/'){
			comment[0] = c;
			c = fgetc(pin);
			if(c == '*'){
				comment[1] = c;
				rowcpy = row;
				columncpy = column;
				state = IN;
				continue;
			}
			else if(c == '/'){
				comment[1] = c;
				rowcpy = row;
				columncpy = column;
				state = IN_SEC;
			}
		}
		if(c == '\\'){
			c = fgetc(pin);
			if(is_escape_sequence) continue;
		}
		if(c == '\n'){
				if(state == IN_SEC) state = OUT;
				column = 1;
				row++;
				continue;
			}
		if(state == OUT){
			if(is_recognized(c)){
				ptemp = make_node(c, row, column);
				add_remove(&table, ptemp);
			}
			if(c == '*'){
				c = fgetc(pin);
				if(c == '/') printf("%d:%d: error: expected '/*' before '*/' token.\n", row, column);
			}
			column++;
		}
		else if(state == IN){
			if(c == '*'){
				c = fgetc(pin);
				if(c == '/'){
					state = OUT;
					continue;
				}
			}
		}
		if (c == EOF) break;
	}
	
	if(state != OUT)
		printf("%d:%d: error: expected '%s' token to be closed with its matching pair.\n", rowcpy, columncpy, comment);
	
	
	fclose(pin);
	
	check_and_free(&table);
	
	return EXIT_SUCCESS;
}