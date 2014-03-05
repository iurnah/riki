// This is the mian function to demonstrate linked list related functions, those
// function include generate a random linked list generate(data, ne
#include "linkedlist.h"
#include "common.h"

int main()
{	
	//1. demo of generate and print a list
	node* head = new node(0);
	generate(head, 1);
	cout << "Generated list: " << endl;
	print_item(head);
	
	//2. demo of generate_v1 and print a list
	node* head2 = new node(0);
	generate_v1(&head2, 10);
	cout << "Generated list head 2: " << endl;
	print_item(head2);

	//3. demo of using push() to build a list
	//
	node* head3 = new node(0);
	push(&head3, 8);
	cout << "push 8 to void list: " << endl;
	print_item(head3);
	node* head4 = new node(0);
	generate(head4, 5);
	cout << "before push: " << endl;
	print_item(head4);
	push(&head4, 7);
	push(&head4, 6);
	push(&head4, 5);
	push(&head4, 4);
	push(&head4, 3);
	push(&head4, 2);
	cout << "push 7 at the begining of list with 5 element: " << endl;
	print_item(head4);

	//TODO: why this is happened?
	//4. demo of generate_v0(), which use push to build a linked list
	//node *head5 = new node(0);
	node *head5 = new node(0);
	head5 = generate_v0(&head5, 5);
	cout << "generate_v0(): " << endl;
	print_item(head5);

	//5. demo of append a list
}
