#pragma once
#include <stdexcept>
#include <stack>

namespace tree
{
	template<typename T>
	struct leaf
	{
		leaf* parent;
		leaf* left;
		leaf* right;
		T value;
		bool is_black;

		explicit leaf(T value)
		{
			parent = left = right = nullptr;
			this->value = value;
			is_black = false;
		}
	};

	template<typename T>
	class iterator
	{
	protected:
		std::stack<leaf<T>*> stack_;
	public:
		virtual ~iterator() = default;
		virtual bool operator!=(iterator&) = 0;
		virtual T operator*() = 0;
		virtual void operator++() = 0;
	};

	template<typename T>
	class infix_iterator final : public iterator<T>
	{
		void push_all(leaf<T>* node)
		{
			for (; node != nullptr; this->stack_.push(node), node = node->left);
		}

		explicit infix_iterator(leaf<T>* root)
		{
			push_all(root);
		}
	public:
		void operator++() override
		{
			leaf<T>* tmp_node = this->stack_.top();
			this->stack_.pop();
			push_all(tmp_node->right);
		}

		T operator*() override
		{
			return this->stack_.top()->value;
		}

		bool operator!=(iterator<T>& iterator_to_compare) override
		{
			return this->stack_.top()->value != iterator_to_compare.leafs_stack.top()->value;
		}
	};

	template<typename T>
	class postfix_iterator final : public iterator<T>
	{
		void push_all(leaf<T>* node)
		{
			for (; node != nullptr; this->stack_.push(node), node = node->right);
		}

		explicit postfix_iterator(leaf<T>* root)
		{
			push_all(root);
		}
	public:
		void operator++() override
		{
			leaf<T>* tmp_node = this->stack_.top();
			this->stack_.pop();
			push_all(tmp_node->left);
		}

		T operator*() override
		{
			return this->stack_.top()->value;
		}

		bool operator!=(iterator<T>& iterator_to_compare) override
		{
			return this->stack_.top()->value != iterator_to_compare.leafs_stack.top()->value;
		}
	};

	template<typename T>
	class rb_tree
	{
		leaf<T>* root_;

		static void replace_node(leaf<T>* node, leaf<T>* child)
		{
			child->parent = node->parent;
			if (node == node->parent->left)
			{
				node->parent->left = child;
			}
			else
			{
				node->parent->right = child;
			}
		}

		static leaf<T>* get_sibling(leaf<T>* &node)
		{
			if (node == node->parent->left)
			{
				return node;
			}
			return node->parent->left;
		}

		static leaf<T>* get_grandparent(leaf<T>* &node)
		{
			if (node != nullptr && node->parent != nullptr)
			{
				return node->parent->parent;
			}
			return nullptr;
		}

		leaf<T>* get_uncle(leaf<T>* &node)
		{
			leaf<T>* grandparent = get_grandparent(node);
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

		void rotate_left(leaf<T>* &node)
		{
			leaf<T>* pivot = node->right;
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
				pivot->left->parent = node;
			}
			node->parent = pivot;
			pivot->left = node;
		}

		void rotate_right(leaf<T>* &node)
		{
			leaf<T>* pivot = node->left;

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
				pivot->right->parent = node;
			}
			node->parent = pivot;
			pivot->right = node;
		}

		leaf<T>* insert_without_balancing(leaf<T>* insertion_node, leaf<T>* &tree_node, leaf<T>* parent, const bool is_left = true)
		{
			if (tree_node == nullptr)
			{
				tree_node = insertion_node;
				tree_node->parent = parent;
				if (parent != nullptr)
				{
					if (is_left)
					{
						parent->left = tree_node;
					}
					else
					{
						parent->right = tree_node;
					}
				}
				return tree_node;
			}

			if (insertion_node->value > tree_node->value)
			{
				insert_without_balancing(insertion_node, tree_node->right, tree_node, false);
			}
			else if (insertion_node->value < tree_node->value)
			{
				insert_without_balancing(insertion_node, tree_node->left, tree_node, true);
			}
			else
			{
				return nullptr;
			}
		}

		void insert_case1(leaf<T>* &node)
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

		void insert_case2(leaf<T>* &node)
		{
			if (!node->parent->is_black)
			{
				insert_case3(node);
			}
		}

		void insert_case3(leaf<T>* &node)
		{
			leaf<T>* uncle = get_uncle(node);

			if (uncle != nullptr && !uncle->is_black)
			{
				node->parent->is_black = true;
				uncle->is_black = true;
				leaf<T>* grandparent = get_grandparent(node);
				grandparent->is_black = false;
				insert_case1(grandparent);
			}
			else
			{
				insert_case4(node);
			}
		}

		void insert_case4(leaf<T>* &node)
		{
			leaf<T>* grandparent = get_grandparent(node);
			if (node == node->parent->right && node->parent == grandparent->left)
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

		void insert_case5(leaf<T>* &node)
		{
			leaf<T>* grandparent = get_grandparent(node);
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

		void delete_case1(leaf<T>* node)
		{
			if (node->parent != nullptr)
			{
				delete_case2(node);
			}
		}

		void delete_case2(leaf<T>* node)
		{
			leaf<T>* sibling = get_sibling(node);

			if (!sibling->is_black)
			{
				node->parent->is_black = false;
				sibling->is_black = true;
				if(node == node->parent->left)
				{
					rotate_left(node->parent);
				}
				else
				{
					rotate_right(node->parent);
				}
			}
			delete_case3(node);
		}

		void delete_case3(leaf<T>* node)
		{
			leaf<T>* sibling = get_sibling(node);

			if(node->parent->is_black && sibling->is_black && sibling->left->is_black && sibling->right->is_black)
			{
				sibling->is_black = false;
				delete_case1(node->parent);
			}
			else
			{
				delete_case4(node);
			}
		}

		void delete_case4(leaf<T>* node)
		{
			leaf<T>* sibling = get_sibling(node);

			if(!node->parent->is_black && sibling->is_black && sibling->left->is_black && sibling->right->is_black)
			{
				sibling->is_black = false;
				node->parent->is_black = true;
			}
			else
			{
				delete_case5(node);
			}
		}

		void delete_case5(leaf<T>* node)
		{
			leaf<T>* sibling = get_sibling(node);

			if(sibling->is_black)
			{
				if(node == node->parent->left && sibling->right->is_black && !sibling->left->is_black)
				{
					sibling->is_black = false;
					sibling->left->is_black = true;
					rotate_right((sibling));
				} else if(node == node->parent->right && sibling->left->is_black && !sibling->right->is_black)
				{
					sibling->is_black = false;
					sibling->right->is_black = true;
					rotate_left(sibling);
				}
			}
			delete_case6(node);
		}

		void delete_case6(leaf<T>* node)
		{
			leaf<T>* sibling = get_sibling(node);
			sibling->is_black = node->parent->is_black;
			node->parent->is_black = true;
			if (node == node->parent->left)
			{
				sibling->right->is_black = true;
				rotate_left(node->parent);
			}
			else
			{
				sibling->left->is_black = true;
				rotate_right(node->parent);
			}

		}

		leaf<T>* search(leaf<T>* &node, T value)
		{
			if (node == nullptr)
			{
				return nullptr;
			}
			if (node->value < value)
			{
				return search(node->right, value);
			}
			if (node->value > value)
			{
				return search(node->left, value);
			}
			return node;
		}
	public:
		rb_tree()
		{
			root_ = nullptr;
		}

		iterator<T>& begin()
		{
			return infix_iterator<T>(root_);
		}

		iterator<T>& end()
		{
			leaf<T>* tmp_leaf = root_;
			while(tmp_leaf->right != nullptr)
			{
				tmp_leaf = tmp_leaf->right;
			}
			return infix_iterator<T>(tmp_leaf);
		}

		iterator<T>& postfix_begin()
		{
			return postfix_iterator<T>(root_);
		}

		iterator<T>& postfix_end()
		{
			leaf<T>* tmp_leaf = root_;
			while (tmp_leaf->left != nullptr)
			{
				tmp_leaf = tmp_leaf->left;
			}
			return postfix_iterator<T>(tmp_leaf);
		}

		void insert(T value)
		{
			auto* node = new leaf<T>(value);
			node = insert_without_balancing(node, root_, nullptr);
			if (node != nullptr)
			{
				insert_case1(node);
			}
			else
			{
				throw std::invalid_argument("Value is already present in the tree!");
			}
		}

		void delete_leaf(T value)
		{
			leaf<T>* node = search(root_, value);
			if (node != nullptr)
			{
				leaf<T>* child = node->right == nullptr ? node->left : node->right;

				replace_node(node, child);
				if (node->is_black)
				{
					if(!child->is_black)
					{
						child->is_black = true;
					}
					else
					{
						delete_case1(child);
					}
				}
				delete node;
			}
		}

		bool is_in_tree(T value)
		{
			return (search(root_, value) != nullptr);
		}
	};
}