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

#include "hadt_common.hpp"

namespace hadt {

	template <class T>
	class forward_list
	{
	protected:

		template <bool IsConst = false>
		class list_iterator : public std::iterator <std::forward_iterator_tag, T>
		{
		public:

			typedef T value_type;
			// T& / const T&
			typedef typename hadt::node_iterator_base<T, IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename hadt::node_iterator_base<T, IsConst>::iterator_pointer pointer;
			typedef std::ptrdiff_t difference_type;
			typedef std::forward_iterator_tag iterator_category;

			list_iterator() : ptr_{ nullptr } {};
			explicit list_iterator(HNode<T> *ptr) : ptr_(ptr) {};

			list_iterator(const list_iterator<IsConst>& it) : ptr_(it.ptr_) {};
			list_iterator(const list_iterator<IsConst>&& it) : ptr_(std::move(it.ptr_)) {};

			list_iterator<IsConst>& operator=(const list_iterator<IsConst>& it) = delete;
			list_iterator<IsConst>& operator=(const list_iterator<IsConst>&& it) = delete;

			bool operator==(const list_iterator<IsConst>& other) { return ptr_ == other.ptr_; }
			bool operator!=(const list_iterator<IsConst>& other) { return ptr_ != other.ptr_; }

			reference operator*() const { return ptr_->data; }
			pointer operator->() const { return &(ptr_->data); } // { return &(**this) }

			auto get() const -> reference { return &(ptr_->data); }
			auto get_node() const -> HNode<T>* { return ptr_; }

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

		private:

			// HNode<T> * / const HNode<T> *
			typename hadt::node_iterator_base<T, IsConst>::iterator_value_type_ptr ptr_;
		};

		HNode<T> *head, *tail;
		HNode<T> *tail_junk;

		size_t size_;

	public:

		typedef std::forward_iterator_tag iterator_category;

		typedef list_iterator<false> iterator;
		typedef list_iterator<true> const_iterator;

		forward_list() :
			head{ nullptr },
			tail{ nullptr },
			tail_junk{ nullptr } {};

		virtual ~forward_list() throw() { clear(); }

		// copy ctor; move ctor; copy assign; move assign
		forward_list(const forward_list& node) = delete;
		forward_list& operator=(const forward_list& node) = delete;
		forward_list(forward_list&& node) = delete;
		forward_list& operator=(forward_list&& node) = delete;

		iterator begin() const { return iterator(head); }
		iterator end() const { return iterator(tail_junk); }

		const_iterator cbegin() const { return const_iterator(head); }
		const_iterator cend() const { return const_iterator(tail_junk); }

		// Insert at front
		auto push_front(const T& data) throw() -> void;
		auto move_front(T&& data) throw() -> void;

		// Insert at back
		auto push_back(const T& data) throw() -> void;
		auto move_back(T&& data) throw() -> void;

		// Pop(remove) from the list
		auto pop_front() throw(std::length_error, std::out_of_range)->T;
		auto pop_back() throw(std::length_error, std::out_of_range)->T;
		auto pop_at(size_t idx) throw(std::length_error, std::out_of_range)->T;

		// Populate the list
		auto fill_with(std::initializer_list<T> init_list) -> void;

		template <class Iter, class Enable = std::enable_if<
			(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
			std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>
		//TODO: CANNOT DEDUCE TEMPLATE ARGUMENT FOR "ENABLE"
		/*template <class Iter, class Enable = std::enable_if<
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>::value &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>*/
		auto fill_with(const Iter& _begin, const Iter& _end) -> void;

		// Append / Prepend the list
		auto append_with(std::initializer_list<T> append_list) -> void;
		auto prepend_with(std::initializer_list<T> prepend_list) -> void;

		template <class Iter, class Enable = std::enable_if<
			(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
			std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>
		auto append_with(const Iter& _begin, const Iter& _end) -> void;

		template <class Iter, class Enable = std::enable_if<
			(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
			std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>
		auto prepend_with(const Iter& _begin, const Iter& _end) -> void;

		// Find by value/by index
		// O(n)
		auto find_first(T value) const -> iterator;
		// O(size() - idx)
		auto find_nth_to_last(size_t idx) const throw(std::out_of_range) -> iterator;

		// Look at item in idx position 
		auto at(size_t idx) throw(std::out_of_range) -> T;
		auto at_front() throw(std::out_of_range) -> T;
		auto at_back() throw(std::out_of_range) -> T;

		// Reverse the list
		virtual auto reverse_inplace() -> void;
		auto reverse(forward_list<T>& ref) -> void;

		// Service functions
		inline auto size() const -> size_t { return size_; };
		inline auto empty() -> bool { return begin() == end(); };

		// Print list
		auto print(std::ostream& ostream = std::cout) const -> std::ostream&;
		virtual auto print_reverse(std::ostream& ostream = std::cout) -> std::ostream&;

		// Clear container
		virtual auto clear() throw() -> void;

	private:

		// Insert HNode at front or back
		virtual auto _push_front(T&& data) throw() -> void;
		virtual auto _push_back(T&& data) throw() -> void;

		// Remove HNode at given position
		virtual auto _remove_at(size_t idx) throw(std::out_of_range) -> T;

		// Return HNode at given position
		auto _node_at(size_t idx) throw(std::out_of_range) -> HNode<T>*;
	};


	template <class T>
	auto forward_list<T>::_push_front(T&& data) throw() -> void
	{
		HNode<T> *node = new HNode<T>(std::move(data));

		if (nullptr == tail_junk)
			tail_junk = new HNode<T>(T{ 55 }, nullptr, nullptr);

		// Head
		if (nullptr != head)
			node->next = head;

		head = node;

		// Tail
		if (nullptr == tail)
		{
			tail = node;
			tail->next = tail_junk;
		}
	}

	template <class T>
	auto forward_list<T>::push_front(const T& data) throw() -> void
	{
		_push_front(std::move(T{ data }));
		size_++;
	}

	template <class T>
	auto forward_list<T>::move_front(T&& data) throw() -> void
	{
		_push_front(std::move(data));
		size_++;
	}

	template <class T>
	auto forward_list<T>::_push_back(T&& data) throw() -> void
	{
		HNode<T> *node = new HNode<T>(std::move(data));

		if (tail_junk == nullptr)
			tail_junk = new HNode<T>(T{}, nullptr, nullptr);

		// Tail
		if (nullptr != tail)
			tail->next = node;

		node->next = tail_junk;
		tail = node;

		// Head
		if (nullptr == head)
			head = node;
	}

	template <class T>
	auto forward_list<T>::push_back(const T& data) throw() -> void
	{
		_push_back(std::move(T{ data }));
		size_++;
	}

	template <class T>
	auto forward_list<T>::move_back(T&& data) throw() -> void
	{
		_push_back(std::move(data));
		size_++;
	}

	template <class T>
	auto forward_list<T>::fill_with(std::initializer_list<T> init_list) -> void
	{
		clear();

		append_with(init_list);
	}

	template <class T>
	auto forward_list<T>::append_with(std::initializer_list<T> append_list) -> void
	{
		for (auto &x : append_list)
			push_back(x);
	}

	template <class T>
	auto forward_list<T>::prepend_with(std::initializer_list<T> prepend_list) -> void
	{
		for (auto &x : prepend_list)
			push_front(x);
	}

	template <class T>
	template <class Iter, class Enable = std::enable_if<
		(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
	>::type>
	auto forward_list<T>::fill_with(const Iter& _begin, const Iter& _end) -> void
	{
		clear();

		append_with(_begin, _end);
	}

	template <class T>
	template <class Iter, class Enable = std::enable_if<
		(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
	>::type>
	auto forward_list<T>::append_with(const Iter& _begin, const Iter& _end) -> void
	{
		auto it = _begin;
		while (it != _end)
			push_back(*it++);
	}

	template <class T>
	template <class Iter, class Enable = std::enable_if<
		(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
	>::type>
	auto forward_list<T>::prepend_with(const Iter& _begin, const Iter& _end) -> void
	{
		auto it = _begin;
		while (it != _end)
			push_front(*it++);
	}

	template <class T>
	auto forward_list<T>::_node_at(size_t idx) throw(std::out_of_range) -> HNode<T>*
	{
		if (idx >= size())
			throw std::out_of_range("_node_at()");

		size_t cnt{};

		auto it = begin();
		while (idx != 0 && it++ != end() && ++cnt != idx);

		return it.get_node();
	}

	template <class T>
	auto forward_list<T>::clear() throw() -> void
	{
		HNode<T> *tmp = head;
		HNode<T> *it = head;

		try
		{
			while (tmp != tail_junk)
			{
				tmp = it->next;
				delete it;
				it = tmp;
			}

			if (nullptr != tail_junk)
				delete tail_junk;

			head = tail = tail_junk = nullptr;

			size_ = 0;
		}
		catch (...)
		{
			terminate();
		}
	}

	template <class T>
	auto forward_list<T>::_remove_at(size_t idx) throw(std::out_of_range) -> T
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
				HNode<T> * tmp = head;
				head = head->next;

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
				HNode<T> * tmp = tail;

				// O(n)
				HNode<T> * prev = _node_at(size() - 2);
				prev->next = tail_junk;
				tail = prev;

				delete tmp;

				size_--;
			}
		}

		// In the middle
		else
		{
			HNode<T> * prev = _node_at(idx - 1);
			HNode<T> * curr = prev->next;
			HNode<T> * next = curr->next;

			val = curr->data;

			prev->next = next;

			delete curr;

			size_--;
		}

		return val;
	}

	template <class T>
	auto forward_list<T>::pop_front() throw(std::length_error, std::out_of_range) -> T
	{
		if (empty())
			throw std::length_error("list is empty");

		return _remove_at(0);
	}

	template <class T>
	auto forward_list<T>::pop_back() throw(std::length_error, std::out_of_range) -> T
	{
		if (empty())
			throw std::length_error("list is empty");

		return _remove_at(size() - 1);
	}

	template <class T>
	auto forward_list<T>::pop_at(size_t idx) throw(std::length_error, std::out_of_range) -> T
	{
		if (empty())
			throw std::length_error("list is empty");

		return _remove_at(idx);
	}

	template <class T>
	auto forward_list<T>::at(size_t idx) throw(std::out_of_range) -> T
	{
		if (idx >= size())
			throw std::out_of_range("at()");

		size_t cnt{};

		auto it = begin();
		while (idx != 0 && it++ != end() && ++cnt != idx);

		return *it;
	}

	template <class T>
	auto forward_list<T>::at_front() throw(std::out_of_range) -> T
	{
		return at(0);
	}

	template <class T>
	auto forward_list<T>::at_back() throw(std::out_of_range) -> T
	{
		return at(size() - 1);
	}

	template <class T>
	auto forward_list<T>::find_first(T value) const -> iterator
	{
		auto it = begin();

		while (*it != value && ++it != end());

		return it;
	}

	template <class T>
	auto forward_list<T>::find_nth_to_last(size_t idx) const throw(std::out_of_range) -> iterator
	{
		if (idx >= size())
			throw std::out_of_range("at()");

		auto it_range_end = ++begin();
		auto it_range_begin = begin();

		while (it_range_end != end())
		{
			it_range_end++;

			if (idx == 0)
				it_range_begin++;
			else
				idx--;
		}

		return it_range_begin;
	}

	template <class T>
	auto forward_list<T>::print(std::ostream& ostream = std::cout) const -> std::ostream&
	{
		if (size() == 0) return ostream;

		auto it = begin();

		ostream << *it++;
		while (it != end())
			ostream << "," << *it++;

		return ostream;
	}

	template <class T>
	auto forward_list<T>::print_reverse(std::ostream& ostream = std::cout) -> std::ostream&
	{
		if (size() == 0) return ostream;

		// Reverse the list
		forward_list temp{};
		reverse(temp);
		temp.print(ostream);

		return ostream;
	}

	template <class T>
	auto forward_list<T>::reverse_inplace() -> void
	{
		if (empty()) return;

		HNode<T> * tmp{};
		HNode<T> * curr = head;
		HNode<T> * succ = curr->next;

		curr->next = tail_junk;
		tail = curr;
		while (succ != tail_junk)
		{
			tmp = succ->next;
			succ->next = curr;
			curr = succ;
			succ = tmp;
		}

		head = curr;
	}

	template <class T>
	auto forward_list<T>::reverse(forward_list<T>& ref) -> void
	{
		ref.clear();

		if (empty()) return;

		auto it = begin();
		while (it != end())
		{
			ref.push_front(*it);
			it++;
		}
	}
}