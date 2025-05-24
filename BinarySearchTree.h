#pragma once

#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <stack>
#include <functional>
#include <iterator>
#include <utility>

template <typename _Ty>
class BinarySearchTree
{
private:
	struct Node
	{
		_Ty _value;
		Node* _left = nullptr;
		Node* _right = nullptr;
		Node* _parent = nullptr;

		Node(const _Ty& value, Node* parent = nullptr)
			: _value(value)
			, _parent(parent)
		{
		}
	};

	Node* _root = nullptr;
	std::size_t _tree_size = 0;

	Node* find_node(Node* node, const _Ty& value) const;
	Node* remove(Node*& node, const _Ty& value);
	Node* min_node(Node* node) const;
	Node* max_node(Node* node) const;
	std::size_t height(Node* node) const;
	void clear(Node* node);
	Node* copy(Node* node, Node* parent);

public:
	using value_type = _Ty;
	using reference = _Ty&;
	using const_reference = const _Ty&;
	using pointer = _Ty*;
	using const_pointer = const _Ty*;
	using size_type = std::size_t;

	class Iterator
	{
	private:
		Node* _current;
		std::stack<Node*> path;

		void pushLeft(Node* node);

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = _Ty;
		using defference_type = std::ptrdiff_t;
		using pointer = _Ty*;
		using reference = _Ty&;

		Iterator(Node* root = nullptr);
		~Iterator();

		reference operator*() const;
		pointer operator->() const;

		Iterator& operator++();
		Iterator operator++(int);

		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
	};

	class ConstIterator : public Iterator
	{
		using pointer = const _Ty*;
		using reference = const _Ty&;

		ConstIterator(Node* root = nullptr);
		~ConstIterator();

		reference operator*() const;
		pointer operator->() const;
	}; 

	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = std::reverse_iterator<Iterator>;
	using const_reverse_iterator = std::reverse_iterator<ConstIterator>;

	BinarySearchTree();
	BinarySearchTree(std::initializer_list<value_type> init);
	~BinarySearchTree();
	BinarySearchTree(const BinarySearchTree& other);
	BinarySearchTree(BinarySearchTree&& other) noexcept;

	BinarySearchTree& operator=(const BinarySearchTree& other);
	BinarySearchTree& operator=(BinarySearchTree&& other);

	void insert(const_reference value);

	void remove(const_reference value);

	bool find(const_reference value) const;

	size_type size() const;
	bool empty() const;

	value_type min() const;
	value_type max() const;

	size_type height() const;

	void clear();

	iterator begin() const;
	iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin() const;
	reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	template<typename U>
	friend void swap(BinarySearchTree<U>& lhs, BinarySearchTree<U>& rhs);

	template <typename T>
	friend bool operator==(const BinarySearchTree<T>& lhs, const BinarySearchTree<T>& rhs);

	template<typename U>
	friend bool operator!=(const BinarySearchTree<U>& lhs, const BinarySearchTree<U>& rhs);

};

template<typename _Ty>
typename BinarySearchTree<_Ty>::Node* BinarySearchTree<_Ty>::find_node(Node* node, const _Ty& value) const
{
	while (node)
	{
		if (value < node->_value)
			node = node->_left;
		else if (value > node->_value)
			node = node->_right;
		else
			return node;
	}
	return nullptr;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Node* BinarySearchTree<_Ty>::remove(Node*& node, const _Ty& value)
{
	if (!node)
		return;

	if (value < node->_value)
		remove(node->_left, value);
	else if (value > node->_value)
		remove(node->_right, value);
	else
	{
		if (!node->_left)
		{
			Node* temp = node;
			node = node->_right;
			delete temp;
		}
		else if (!node->_right)
		{
			Node* temp = node;
			node = node->_left;
			delete temp;
		}
		else
		{
			Node* succ = min_node(node->_right);
			node->_value = succ->_value;
			remove(node->_right, succ->_value);
		}
		--_tree_size;
	}
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Node* BinarySearchTree<_Ty>::min_node(Node* node) const
{
	while (node && node->_left)
		node = node->_left;
	return node;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Node* BinarySearchTree<_Ty>::max_node(Node* node) const
{
	while (node && node->_right)
		node = node->_right;
	return node;
}

template<typename _Ty>
inline std::size_t BinarySearchTree<_Ty>::height(Node* node) const
{
	if (!node)
		return 0;
	return 1 + std::max(height(node->_left), height(node->_right));
}

template<typename _Ty>
inline void BinarySearchTree<_Ty>::clear(Node* node)
{
	if (!node)
		return;
	clear(node->_left);
	clear(node->_right);
	delete node;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Node* BinarySearchTree<_Ty>::copy(Node* node, Node* parent)
{
	if (!node)
		return nullptr;
	Node* new_node = new Node(node->_value, parent);
	new_node->_left = copy(node->_left, new_node);
	new_node->_right = copy(node->_right, new_node);
	return new_node;
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::BinarySearchTree() = default;

template<typename _Ty>
inline BinarySearchTree<_Ty>::BinarySearchTree(std::initializer_list<value_type> init)
{
	for (const auto& value : init)
		insert(value);
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::~BinarySearchTree()
{
	clear();
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::BinarySearchTree(const BinarySearchTree& other)
{
	_root = copy(other._root, nullptr);
	_tree_size = other._tree_size;
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::BinarySearchTree(BinarySearchTree&& other) noexcept
	: _root(std::exchange(other._root, nullptr))
	, _tree_size(std::exchange(other._tree_size, 0))
{
}

template<typename _Ty>
inline BinarySearchTree<_Ty>& BinarySearchTree<_Ty>::operator=(const BinarySearchTree& other)
{
	if (this != &other)
	{
		clear();
		_root = copy(other._root, nullptr);
		_tree_size = other._tree_size;
	}
	return *this;
}

template<typename _Ty>
inline BinarySearchTree<_Ty>& BinarySearchTree<_Ty>::operator=(BinarySearchTree&& other)
{
	if (this != &other)
	{
		clear();
		_root = std::exchange(other._root, nullptr);
		_tree_size = std::exchange(other._tree_size, 0);
	}
	return *this;
}

template<typename _Ty>
inline void BinarySearchTree<_Ty>::insert(const_reference value)
{
	Node* parent = nullptr;
	Node** current = &_root;

	while (*current)
	{
		parent = *current;
		if (value < (*current)->_value)
			current = &((*current)->_left);
		else if (value > (*current)->_value)
			current = &((*current)->_right);
		else
			return;
	}
	*current = new Node(value, parent);
	++_tree_size;
}

template<typename _Ty>
inline void BinarySearchTree<_Ty>::remove(const_reference value)
{
	remove(_root, value);
}

template<typename _Ty>
inline bool BinarySearchTree<_Ty>::find(const_reference value) const
{
	return find_node(_root, value) != nullptr;
}

template <typename _Ty>
typename BinarySearchTree<_Ty>::size_type BinarySearchTree<_Ty>::size() const
{
	return _tree_size;
}

template<typename _Ty>
bool BinarySearchTree<_Ty>::empty() const
{
	return _tree_size == 0;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::value_type BinarySearchTree<_Ty>::min() const
{
	Node* node = min_node(_root);
	if (!node)
		throw std::runtime_error("Tree is empty");
	return node->_value;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::value_type BinarySearchTree<_Ty>::max() const
{
	Node* node = max_node(_root);
	if (!node)
		throw std::runtime_error("Tree is empty");
	return node->_value;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::size_type BinarySearchTree<_Ty>::height() const
{
	return height(_root);
}

template<typename _Ty>
void BinarySearchTree<_Ty>::clear()
{
	clear(_root);
	_root = nullptr;
	_tree_size = 0;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::iterator BinarySearchTree<_Ty>::begin() const
{
	return iterator(_root);
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::iterator BinarySearchTree<_Ty>::end() const
{
	return iterator();
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::const_iterator BinarySearchTree<_Ty>::cbegin() const 
{
	return const_iterator(_root);
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::const_iterator BinarySearchTree<_Ty>::cend() const 
{
	return const_iterator();
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::reverse_iterator BinarySearchTree<_Ty>::rbegin() const 
{
	return reverse_iterator(end());
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::reverse_iterator BinarySearchTree<_Ty>::rend() const 
{
	return reverse_iterator(begin());
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::const_reverse_iterator BinarySearchTree<_Ty>::crbegin() const 
{
	return const_reverse_iterator(cend());
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::const_reverse_iterator BinarySearchTree<_Ty>::crend() const 
{
	return const_reverse_iterator(cbegin());
}

template<typename _Ty>
inline void BinarySearchTree<_Ty>::Iterator::pushLeft(Node* node)
{
	while (node)
	{
		path.push(node);
		node = node->_left;
	}
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::Iterator::Iterator(Node* root)
	: _current(nullptr)
{
	pushLeft(root);
	++(*this);
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::Iterator::~Iterator()
{
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Iterator::reference BinarySearchTree<_Ty>::Iterator::operator*() const
{
	return _current->_value;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Iterator::pointer BinarySearchTree<_Ty>::Iterator::operator->() const
{
	return &_current->_value;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Iterator& BinarySearchTree<_Ty>::Iterator::operator++()
{
	if (path.empty())
	{
		_current = nullptr;
		return *this;
	}
	_current = path.top();
	path.pop();
	pushLeft(_current->_right);
	return *this;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::Iterator BinarySearchTree<_Ty>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

template<typename _Ty>
inline bool BinarySearchTree<_Ty>::Iterator::operator==(const Iterator& other) const
{
	return _current == other._current;
}

template<typename _Ty>
inline bool BinarySearchTree<_Ty>::Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::ConstIterator::ConstIterator(Node* root)
	: Iterator(root)
{
}

template<typename _Ty>
inline BinarySearchTree<_Ty>::ConstIterator::~ConstIterator()
{
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::ConstIterator::reference BinarySearchTree<_Ty>::ConstIterator::operator*() const
{
	return Iterator::_current->_value;
}

template<typename _Ty>
typename BinarySearchTree<_Ty>::ConstIterator::pointer BinarySearchTree<_Ty>::ConstIterator::operator->() const
{
	return &(Iterator::_current->_value);
}

template<typename U>
void swap(BinarySearchTree<U>& lhs, BinarySearchTree<U>& rhs)
{
	std::swap(lhs._root, rhs._root);
	std::swap(lhs._tree_size, rhs._tree_size);
}

template<typename U>
bool operator==(const BinarySearchTree<U>& lhs, const BinarySearchTree<U>& rhs)
{
	using Node = typename BinarySearchTree<U>::Node;

	std::function<bool(Node*, Node*)> equal = [&](Node* a, Node* b) -> bool
		{
			if (!a && !b) return true;
			if (!a || !b) return false;
			if (a->_value != b->_value) return false;
			return equal(a->_left, b->_left) && equal(a->_right, b->_right);
		};

	return lhs.size() == rhs.size() && equal(lhs._root, rhs._root);
}

template<typename U>
bool operator!=(const BinarySearchTree<U>& lhs, const BinarySearchTree<U>& rhs)
{
	return !(lhs == rhs);
}
