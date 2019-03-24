#include <iostream>
#include <string>

#define DataType string
#define NULL_FOR_RETURN ""
#define MY_NULL -10e9
using namespace std;

struct node {
	DataType value;
	node* next;
};

struct queue {
	node* first;
	node* last;
	unsigned int size=0;
};

void queue_init(queue *q, DataType data);

void queue_push(queue *q, DataType data);

void queue_print(queue *q, string prefix="", char separator=' ', int show_index=MY_NULL, ostream &out=cout);

DataType queue_pop(queue *q);

void queue_clear(queue *q);

void queue_cpy(queue *from, queue *to);


