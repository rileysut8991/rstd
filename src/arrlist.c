#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arrlist.h"

ArrList initArrList(int length, size_t nsize){
	//Allocate for ArrList in a singular data segment that expands dynamically
	//Allocated memory will look like this:
	//
	// |--------------|--------------|--------------|-----
	// | Data Segment | Data Segment | Data Segment |. . .
	// |--------------|--------------|--------------|-----
	// |     Data     |     Data     |     Data     |. . .
	// |--------------|--------------|--------------|-----
	//
	//Each data node is of size nsize and the number of nodes is length
	struct ArrList_t *l;

	//Return NULL and printt error to indicate no memory allocated
	if ((l = malloc(sizeof(struct ArrList_t))) == NULL){
		fprintf(stderr, "Malloc failed in initArrList.\n");
		return NULL;

	}

	//Return NULL and print error to indicate no memory allocated
	if ((l->head = malloc(nsize*length)) == NULL){ 
		fprintf(stderr, "Malloc failed in initArrList.\n");
		return NULL;
	}

	l->nsize = nsize;
	l->clength = 0;
	l->length = length;

	return l;
}

void freeArrList(ArrList l){
	free(l->head);
	free(l);
}

void expandArrList(int size, ArrList l){
	void *nhead = realloc(l->head,size*l->nsize);
	
	//Exit after realloc failure as list requires new element when expanded
	if (nhead == NULL){
		fprintf(stderr, "Realloc failed in expandArrList.\n");
		exit(-1);
	} else {
		l->head = nhead;
		l->length = size;
	}
}

//NULL pointer means to allocate an empty node
void addArrList(void *src,  ArrList l){
	//Check if ArrList needs to be resized
	if (l->clength == l->length)
		expandArrList(l->length*2,l);

	if (src != NULL)
		memcpy((char*)l->head + l->clength*l->nsize, src, l->nsize);

	l->clength++;

}

void addAllArrList(ArrList sl, ArrList dl){
	//Next power of 2 in dl to fit sl
	int length = dl->length;
	while (sl->clength + dl->clength > length)
		length*=2;
	expandArrList(length, dl);

	//Copy all the data in one chunk over to the destination list and increment the clength
	memcpy((char*)dl->head + dl->clength*dl->nsize, sl->head, sl->clength*sl->nsize);
	dl->clength += sl->clength;
}

//Undefined behaviour if src is in l
void delArrList(void *src, ArrList l){
	//Iterate through all indicies and delete those that match
	for (int i = 0; i < l->length; i++){
		if ( (memcmp(src,((char*)l->head + i*l->nsize),l->nsize) == 0) ){
			remArrList(i,l);
			//Iterate i down since now the i'th element will be different
			i--;
		}
	}
}

void delAllArrList(ArrList sl, ArrList dl){
	//Protect function from failing due to deleting all elements in the same list
	if (sl == dl)
		//Simply reset the ArrList by setting clength to 0
		dl->clength = 0;
	else {
		for (int i = 0; i < sl->clength; i++){
			delArrList((char*)sl->head + i*dl->nsize,dl);
		}
	}
}

bool containsArrList(void *src, ArrList l){
	//Iterate through all indicies and return TRUE if equal element found
	for (int i = 0; i < l->clength; i++){
		if (memcmp(src,l->head + i*l->nsize,l->nsize) == 0){
			return TRUE;
		}
	}

	//No element in the list is equal and thus we return FALSE
	return FALSE;
}

int countArrList(void *src, ArrList l){
	int count = 0;

	//Iterate through all elements in arrlist and increment count accordingly if equal
	for (int i = 0; i < l->clength; i++){
		if (memcmp(src,(char*)l->head + i*l->nsize,l->nsize)) count++;
	}

	//All elements tested and incremented accordingly and thus we return the count
	return count;
}

ArrList cloneArrList(ArrList l){
	//Add all optimised for large sections of data
	ArrList lc = initArrList(l->length, l->nsize);
	addAllArrList(l, lc);
	return lc;
}

void *getArrList(int ind, ArrList l){
	//Simply increment the head pointer by the size * ind to find the pointer to the data
	return (char*)l->head + ind*l->nsize;
}

void setArrList(int ind, void *src, ArrList l){
	//Simply copy the new data from src to the corresponding pointer to the data in the ArrList
	memcpy(getArrList(ind, l), src, l->nsize);
}

//Can only squeeze between elements and at 0, not at clength
void squeezeArrList(int ind, void* src, ArrList l){
	//Increment clength so we know whether to expand or not
	l->clength++;

	//Expand if required
	if (l->clength > l->length)
		expandArrList(l->length*2,l);

	//memcpy doesn't support overlapping memory so we need to do this		
	for (int j = l->clength - 2; j >= ind; j--)
		if (src != NULL)
			memcpy((char*)l->head + (j+1)*l->nsize, (char*)l->head + j*l->nsize, l->nsize);
	
	//Empty element at ind now set
	setArrList(ind,src,l);
}

void remArrList(int ind, ArrList l){
	l->clength--;
	//Memory is moved down so this will work regardless of memcpy implementation
	memcpy((char*)l->head + ind*l->nsize, (char*)l->head + (ind+1)*l->nsize, (l->clength - ind)*l->nsize);
}

void *getHeadArrList(ArrList l){
	if (l->clength == 0)
		return NULL;
	return l->head;
}

void *getTailArrList(ArrList l){
	//Jump to the end element pointer using clength and size
	if (l->clength == 0)
		return NULL;
	return (char*)(l->head) + (l->clength-1) * l->nsize;
}

void printDiagsArrList(ArrList l){	
	for (int i = 0; i < 30; i++) printf("-");
	printf("\n");
	
	printf("List Length:%d\n",l->length);
	printf("List CLength:%d\n",l->clength);

	printf("Contains:\n");
	for (int i = 0; i < l->clength; i++){
		for (int j = l->nsize-1; j >= 0; j--){
			printf("%02x",(*(((unsigned char*)l->head) + i*l->nsize + j)));
		}
		printf("\n");
	}

	for (int i = 0; i < 30; i++) printf("-");
	printf("\n");
}
