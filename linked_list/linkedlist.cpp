#include "common.h"
#include "linkedlist.h"
//Documents TODO:
//1. unify the method used in build a linked list, all without dummy node.


// generate():	generate a random linked list, this is the straight foward way
// of generating one for testing of ther functions.
void generate(linkp head, int n)
{	//linkp head = head;	
	if(n > 0){
		head->item = rand() % 1000;
		head->next = NULL;
		for (int i = 1; i < n; i++){
			head = head->next = new node(rand() % 1000);	
			cout << head->item << endl;
		}
	}else 
		return;
	//head->next = NULL; //don't need it, it include in the constructor
}

// generate_v0():TODO:	generate a random link list, by calling the push() function
// notice the subtle difference with the generate()
linkp generate_v0(linkp* headRef, int n)
{
	//for(int i=1; i<=n; i++)
		//push(headRef, rand()%1000);
		cout << "begining of the generat_v0(): " << endl;
		push(headRef, 88);
		cout << "*headRef->item = " << (*headRef)->item << endl;
		print_item(*headRef);
		push(headRef, 77);
		cout << "test: " << endl;
		//cout << "*headRef->next->item = " << (*headRef)->next->item << endl;

		cout << "in the generate_v0(): " << endl;
		print_item(*headRef);
	return *headRef;

}

// generate_v1():	generate a random link list, this version is using a strange
// referenece pointer to the head node, but it is also worked, the idea is from
// stanford article about linked list. This is also for a reference function
// here to compare other function in future 
void generate_v1(linkp* headRef, int n)
{	linkp tail = *headRef;	
	tail->item = rand() % 1000;
	cout << "in generate: " << endl;
	cout << tail->item << endl;
	tail->next = NULL;
	for (int i = 1; i < n; i++){
		tail = tail->next = new node(rand() % 1000);	
		cout << tail->item << endl;
	}
	//tail->next = NULL; //don't need it, it include in the constructor
}


// print_item():	Print the item of a linked list passed in by its header
// pointer
void print_item(linkp head)
{	
	for (;head->next != NULL; head=head->next)
		cout << head->item << endl;
	cout << head->item << endl;
}

// push():	Push one element at the begining of a linked list. Use a reference
// pointer refer to the head of the list
void push(linkp* headRef, int item)
{	linkp new_node = new node(item);
	new_node->next = *headRef;
	*headRef = new_node;

/*
	if((*headRef)->next == NULL)
		*headRef = new_node;
	else {
		new_node->next = *headRef;
		*headRef = new_node;
	}
*/
}


//
///*
// * Insertion sort of linked list
// * 
// */
//linkp insertion_sort(linkp a)
//{	node headb(0, 0);
//	linkp u, x, t, b = &headb;
//	for (t = a->next; t != 0; t = u)
//	{	u = t->next; //save t->next, because it will be overwrite later
//		for (x = b; x->next != 0; x = x->next)
//			if (x->next->item > t->item) break;
//		t->next = x->next; 
//		x->next = t;	
//	}
//	a = b;
//	return b;
//}
//
///*
// * Reverse a linked list
// * Interatively reverse a linked list using the left pointer and the right
// * pointerh
// */
//linkp reverse(linkp x)
//{ linkp right, y = x, left = 0;
//  while(y != 0)
//	{	right = y->next;	//save the next, proceed right pointer 
//		y->next = left;		//reverse element y 
//		left = y;			//proced the left to y
//		y = right;			//proceed the y to right,
//	}
//  return left;
//}
//
///*
// * Circular list example:
// *
// * build a circular linked list, with a link from each person to the person on
// * the left in the circle. The integer i represents the ith person in the
// * circle. After building a one-node circular list for 1, we insert 2 through
// * N after that node, resulting in a circle with 1 through N, leaving x
// * pointing to N. Then, we skip M - 1 nodes, beginning with 1, and set the
// * link of the (M - 1)st to skip the Mth, continuing until only one node is left.
// */   
//void Josephus(int n, int m)
//{	int i;
//	linkp t = new node(1, 0); 
//	t->next = t;
//	linkp x = t;
//	for (i = 2; i <= n; i++)
//		x = (x->next = new node(i, t));
//	//x = x->next;
//	while (x != x->next){
//		for (i = 1; i <m; i++) x = x->next;
//		cout << x->next->item << endl;
//		x->next = x->next->next;
//	}
//	cout << x->item << endl;
//}
//
