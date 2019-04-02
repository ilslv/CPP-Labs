#pragma once

template<typename  T>
class rb_tree
{
	struct leaf
	{
		leaf* parent, left, right;
		T value;
		bool is_black;

		explicit leaf(T value)
		{
			parent = left = right = nullptr;
			this->value = value;
			is_black = false;
		}
	};

	leaf* root_ = nullptr;

	leaf* get_grandparent(leaf* node)
	{
		if (node != nullptr && node->parent != nullptr)
		{
			return node->parent->parent;
		}
		return nullptr;
	}

	leaf* get_uncle(leaf* node)
	{
		leaf* grandparent = get_grandparent(node);
		if (grandparent == nullptr)
		{
			return nullptr;
		}
		if (node->parent == grandparent->left)
		{
			return grandparent->right;
		}
		return grandparent->left;
	}

	void rotate_left(leaf* node)
	{
		leaf* pivot = node->right;
		pivot->parent = node->parent;
		if (node->parent != nullptr)
		{
			if (node->parent->left == node)
			{
				node->parent->left = pivot;
			}
			else
			{
				node->parent->right = pivot;
			}
		}
		node->right = pivot->left;
		if (pivot->left != nullptr)
		{
			pivot->left.parent = node;
		}
		node->parent = pivot;
		pivot->left = node;
	}

	void rotate_right(leaf* node)
	{
		leaf* pivot = node->left;

		pivot->parent = node->parent;
		if (node->parent != nullptr)
		{
			if (node->parent->left == node)
			{
				node->parent->left = pivot;
			}
			else
			{
				node->parent->right = pivot;
			}
		}
		node->left = pivot->right;
		if (pivot->right != nullptr)
		{
			pivot->right.parent = node;
		}
		node->parent = pivot;
		pivot->right = node;
	}

	leaf* insert_without_balancing(leaf* insertion_node, leaf* tree_node)
	{
		if (tree_node == nullptr)
		{
			tree_node = insertion_node;
			return tree_node;
		}

		if (insertion_node->value > tree_node->value)
		{
			insert_without_balancing(insertion_node, tree_node->right);
		}
		else if (insertion_node->value < tree_node->value)
		{
			insert_without_balancing(insertion_node, tree_node->left);
		}

		return nullptr;
	}

	void insert_case1(leaf* node)
	{
		if (node->parent == nullptr)
		{
			node->is_black = true;
		}
		else
		{
			insert_case2(node);
		}
	}

	void insert_case2(leaf* node)
	{
		if (!node->parent->is_black)
		{
			insert_case3(node);
		}
	}

	void insert_case3(leaf* node)
	{
		leaf* uncle = get_uncle(node);

		if(uncle != nullptr && !uncle->is_black)
		{
			node->parent->is_black = true;
			uncle->is_black = true;
			leaf* grandparent = get_grandparent(node);
			grandparent->is_black = false;
			insert_case1(grandparent);
		}
		else
		{
			insert_case4(node);
		}
	}

	void insert_case4(leaf* node)
	{
		leaf* grandparent = get_grandparent(node);
		if(node == node->parent->right && node->parent == grandparent->left)
		{
			rotate_left(node->parent);
			node = node->left;
		}
		else if (node == node->parent->left && node->parent == grandparent->right)
		{
			rotate_right(node->parent);
			node = node->right;
		}
		insert_case5(node);
	}

	void insert_case5(leaf* node)
	{
		leaf* grandparent = get_grandparent(node);
		node->parent->is_black = true;
		grandparent->is_black = false;
		if (node == node->parent->left && node->parent == grandparent->left)
		{
			rotate_right(grandparent);
		}
		else
		{
			rotate_left(grandparent);
		}
	}
public:
	void insert(T value)
	{
		leaf* node = leaf(value);
		node = insert_without_balancing(node, root_);
		if (node != nullptr)
		{
			insert_case1(node);
		}
	}
};