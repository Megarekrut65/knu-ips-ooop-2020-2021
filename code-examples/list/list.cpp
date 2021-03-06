/*
 * list.cpp
 *
 *  Created on: Sep 22, 2020
 *      Author: KZ
 */

#include "../doctest.h"

#include <cstddef>
#include <ostream>
#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * \brief A single node in doubly linked list
 *
 * A single node holding a value and pointers to the next and previous nodes.
 * Pointers can be empty (for first and last nodes).
 * \see DoublyLinkedList
 */
template<typename T>
struct ListNode {
	T value;			/**< Value stored in this node */
	ListNode<T>* prev;		/**< Pointer to the previous node in the list, can be empty (nullptr) for the first node */
	ListNode<T>* next;		/**< Pointer to the next node in the list, can be empty (nullptr) for the last node */

	/**
	 * \brief ListNode constructor
	 *
	 * Value must be specified, previous and next pointers are optional.
	 * Can specify only previous without next (useful for append)
	 * \callergraph
	 */
	ListNode(T value, ListNode<T>* prev=nullptr, ListNode<T>* next=nullptr): value{value}, prev{prev}, next{next} {}
};


namespace test_doubly_linked_list {
	void test_create_append_clear();
}

/**
 * \brief Doubly linked list
 *
 * Stores a sequence of values using nodes (ListNode objects) for each value linked with next and previous pointers,
 * See [Doubly Linked List](https://en.wikipedia.org/wiki/Doubly_linked_list "Wikipedia article on Doubly Linked List")
 */
template<typename T>
class DoublyLinkedList {
private:
	ListNode<T>* begin;
	ListNode<T>* end;
	std::size_t _size;
public:

	DoublyLinkedList(): begin{nullptr}, end{nullptr}, _size{0} {}

	~DoublyLinkedList() {
		this->clear();
	}

	/**
	 * \brief Append value to the end of this list
	 *
	 * Creates a new node containing this value, inserts it at the end of list.
	 *
	 * \param value a value to be appended
	 * \post List size is increased by 1
	 * \callgraph
	 */
	void append(T value) {
		auto new_node = new ListNode<T>{value};
		if (begin == nullptr) {
			begin = end = new_node;
		} else {
			new_node->prev = end;
			end->next = new_node;
			end = new_node;
		}
		_size++;
	}

	void clear() {
		ListNode<T>* current = begin;
		while(current) {
			ListNode<T>* to_delete = current;
			current = current->next;
			delete to_delete;
		}
		begin = end = nullptr;
		_size = 0;
	}

	/**
	 * \brief Access items by index
	 *
	 * Complexity is O(n)
	 * \param index zero-based index of item to get
	 * \throw std::out_of_range if index is too large (greater or equals to list size)
	 * \return value of item
	 */
	int operator[](std::size_t index) {
		ListNode<T>* current = begin;
		std::size_t cur_index = 0;
		while(current) {
			if (cur_index == index) {
				return current->value;
			}
			current = current->next;
			cur_index++;
		}
		throw std::out_of_range{"index="+std::to_string(index)+" larger than list size="+std::to_string(_size)};
	}

	std::size_t size() {
		return _size;
	}

	std::size_t size_naive() {
		std::size_t result = 0;
		ListNode<T>* current = begin;
		while(current) {
			result++;
			current = current->next;
		}
		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, const DoublyLinkedList<T>& list) {
		ListNode<T>* current = list.begin; //can also use auto current; or auto* current;
		out<<"[ ";
		while(current) {
			out << current->value << " ";
			current = current->next;
		}
		out<<"]";
		return out;
	}

	friend void test_doubly_linked_list::test_create_append_clear();
};




TEST_CASE("[list] - creating list nodes") {
	ListNode<int> node{123};
	CHECK(node.value == 123);

	ListNode<int>* node2;
	SUBCASE("constructor with 3 arguments") {
		node2 = new ListNode<int>{456, &node, nullptr};
	}
	SUBCASE("constructor with 2 arguments") {
		node2 = new ListNode<int>{456, &node};
	}

	CHECK(node2->value == 456);
	CHECK(node2->prev == &node);
	CHECK(node2->prev->value == 123);
	CHECK(node2->next == nullptr);

	delete node2;
}


namespace test_doubly_linked_list {
	void test_create_append_clear() {
		DoublyLinkedList<int> list;
			CHECK(list.begin == nullptr);
			CHECK(list.end == nullptr);
			CHECK(list.size() == 0);

			SUBCASE("append element") {
				list.append(123);
				CHECK(list.end == list.begin);
				CHECK(list.begin->value == 123);
				CHECK(list.begin->prev == nullptr);
				CHECK(list.begin->next == nullptr);
				CHECK(list.size() == 1);

				list.append(456);
				CHECK(list.end != list.begin);
				CHECK(list.begin->value == 123);
				CHECK(list.begin->prev == nullptr);
				CHECK(list.begin->next == list.end);

				CHECK(list.end->value == 456);
				CHECK(list.end->prev == list.begin);
				CHECK(list.end->next == nullptr);

				CHECK(list.size() == 2);

				{
					std::stringstream s_out;
					s_out<<list;
					CHECK(s_out.str()=="[ 123 456 ]");
				} // to keep s_out in local scope

				CHECK(list[0]==123);
				CHECK(list[1]==456);
				try {
					int result = list[2];
					CHECK(result); // not called
				} catch(const std::out_of_range& ex) {
					CHECK(std::string(ex.what()) == "index=2 larger than list size=2");
				}

				CHECK_THROWS_WITH_AS(list[2],"index=2 larger than list size=2",std::out_of_range);

				SUBCASE("clear list") {
					list.clear();
					CHECK(list.size()==0);
					list.append(789);
					CHECK(list.size()==1);
					CHECK(list[0]==789);
				}

			}
	}
}

TEST_CASE("[list] - creating doubly-linked list") {
	test_doubly_linked_list::test_create_append_clear();
}



