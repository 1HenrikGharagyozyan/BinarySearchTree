#include <iostream>

#include "BinarySearchTree.h"

int main()
{
	BinarySearchTree<int> bst1{ 1, 3, 5, 9, 10, 23, 99 };
	BinarySearchTree<int> bst2{ 3, 9, 22, 99, 3, 0, 11, 32 };
	BinarySearchTree<int> bst3{ 3, 9, 22, 99, 3, 0, 11, 32 };

	std::cout << std::boolalpha;
	std::cout << (bst1 == bst2) << "\n";
	std::cout << (bst2 == bst3);
}