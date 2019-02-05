#ifndef AVL_TREE
#define AVL_TREE

#include <stdlib.h>

#define dataAVLNode(N) (((AVLNode) N) + 1) 

typedef struct AVLNode_t{
	struct AVLNode_t *parent;
	int height;
	struct AVLNode_t *left;
	struct AVLNode_t *right;
} *AVLNode;

typedef struct AVLTree_t{
	AVL_Node head;
	size_t nsize;
	int length;
	int (*compare)(void*,void*,size_t); 
		//: {-1, 0, 1}
		//-1 ==> src1 < src2
		//0 ==> src1 == src2
		//1 ==> src1 > src2
		//Maybe I should add an enum to simplify output
} *AVLTree;

//Init
AVLNode initAVLNode(void*,AVLTree);
AVLTree initAVLTree(size_t,int(void*,void*,size_t));

//Free
void freeAVLNode(AVLNode);
void freeAVLTree(AVLTree);

//Misc

void addAVLTree(void*,AVLTree);
//addAllAVLTree

void delAVLList(void*,AVLTree);
//delAllAVLTree
//


void rebalanceAVLTree(AVLTree);

AVLNode searchAVLTree(void,AVLTree);

//Diag
void printDiagsAVLTree(AVLTree);

#endif
