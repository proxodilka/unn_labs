#include "my_tree.h"

bool memory_error(){
	cout << "Houston we have a problem: MEMORY ERROR\n";
	exit(-100);
}

int __default_compare(DataType first, DataType second) {
	if (first > second)
		return 1;
	else if (first < second)
		return -1;
	else if (first == second)
		return 0;
}

int(*comparator)(DataType a, DataType b) = __default_compare;

void tree_init(tree *t, DataType data) {
	t->value = data;
	t->count = 1;
	t->size = 1;
	t->left = nullptr;
	t->right = nullptr;
}

void tree_destruct(tree *t) {
	t->count = -1;
	t->size = 0;
	t->left = nullptr;
	t->right = nullptr;
}

void tree_insert(tree *t, DataType data) {
	if (t->count == -1) {
		tree_init(t, data);
		return;
	}
	tree *current = t;
	int compare_result = comparator(data, current->value);
	if (compare_result == 1) {
		if (current->right != nullptr)
			tree_insert(current->right, data);
		else {
			tree *new_node = new tree;
			if (!new_node)
				memory_error();
			tree_init(new_node, data);
			current->right = new_node;
		}
	}
	else if (compare_result == -1) {
		if (current->left != nullptr)
			tree_insert(current->left, data);
		else {
			tree *new_node = new tree;
			if (!new_node)
				memory_error();
			tree_init(new_node, data);
			current->left = new_node;
		}
	}
	else if (compare_result == 0) {
		current->count++;
	}
	t->size++;
}

tree *find_null_left(tree *t, tree *parrent=nullptr) {
	if (t->left == nullptr)
		return parrent;
	else
		return find_null_left(t->left, t);
}

void tree_erase(tree *t, DataType data, tree *parrent) { 
	//if (t == nullptr) cout << "ROOT IS NULLPTR\n";
	//if (parrent == nullptr) cout << "PARRENT IS NULLPTR\n";
	if (t == nullptr) return;
	int compare_result = comparator(data, t->value);
	if (compare_result == 1)
		tree_erase(t->right, data, t);
	else if (compare_result == -1)
		tree_erase(t->left, data, t);
	else if (compare_result == 0) {
		tree *current = t;
		t->size--;
		if (current->left == nullptr && current->right == nullptr){
			if (parrent != nullptr) {
				if (parrent->right == current)
					parrent->right = nullptr;
				else if (parrent->left == current)
					parrent->left = nullptr;
				delete current;
				//cout << "delete_init\n";
			}
			else
				tree_destruct(current);
			return;
		}
		else if (current->left == nullptr || current->right == nullptr) {
			tree *not_null_child;
			if (current->left != nullptr)
				not_null_child = current->left;
			else
				not_null_child = current->right;
			if (parrent != nullptr) {
				if (parrent->right == current)
					parrent->right = not_null_child;
				else if (parrent->left == current)
					parrent->left = not_null_child;
				delete current;
				//cout << "delete_init\n";
			}
			if (parrent == nullptr) {
				*current = *not_null_child;
				delete not_null_child;
			}
			return;
		}
		else {
			tree *left_child = current->left;
			tree *right_child = current->right;
			tree *replacment_node_parrent = find_null_left(current->right, current);
			tree *replacment_node;
			if (replacment_node_parrent == current) {
				replacment_node = replacment_node_parrent->right;
				if (parrent != nullptr) {
					if (parrent->left == current)
						parrent->left = replacment_node;
					else if (parrent->right == current)
						parrent->right = replacment_node;
					delete current;
					//cout << "delete_init\n";
				}
				else {
					replacment_node->left = current->left;
					*current = *replacment_node;
					delete replacment_node;
				}
				return;
			}
			else {
				replacment_node = replacment_node_parrent->left;
				replacment_node_parrent->left = replacment_node->right;
				if (parrent!=nullptr)
					if (parrent->left == current)
						parrent->left = replacment_node;
					else if (parrent->right == current)
						parrent->right = replacment_node;
			}
			replacment_node->right = current->right;
			replacment_node->left = current->left;
			if (parrent == nullptr) {
				*current = *replacment_node;
				delete replacment_node;
			}
			else {
				delete current;
				//cout << "delete_init\n";
			}
			return;
		}
	}
}

tree *tree_find(tree *t, DataType data) {
	if (t == nullptr) return t;
	int compare_result = comparator(data, t->value);
	if (compare_result == 1)
		return tree_find(t->right, data);
	else if (compare_result == -1)
		return tree_find(t->left, data);
	else
		return t;
}

void tree_kick(tree *t, DataType data) { 
	tree *tmp = tree_find(t, data);
	if (tmp == nullptr)
		return;
	tmp->count--;
	if (tmp->count == 0)
		tree_erase(t, data);
}

void tree_print(tree *t, string prefix,char separator, ostream& out) {
	if (t == nullptr || t->count==-1) return;
	tree_print(t->left, prefix, separator, out);
	out << prefix << t->value << " : " << t->count << separator;
	tree_print(t->right, prefix, separator, out);
}

void tree_set_compare_function(int(*new_comparator)(DataType a, DataType b)=__default_compare) {
	comparator = new_comparator;
}

void tree_clear(tree *t, tree *parrent, bool is_root) { 
	if (t == nullptr) return;
	if (t->left == nullptr && t->right == nullptr) {
		if (parrent!=nullptr)
			if (parrent->left == t)
				parrent->left = nullptr;
			else
				parrent->right = nullptr;
		delete t;
		return;
	}
	tree_clear(t->left, t, 0);
	tree *next = t->right;
	if (!is_root)
		delete t;
	else
		tree_destruct(t);
	tree_clear(next, nullptr, 0);
}
