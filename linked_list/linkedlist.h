#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include "common.h"
struct node
{	int item; node* next;
	node(int x)
		{ item = x; next = NULL; }
};

typedef node* linkp;

void generate(linkp head, int n);

linkp generate_v0(linkp* headRef, int n); 

void generate_v1(linkp* headRef, int n);

void print_item(linkp list);

// TODO: write all those linkedlist functions
// KEY 1: Push() at the head and Push() at the tail. Pay special attention to the
// zero element list.
void push(linkp* headRef, int item);

linkp reverse(linkp x);

void Josephus(int n, int m);

//int length()
//int push()
//int popy()
//int append()
//int 
#endif
