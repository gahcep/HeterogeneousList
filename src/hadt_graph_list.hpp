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


namespace hadt {

	/* External class : Vertex Data */

	template <class T, class W>
	class EdgeNode;

	template <class T, class W>
	class VertexNode
	{
	public:
		T data;
		bool is_visited;
		VertexNode *next;
		EdgeNode<T, W> *edge_head;

		explicit VertexNode(T d)
			: data{ d }, is_visited{ false }, next{ nullptr }, edge_head{ nullptr } {};
		VertexNode(T d, bool vis, VertexNode* vn, EdgeNode<T, W>* en)
			: data{ d }, is_visited{ vis }, next{ vn }, edge_head{ en } {};
	};

	/* External class : Edge Data */

	template <class T, class W>
	class EdgeNode
	{
	public:
		W weight;
		VertexNode<T, W>* connects_to;
		EdgeNode* next;

		explicit EdgeNode(W w)
			: weight{ w }, connects_to{ nullptr }, next{ nullptr } {};
		EdgeNode(W w, VertexNode<T, W>* vn, EdgeNode* en)
			: weight{ w }, connects_to{ vn }, next{ en } {};
	};

	template <class T, class W>
	class graph_list
	{

	protected:

		/* Internal class : [Const] Vertex Iterator */

		template <bool IsConst = false>
		class list_vertex_iterator : public std::iterator <std::forward_iterator_tag, T>
		{
			template <bool is_const>
			struct iterator_base_type {};

			// Specialization for iterator
			template <>
			struct iterator_base_type<false>
			{
				typedef T& iterator_reference;
				typedef T* iterator_pointer;
				typedef VertexNode<T, W>* iterator_value_type_ptr;
			};

			// Specialization for const_iterator
			template <>
			struct iterator_base_type<true>
			{
				typedef const T& iterator_reference;
				typedef const T* iterator_pointer;
				typedef const VertexNode<T, W>* iterator_value_type_ptr;
			};

		public:

			typedef T value_type;
			// T& / const T&
			typedef typename iterator_base_type<IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename iterator_base_type<IsConst>::iterator_pointer pointer;
			typedef std::ptrdiff_t difference_type;

			typedef std::forward_iterator_tag iterator_category;

			list_vertex_iterator() : ptr_{ nullptr } {};
			explicit list_vertex_iterator(VertexNode<T, W> *ptr) : ptr_(ptr) {};

			list_vertex_iterator(const list_vertex_iterator<IsConst>& it) : ptr_(it.ptr_) {};
			list_vertex_iterator(const list_vertex_iterator<IsConst>&& it) : ptr_(std::move(it.ptr_)) {};

			list_vertex_iterator<IsConst>& operator=(const list_vertex_iterator<IsConst>& it) = delete;
			list_vertex_iterator<IsConst>& operator=(const list_vertex_iterator<IsConst>&& it) = delete;

			bool operator==(const list_vertex_iterator<IsConst>& other) { return ptr_ == other.ptr_; }
			bool operator!=(const list_vertex_iterator<IsConst>& other) { return ptr_ != other.ptr_; }

			reference operator*() const { return ptr_->data; }
			pointer operator->() const { return &(ptr_->data); } // { return &(**this) }

			auto get() const -> reference { return &(ptr_->data); }
			auto get_node() const -> VertexNode<T, W>* { return ptr_; }

			auto operator++() -> list_vertex_iterator<IsConst>&
			{
				ptr_ = ptr_->next;
				return *this;
			}

			auto operator++(int) -> list_vertex_iterator<IsConst>
			{
				list_vertex_iterator<IsConst> it(*this);
				this->operator++();
				return it;
			}

		private:

			// HNode * / const HNode *
			typename iterator_base_type<IsConst>::iterator_value_type_ptr ptr_;
		};

		/* Internal class : [Const] Edge Iterator */

		template <bool IsConst = false>
		class list_edge_iterator : public std::iterator <std::forward_iterator_tag, W>
		{
			template <bool is_const>
			struct iterator_base_type {};

			// Specialization for iterator
			template <>
			struct iterator_base_type<false>
			{
				typedef W& iterator_reference;
				typedef W* iterator_pointer;
				typedef EdgeNode<T, W>* iterator_value_type_ptr;
			};

			// Specialization for const_iterator
			template <>
			struct iterator_base_type<true>
			{
				typedef const W& iterator_reference;
				typedef const W* iterator_pointer;
				typedef const EdgeNode<T, W>* iterator_value_type_ptr;
			};

		public:

			typedef T value_type;
			// T& / const T&
			typedef typename iterator_base_type<IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename iterator_base_type<IsConst>::iterator_pointer pointer;
			typedef std::ptrdiff_t difference_type;

			typedef std::forward_iterator_tag iterator_category;

			list_edge_iterator() : ptr_{ nullptr } {};
			explicit list_edge_iterator(EdgeNode<T, W>* ptr) : ptr_(ptr) {};

			list_edge_iterator(const list_edge_iterator<IsConst>& it) : ptr_(it.ptr_) {};
			list_edge_iterator(const list_edge_iterator<IsConst>&& it) : ptr_(std::move(it.ptr_)) {};

			list_edge_iterator<IsConst>& operator=(const list_edge_iterator<IsConst>& it) = delete;
			list_edge_iterator<IsConst>& operator=(const list_edge_iterator<IsConst>&& it) = delete;

			bool operator==(const list_edge_iterator<IsConst>& other) { return ptr_ == other.ptr_; }
			bool operator!=(const list_edge_iterator<IsConst>& other) { return ptr_ != other.ptr_; }

			reference operator*() const { return ptr_->weight; }
			pointer operator->() const { return &(ptr_->weight); } // { return &(**this) }

			auto get() const -> reference { return &(ptr_->weight); }
			auto get_node() const -> EdgeNode<T, W>* { return ptr_; }

			auto operator++() -> list_edge_iterator<IsConst>&
			{
				ptr_ = ptr_->next;
				return *this;
			}

			auto operator++(int) -> list_edge_iterator<IsConst>
			{
				list_edge_iterator<IsConst> it(*this);
				this->operator++();
				return it;
			}

		private:

			// HNode * / const HNode *
			typename iterator_base_type<IsConst>::iterator_value_type_ptr ptr_;
		};

		VertexNode<T, W> *head, *tail;
		VertexNode<T, W> *tail_junk;
		EdgeNode<T, W> *edge_tail_junk;

		size_t size_;

	public:

		typedef std::forward_iterator_tag iterator_category;

		typedef list_vertex_iterator<false> vertex_iterator;
		typedef list_vertex_iterator<true> const_vertex_iterator;
		typedef list_edge_iterator<false> edge_iterator;
		typedef list_edge_iterator<true> const_edge_iterator;

		graph_list();
		~graph_list() throw();

		// copy ctor; move ctor; copy assign; move assign
		graph_list(const graph_list& node) = delete;
		graph_list& operator=(const graph_list& node) = delete;
		graph_list(graph_list&& node) = delete;
		graph_list& operator=(graph_list&& node) = delete;

		vertex_iterator begin() const { return vertex_iterator(head); }
		vertex_iterator end() const { return vertex_iterator(tail_junk); }

		const_vertex_iterator cbegin() const { return const_vertex_iterator(head); }
		const_vertex_iterator cend() const { return const_vertex_iterator(tail_junk); }

		edge_iterator edge_begin(VertexNode<T, W>* node) const { return edge_iterator(node->edge_head); }
		edge_iterator edge_end() const { return edge_iterator(edge_tail_junk); }

		const_edge_iterator edge_cbegin(VertexNode<T, W>* node) const { return const_edge_iterator(node->edge_head); }
		const_edge_iterator edge_cend() const { return const_edge_iterator(edge_tail_junk); }

		// Add nodes
		auto add_vertex(T data) -> VertexNode<T, W>*;
		auto add_edge(W weight, VertexNode<T, W>* from, VertexNode<T, W>* to) -> void;

		// Remove nodes
		// O(V * E)
		auto remove_vertex(VertexNode<T, W>* node) throw(std::invalid_argument) -> void;
		// O(E)
		auto remove_egde(VertexNode<T, W>* from, VertexNode<T, W>* to) throw(std::invalid_argument) -> void;

		// Populate vertex nodes
		auto fill_vertex_with(std::initializer_list<T> values) -> void;

		template <class Iter, class Enable = std::enable_if<
			(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
			std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>
		auto fill_vertex_with(const Iter& _begin, const Iter& _end) -> void;

		// Service functions

		// O(1) | returns vertex node count
		inline auto size() const -> size_t { return size_; };
		// O(n) | returns giveb vertex's edge count
		auto edge_size(VertexNode<T, W>* node) const throw(std::invalid_argument)->size_t;
		inline auto empty() -> bool { return begin() == end(); };

		// Print functions
		auto print_graph(std::ostream& stream = std::cout) -> std::ostream&;
		auto print_edges(VertexNode<T, W>* node, std::ostream& stream = std::cout) -> std::ostream&;

		// Clear data
		auto clear() throw() -> void;
	};

	template <class T, class W>
	graph_list<T, W>::graph_list()
	{ 
		tail_junk = new VertexNode<T, W>(T{}, false, nullptr, nullptr);
		head = tail = tail_junk;

		edge_tail_junk = new EdgeNode<T, W>(W{}, nullptr, nullptr);
	};

	template <class T, class W>
	graph_list<T, W>::~graph_list() throw()
	{
		clear();

		delete tail_junk;
		delete edge_tail_junk;
	}

	template <class T, class W>
	auto graph_list<T, W>::add_vertex(T data) -> VertexNode<T, W>*
	{
		auto node = new VertexNode<T, W>(data, false, tail_junk, edge_tail_junk);

		// Tail
		if (tail_junk != tail)
			tail->next = node;

		tail = node;
		node->next = tail_junk;

		// Head
		if (tail_junk == head)
			head = node;

		size_++;

		return node;
	}

	template <class T, class W>
	auto graph_list<T, W>::add_edge(W weight, VertexNode<T, W>* from, VertexNode<T, W>* to) -> void
	{
		EdgeNode<T, W>* node = new EdgeNode<T, W>(weight, to, edge_tail_junk);

		if (from->edge_head == edge_tail_junk)
			from->edge_head = node;
		else
		{
			// O(n) | Find last valid node
			auto raw_it = from->edge_head;

			while (raw_it->next != edge_tail_junk)
				raw_it = raw_it->next;

			raw_it->next = node;
		}

		node->next = edge_tail_junk;
	}

	template <class T, class W>
	auto graph_list<T, W>::remove_vertex(VertexNode<T, W>* node) throw(std::invalid_argument) -> void
	{
		// Remove all vertex' edges
		EdgeNode<T, W>* curr = node->edge_head, *e_tmp{};
		VertexNode<T, W> *v_temp{};

		// First: Remove all edges in other vertixes that point to the one that's being removed
		v_temp = head;
		while (v_temp != tail_junk)
		{
			remove_egde(v_temp, node);
			v_temp = v_temp->next;
		}

		// Second: Remove this vertex' all edges
		while (curr && curr != edge_tail_junk)
		{
			e_tmp = curr->next;
			delete curr;
			curr = e_tmp;
		}

		// Third: Remove the vertex itself
		
		// Deleting head vertex?
		if (head == node)
		{
			v_temp = head->next;
			delete head;
			head = v_temp;
		}
		else
		{
			// Find predecessor
			v_temp = head;

			while (v_temp->next != node && v_temp != tail_junk)
				v_temp = v_temp->next;

			// Unexpected situation
			if (v_temp == tail_junk)
				throw std::invalid_argument("Unknown vertex node");

			v_temp->next = node->next;
			delete node;
		}

		size_--;

		// Clean up things
		if (size_ == 0)
			head = tail = tail_junk;
	}

	template <class T, class W>
	auto graph_list<T, W>::remove_egde(VertexNode<T, W>* from, VertexNode<T, W>* to) throw(std::invalid_argument) -> void
	{
		if (from == nullptr || to == nullptr)
			throw std::invalid_argument("Vertex nodes can't be null");

		if (from->edge_head == edge_tail_junk)
			return;

		EdgeNode<T, W>* tmp{};

		// Removing head edge?
		if (from->edge_head->connects_to == to)
		{
			// Removing last edge?
			if (from->edge_head->next == edge_tail_junk)
			{
				delete from->edge_head;
				from->edge_head = edge_tail_junk;
			}
			else
			{
				tmp = from->edge_head->next;
				delete from->edge_head;
				from->edge_head = tmp;
			}

			return;
		}

		// Find egde to delete and its predecessor
		auto eit = from->edge_head, pred = from->edge_head;
		while (eit != edge_tail_junk)
		{
			if (eit->connects_to == to)
			{
				pred->next = eit->next;
				delete eit;
				
				return;
			}

			pred = eit;
			eit = eit->next;
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::edge_size(VertexNode<T, W>* node) const throw(std::invalid_argument) -> size_t
	{
		if (node == nullptr)
			throw std::invalid_argument("Vertex node can't be null");
		
		if (node->edge_head == edge_tail_junk)
			return size_t{};

		size_t edge_count{};

		auto eit = node->edge_head;
		while (eit != edge_tail_junk)
		{
			edge_count++;
			eit = eit->next;
		}

		return edge_count;
	}

	template <class T, class W>
	auto graph_list<T, W>::print_graph(std::ostream& stream = std::cout) -> std::ostream&
	{
		auto vit = begin();
		while (vit != end())
		{
			stream << *vit;

			auto eit = edge_begin(vit.get_node());
			if (eit != edge_end())
			{
				stream << "--> " << eit.get_node()->connects_to->data << "-" << *eit;

				while (++eit != edge_end())
					stream << ", " << eit.get_node()->connects_to->data << "-" << *eit;
			}

			stream << std::endl;
			vit++;
		}

		return stream;
	}

	template <class T, class W>
	auto graph_list<T, W>::print_edges(VertexNode<T, W>* node, std::ostream& stream = std::cout)->std::ostream&
	{
		if (node == nullptr)
			return stream;

		auto eit = edge_begin(node);

		if (eit != edge_end())
		{
			stream << "Vertex " << node->data << ": ";

			stream << eit.get_node()->connects_to->data << "-" << *eit;
			while (++eit != edge_end())
				stream << "; " << eit.get_node()->connects_to->data << "-" << *eit;
		}
		else
			stream << "Vertex " << node->data << " has no edges";

		stream << std::endl;

		return stream;
	}

	template <class T, class W>
	auto graph_list<T, W>::clear() throw() -> void
	{
		try
		{
			if (head == tail_junk)
				return;

			VertexNode<T, W>* vit = head, *tmp{};
			while (vit != tail_junk)
			{
				tmp = vit->next;
				remove_vertex(vit);
				vit = tmp;
			}
		
			head = tail = tail_junk;
		}
		catch (...)
		{
			terminate();
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::fill_vertex_with(std::initializer_list<T> values) -> void
	{
		clear();

		for (auto &v : values)
			add_vertex(v);
	}

	template <class T, class W>
	template <class Iter, class Enable = std::enable_if<
		(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
	>::type>
	auto graph_list<T, W>::fill_vertex_with(const Iter& _begin, const Iter& _end) -> void
	{
		clear();

		auto it = _begin;
		while (it != _end)
			add_vertex(*it++);
	}
}