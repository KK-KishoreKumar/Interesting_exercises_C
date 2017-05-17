typedef struct move_st {
	int type1;
	int type2;
	int i1;	// from row
	int j1;	// from column
	int i2;	// to row
	int j2;	// to column
	int state1;
	int state2;
	int player1;
	int player2;
} MOVE;
typedef struct node_st {
	MOVE inf;
	struct node_st *pnext;
	struct node_st *pprev;
} NODE;

NODE *make_node(MOVE);
void list_push(NODE **, MOVE);
void delete_fnodes(NODE **);
NODE *find_start(NODE *);
void print_list(NODE *);