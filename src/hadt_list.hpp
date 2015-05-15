#pragma once

// std::bidirectional_iterator_tag, std::forward_iterator_tag, std::iterator
#include <iterator>

// std::out_of_range, std::length_error
#include <stdexcept>

// std::cout, std::endl, std::ostream
#include <iostream>

// std::move, std::swap, std::ptrdiff_t
#include <utility>

// std::is_same, std::enable_if
#include <type_traits>

// std::initializer_list
#include <initializer_list>

// hadt::forward_list
#include "./hadt_forward_list.hpp"

namespace hadt {

	template <class T>
	class list : public forward_list<T>
	{
		/* Internal class : [Const] Iterator */

		// Differs from the list_iterator in forward_list class
		template <bool IsConst = false>
		class list_iterator : public std::iterator <std::bidirectional_iterator_tag, T>
		{
			template <bool is_const>
			struct iterator_base_type {};

			// Specialization for iterator
			template <>
			struct iterator_base_type<false>
			{
				typedef T& iterator_reference;
				typedef T* iterator_pointer;
				typedef HNode* iterator_value_type_ptr;
			};

			// Specialization for const_iterator
			template <>
			struct iterator_base_type<true>
			{
				typedef const T& iterator_reference;
				typedef const T* iterator_pointer;
				typedef const HNode* iterator_value_type_ptr;
			};

		public:

			typedef T value_type;
			// T& / const T&
			typedef typename iterator_base_type<IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename iterator_base_type<IsConst>::iterator_pointer pointer;
			typedef std::ptrdiff_t difference_type;

			typedef std::forward_iterator_tag iterator_category;

			list_iterator() : ptr_{ nullptr } {};
			explicit list_iterator(HNode *ptr) : ptr_(ptr) {};

			list_iterator(const list_iterator<IsConst>& it) : ptr_(it.ptr_) {};
			list_iterator(const list_iterator<IsConst>&& it) : ptr_(std::move(it.ptr_)) {};

			list_iterator<IsConst>& operator=(const list_iterator<IsConst>& it) = delete;
			list_iterator<IsConst>& operator=(const list_iterator<IsConst>&& it) = delete;

			bool operator==(const list_iterator<IsConst>& other) { return ptr_ == other.ptr_; }
			bool operator!=(const list_iterator<IsConst>& other) { return ptr_ != other.ptr_; }

			reference operator*() const { return ptr_->data; }
			pointer operator->() const { return &(ptr_->data); } // { return &(**this) }

			auto get() const -> reference { return &(ptr_->data); }
			auto get_node() const -> HNode* { return ptr_; }

			auto operator++() -> list_iterator<IsConst>&
			{
				ptr_ = ptr_->next;
				return *this;
			}

			auto operator++(int) -> list_iterator<IsConst>
			{
				list_iterator<IsConst> it(*this);
				this->operator++();
				return it;
			}

			auto operator--() -> list_iterator<IsConst>&
			{
				ptr_ = ptr_->prev;
				return *this;
			}

			auto operator--(int) -> list_iterator<IsConst>
			{
				list_iterator<IsConst> it(*this);
				this->operator--();
				return it;
			}

		private:

			// HNode * / const HNode *
			typename iterator_base_type<IsConst>::iterator_value_type_ptr ptr_;
		};

		/* Internal class : Reverse [Const] Iterator */

		template <bool IsConst = false>
		class list_reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
			template <bool is_const>
			struct iterator_base_type {};

			// Specialization for iterator
			template <>
			struct iterator_base_type<false>
			{
				typedef T& iterator_reference;
				typedef T* iterator_pointer;
				typedef HNode* iterator_value_type_ptr;
			};

			// Specialization for const_iterator
			template <>
			struct iterator_base_type<true>
			{
				typedef const T& iterator_reference;
				typedef const T* iterator_pointer;
				typedef const HNode* iterator_value_type_ptr;
			};

		public:

			typedef T value_type;
			// T& / const T&
			typedef typename iterator_base_type<IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename iterator_base_type<IsConst>::iterator_pointer pointer;
			typedef std::ptrdiff_t difference_type;
			typedef std::bidirectional_iterator_tag iterator_category;

			list_reverse_iterator() : ptr_{ nullptr } {};
			explicit list_reverse_iterator(HNode *ptr) : ptr_(ptr) {};

			list_reverse_iterator(const list_reverse_iterator<IsConst>& it) : ptr_(it.ptr_) {};
			list_reverse_iterator(const list_reverse_iterator<IsConst>&& it) : ptr_(std::move(it.ptr_)) {};

			list_reverse_iterator<IsConst>& operator=(const list_reverse_iterator<IsConst>& it) = delete;
			list_reverse_iterator<IsConst>& operator=(const list_reverse_iterator<IsConst>&& it) = delete;

			bool operator==(const list_reverse_iterator<IsConst>& other) { return ptr_ == other.ptr_; }
			bool operator!=(const list_reverse_iterator<IsConst>& other) { return ptr_ != other.ptr_; }

			reference operator*() const { return ptr_->data; }
			pointer operator->() const { return &(ptr_->data); }

			auto get() const -> reference { return &(ptr_->data); }
			auto get_node() const -> HNode* { return ptr_; }

			auto operator++() -> list_reverse_iterator<IsConst>&
			{
				ptr_ = ptr_->prev;
				return *this;
			}

			auto operator++(int) -> list_reverse_iterator<IsConst>
			{
				list_reverse_iterator<IsConst> it(*this);

				this->operator++();

				return it;
			}

			auto operator--() -> list_reverse_iterator<IsConst>&
			{
				ptr_ = ptr_->next;
				return *this;
			}

			auto operator--(int) -> list_reverse_iterator<IsConst>
			{
				list_reverse_iterator<IsConst> it(*this);

				this->operator--();

				return it;
			}

		private:

			// HNode * / const HNode *
			typename iterator_base_type<IsConst>::iterator_value_type_ptr ptr_;
		};

		HNode *rhead, *rtail;
		HNode *rtail_junk;

	public:

		typedef std::forward_iterator_tag iterator_category;

		typedef list_iterator<false> iterator;
		typedef list_iterator<true> const_iterator;
		typedef list_reverse_iterator<false> reverse_iterator;
		typedef list_reverse_iterator<true> const_reverse_iterator;

		list() :
			rhead{ nullptr },
			rtail{ nullptr },
			rtail_junk{ nullptr } {};

		~list() throw() { clear(); }

		// copy ctor; move ctor; copy assign; move assign
		list(const list& node) = delete;
		list<T>& operator=(const list<T>& node) = delete;
		list(list&& node) = delete;
		list<T>& operator=(list<T>&& node) = delete;

		iterator begin() const { return iterator(head); }
		iterator end() const { return iterator(tail_junk); }

		const_iterator cbegin() const { return const_iterator(head); }
		const_iterator cend() const { return const_iterator(tail_junk); }

		reverse_iterator rbegin() const { return reverse_iterator(rhead); }
		reverse_iterator rend() const { return reverse_iterator(rtail_junk); }

		const_reverse_iterator rcbegin() const { return const_reverse_iterator(rhead); }
		const_reverse_iterator rcend() const { return const_reverse_iterator(rtail_junk); }

		// Find by value/by index
		// O(n)
		auto find_first(T value) const -> iterator;
		// Differs from the function in forward_list<T>
		// O(idx)
		auto find_nth_to_last(size_t idx) const throw(std::out_of_range) -> iterator;

		// Reverse the list
		auto reverse_inplace() -> void override;
		auto reverse(list<T>& ref) -> void;

		// Service functions
		inline auto empty() -> bool { return begin() == end(); };

		// Print list
		auto print_reverse(std::ostream& ostream = std::cout)->std::ostream& override;

		// Clear container
		virtual auto clear() throw() -> void;

	private:

		// Insert HNode at front or back
		auto _push_front(T&& data) throw() -> void override;
		auto _push_back(T&& data) throw() -> void override;

		// Remove HNode at given position
		auto _remove_at(size_t idx) throw(std::out_of_range) -> T override;

		// Return HNode at given position
		auto _node_at(size_t idx) throw(std::out_of_range) -> HNode*;
	};


	template <class T>
	auto list<T>::_push_front(T&& data) throw() -> void
	{
		HNode *node = new HNode(std::move(data));

		if (nullptr == tail_junk)
			tail_junk = new HNode(T{}, nullptr, nullptr);

		if (nullptr == rtail_junk)
			rtail_junk = new HNode(T{}, nullptr, nullptr);

		// Head
		if (nullptr != head)
		{
			node->next = head;
			head->prev = node;
		}

		head = node;
		head->prev = rtail_junk;

		// Tail
		if (nullptr == tail)
		{
			tail = node;
			tail->next = tail_junk;
		}

		// RTail
		rtail = head;

		// RHead
		if (nullptr == rhead)
			rhead = tail;
	}

	template <class T>
	auto list<T>::_push_back(T&& data) throw() -> void
	{
		HNode *node = new HNode(std::move(data));

		if (tail_junk == nullptr)
			tail_junk = new HNode(T{}, nullptr, nullptr);

		if (rtail_junk == nullptr)
			rtail_junk = new HNode(T{}, nullptr, nullptr);

		// Tail
		if (nullptr != tail)
		{
			tail->next = node;
			node->prev = tail;
		}

		node->next = tail_junk;
		tail = node;

		// Head
		if (nullptr == head)
		{
			head = node;
			head->prev = rtail_junk;
		}

		// RHead
		rhead = tail;

		// RHead
		if (nullptr == rtail)
			rtail = head;
	}

	template <class T>
	auto list<T>::_node_at(size_t idx) throw(std::out_of_range) -> HNode*
	{
		if (idx >= size())
			throw std::out_of_range("_node_at()");

		size_t cnt{};

		auto it = begin();
		while (idx != 0 && it++ != end() && ++cnt != idx);

		return it.get_node();
	}

	template <class T>
	auto list<T>::clear() throw() -> void
	{
		forward_list<T>::clear();

		if (nullptr != rtail_junk)
			delete rtail_junk;

		rhead = rtail = rtail_junk = nullptr;
	}

	template <class T>
	auto list<T>::_remove_at(size_t idx) throw(std::out_of_range) -> T
	{
		T val{};

		if (idx >= size())
			throw std::out_of_range("_remove_at()");

		// At the front
		if (idx == 0)
		{
			val = head->data;

			if (size() == 1)
			{
				clear();
			}
			else
			{
				HNode * tmp = head;
				head = head->next;

				head->prev = rtail_junk;
				rtail = head;

				delete tmp;

				size_--;
			}
		}

		// At the back
		else if (idx == (size() - 1))
		{
			val = tail->data;

			if (size() == 1)
			{
				clear();
			}
			else
			{
				HNode * tmp = tail;
				HNode * prev = _node_at(size() - 2);

				prev->next = tail_junk;
				tail = prev;
				rhead = tail;

				delete tmp;

				size_--;
			}
		}

		// In the middle
		else
		{
			HNode * prev = _node_at(idx - 1);
			HNode * curr = prev->next;
			HNode * next = curr->next;

			val = curr->data;

			prev->next = next;
			next->prev = prev;

			delete curr;

			size_--;
		}

		return val;
	}

	template <class T>
	auto list<T>::find_first(T value) const -> iterator
	{
		auto it = begin();

		while (*it != value && ++it != end());

		return it;
	}

	template <class T>
	auto list<T>::find_nth_to_last(size_t idx) const throw(std::out_of_range) -> iterator
	{
		if (idx >= size())
			throw std::out_of_range("at()");

		// Effective search using reverse_iterator
		auto it = rbegin();
		while (idx > 0)
		{
			it++;
			idx--;
		}

		return iterator(it.get_node());
	}

	template <class T>
	auto list<T>::print_reverse(std::ostream& ostream = std::cout) -> std::ostream&
	{
		if (size() == 0) return ostream;

		auto it = rbegin();

		ostream << *it++;
		while (it != rend())
		{
			ostream << "," << *it;
			it++;
		}

		return ostream;
	}

	template <class T>
	auto list<T>::reverse_inplace() -> void
	{
		if (empty()) return;

		auto it_beg = begin();
		auto it_end = rbegin();

		while (it_beg.get_node() != it_end.get_node())
			std::swap(*it_beg++, *it_end++);
	}

	template <class T>
	auto list<T>::reverse(list<T>& ref) -> void
	{
		ref.clear();

		if (empty()) return;

		auto it = rbegin();
		while (it != rend())
		{
			ref.push_back(*it);
			it++;
		}
	}
}