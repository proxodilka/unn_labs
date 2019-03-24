#include <iostream>
#include <string>
#define DataType string

using namespace std;

struct tree {
	DataType value;
	int count = -1;
	int size = 0;
	tree *left = nullptr;
	tree *right = nullptr;
};

void tree_init(tree *t, DataType data);

int __default_compare(DataType first, DataType second);

void tree_insert(tree *t, DataType data);

void tree_print(tree *t, string prefix = "", char separator = '\n', ostream& out = cout);

void tree_set_compare_function(int(*new_comparator)(DataType a, DataType b));

void tree_erase(tree *t, DataType data, tree *parrent = nullptr);

void tree_kick(tree *t, DataType data);

void tree_clear(tree *t, tree *parrent = nullptr, bool is_root = 1);

tree *tree_find(tree *t, DataType data);