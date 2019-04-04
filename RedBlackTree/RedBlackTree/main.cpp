#include "RedBlackTree.h"
#include <iostream>

int main()
{
	tree::rb_tree<int> tree;
	tree.insert(5);
	tree.insert(8);
	tree.insert(2);
	if (tree.is_in_tree(8))
	{
		std::cout << "Works!";
	}
	if (tree.is_in_tree(-1))
	{
		std::cout << "Fails(";
	}
	system("pause");
	return 0;
}
