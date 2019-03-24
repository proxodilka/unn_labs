#include "my_queue.h"

using namespace std;

void memory_error() {
	cout << "Houston, we have a problem: MEMORY ERROR\n";
	exit(-100);
}

void queue_init(queue *q, DataType data) {
	node *tmp = new node;
	if (!tmp)
		memory_error();
	tmp->value = data;
	tmp->next = nullptr;
	q->first = tmp;
	q->last = tmp;
	q->size++;
}

void queue_push(queue *q, DataType data) {
	if (q->size == 0) {     
		queue_init(q, data); 
		return;
	}
	node *tmp = new node;
	if (!tmp)
		memory_error();
	tmp->value = data;	 
	tmp->next = nullptr;  
	q->last->next = tmp;  
	q->last = tmp;		  
	q->size++;	
}

void queue_print(queue *q, string prefix, char separator, int show_index, ostream &out) {
	if (q->size == 0) return;							  
	node *current = q->first;
	int current_index = show_index;
	string current_index_string="";
	while (current->next) {		
		if (show_index!=MY_NULL)
			current_index_string = to_string(current_index) + ". ";
		out << prefix << current_index_string << current->value << separator;
		current = current->next;
		current_index++;
	}
	if (show_index != MY_NULL)
		current_index_string = to_string(current_index) + ". ";
	out << prefix << current_index_string << current->value << '\n';
}

DataType queue_pop(queue *q) {
	if (q->size == 0) return NULL_FOR_RETURN; 
	node *tmp = q->first;          
	DataType ans = tmp->value;     
	q->first = q->first->next;     
	q->size--;					   
	delete tmp;					   
	return ans;					 
}

void queue_clear(queue *q) {
	while (q->size) {
		queue_pop(q);
	}
}

void queue_cpy(queue *from, queue *to) {
	if (from->size == 0)
		return;
	node *queue_current_node = from->first;
	while (queue_current_node) {
		queue_push(to, queue_current_node->value);
		queue_current_node = queue_current_node->next;
	}
	//queue_push(to, queue_current_node->value);
}

