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

// std::function
#include <functional>

// std::stack
#include <deque>

// std::tuple, std::get
#include <tuple>

#include "hadt_common.hpp"


namespace hadt {

	template <class T, class W>
	class graph_list
	{
	protected:

		/* Internal class : [Const] Vertex Iterator */

		template <bool IsConst = false>
		class list_vertex_iterator : public std::iterator <std::forward_iterator_tag, T>
		{
			// Works in VC++, but not in GCC, due to
			// "Explicit specialization in non-namespace scope" error
			// Read more at http://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope

			/* ----
			
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

			---- */

		public:

			typedef T value_type;
			// T& / const T&
			typedef typename hadt::vertex_iterator_base<T, W, IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename hadt::vertex_iterator_base<T, W, IsConst>::iterator_pointer pointer;
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
			typename hadt::vertex_iterator_base<T, W, IsConst>::iterator_value_type_ptr ptr_;
		};

		/* Internal class : [Const] Edge Iterator */

		template <bool IsConst = false>
		class list_edge_iterator : public std::iterator <std::forward_iterator_tag, W>
		{
		public:

			typedef T value_type;
			// T& / const T&
			typedef typename hadt::edge_iterator_base<T, W, IsConst>::iterator_reference reference;
			// T* / const T*
			typedef typename hadt::edge_iterator_base<T, W, IsConst>::iterator_pointer pointer;
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
			typename hadt::edge_iterator_base<T, W, IsConst>::iterator_value_type_ptr ptr_;
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

		// TODO: add next() function to iterators ?

		vertex_iterator begin() const { return vertex_iterator(head); }
		vertex_iterator end() const { return vertex_iterator(tail_junk); }

		const_vertex_iterator cbegin() const { return const_vertex_iterator(head); }
		const_vertex_iterator cend() const { return const_vertex_iterator(tail_junk); }

		edge_iterator edge_begin(VertexNode<T, W>* node) const { return edge_iterator(node->edge_head); }
		edge_iterator edge_end() const { return edge_iterator(edge_tail_junk); }

		const_edge_iterator edge_cbegin(VertexNode<T, W>* node) const { return const_edge_iterator(node->edge_head); }
		const_edge_iterator edge_cend() const { return const_edge_iterator(edge_tail_junk); }

		// Add nodes
		auto add_vertex(T vertex_data) -> VertexNode<T, W>*;
		auto add_vertices(std::initializer_list<T> vertices) -> void;

		auto add_edge(W weight, VertexNode<T, W>* const from, VertexNode<T, W>* const to) -> void;
		auto add_edge(W weight, T from, T to) throw(std::invalid_argument) -> void;
		auto add_edges(std::initializer_list<std::tuple<W, T, T>> edges) throw(std::invalid_argument) -> void;
		
		// Remove nodes
		// O(V * E)
		auto remove_vertex(VertexNode<T, W>* node) throw(std::invalid_argument) -> void;
		// O(E)
		auto remove_egde(VertexNode<T, W>* from, VertexNode<T, W>* to) throw(std::invalid_argument) -> void;

		// Populate vertex nodes
		auto fill_vertices(std::initializer_list<T> values) -> void;

		template <class Iter, class Enable = typename std::enable_if<
			(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
			std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
			std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
		>::type>
		auto fill_vertices(const Iter& _begin, const Iter& _end) -> void;

		// Graph Traversal
		// TODO: make difference between DFS/BFS and Preorder/Inorder/Postorder traversal
		auto bfs_vertex_map(std::function<void(T&)> map_func) -> void; // Breadth-First Search
		auto dfs_vertex_map(std::function<void(T&)> map_func) -> void; // Depth-First-Search

		// Functional Programming (FP) Routines

		// Apply <map> function to all vertices
		auto vertex_map(std::function<void(T&)> map_func) -> void;
		// Apply <fold> function to all vertices
		template <class Res> auto vertex_fold(T& acc, std::function<Res(T)> fold_func) -> void;

		// Apply <map> function to all edges of a given vertex
		auto edge_map(VertexNode<T, W>* node, std::function<void(W&)> map_func) throw(std::invalid_argument) -> void;
		auto edge_map(const T& data_to_find, std::function<void(W&)> map_func) throw(std::invalid_argument) -> void;

		// Apply <fold> function to all edges of a given vertex
		template <class Res> 
		auto edge_fold(W& acc, VertexNode<T, W>* node, std::function<Res(W)> fold_func) throw(std::invalid_argument) -> void;
		template <class Res> 
		auto edge_fold(W& acc, const T& data_to_find, std::function<Res(W)> fold_func) throw(std::invalid_argument) -> void;

		// Search paths (from root to the bottom) using DFS traversal and 
		// applying a map/fold function to each found path
		template <class Cont, class Oper>
		auto dfs_path_fold(Cont& container, std::function<typename Cont::value_type(T)> transform_func) -> void;

		/* Service functions */

		// O(1) | returns vertex node count
		inline auto size() const -> size_t { return size_; };
		// O(n) | returns giveb vertex's edge count
		auto edge_size(const VertexNode<T, W>* const node) const throw(std::invalid_argument) -> size_t;
		auto edge_size(const T& vertex_data) const throw(std::invalid_argument) -> size_t;
		
		inline auto empty() -> bool { return begin() == end(); };

		// Returns VertexNode's data
		auto vertex_data(const VertexNode<T, W>* const node) const throw(std::invalid_argument) -> T;

		// O(n) | clears <is_visited> flag
		auto clear_visited() -> void;

		// TODO: add IsReachable function

		// Print functions
		auto print_graph(std::ostream& stream = std::cout) -> std::ostream&;
		auto print_edges(VertexNode<T, W>* node, std::ostream& stream = std::cout) -> std::ostream&;

		// O(1) | returns initial vertex
		auto root() const -> const VertexNode<T, W>* { return head; }

		// Clear data
		auto clear() throw() -> void;

	private:

		// Find vertex (returns first found node with the given value to search)
		auto find_vertex(const T& data_to_found) const->VertexNode<T, W>*;
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

		if (tail_junk != nullptr)
			delete tail_junk;

		if (edge_tail_junk != nullptr)
			delete edge_tail_junk;
	}

	template <class T, class W>
	auto graph_list<T, W>::clear_visited() -> void
	{
		for (auto vit = begin(); vit != end(); vit++)
			vit.get_node()->is_visited = false;
	}

	template <class T, class W>
	auto graph_list<T, W>::bfs_vertex_map(std::function<void(T&)> map_func) -> void
	{
		using VNode = VertexNode<T, W>*;

		// Breadth-First Traversal
		clear_visited();

		VNode node;
		std::deque<VNode> v_list;

		// Push to Back ...
		v_list.push_back(head);
		while (!v_list.empty())
		{
			// ... and Pull from Front
			node = v_list.front();
			v_list.pop_front();

			if (node->is_visited)
				continue;

			map_func(node->data);
			node->is_visited = true;
			for (auto it = edge_begin(node); it != edge_end(); it++)
				v_list.push_back(it.get_node()->connects_to);
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::dfs_vertex_map(std::function<void(T&)> map_func) -> void
	{
		using VNode = VertexNode<T, W>*;

		// Depth-First Traversal
		clear_visited();

		VNode node;
		std::deque<VNode> v_list;
		std::vector<VNode> v_reverse;

		// Push to Back ...
		v_list.push_back(head);
		while (!v_list.empty())
		{
			// ... and Pull from Back
			node = v_list.back();
			v_list.pop_back();

			if (node->is_visited)
				continue;

			map_func(node->data);
			node->is_visited = true;
			
			// In order to receive valid graph order (from left to right)
			// we need to put new nodes in reverse order (due to internal order)
			v_reverse.clear();
			for (auto it = edge_begin(node); it != edge_end(); it++)
				v_reverse.push_back(it.get_node()->connects_to);
			
			// Put childs in reverse order (in order that is opposite to what we have in VertexNode)
			auto it = v_reverse.rbegin();
			while (it != v_reverse.rend())
				v_list.push_back(*it++);
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::vertex_map(std::function<void(T&)> map_func) -> void
	{
		for (auto it = begin(); it != end(); it++)
			map_func(*it);
	}

	template <class T, class W>
	template <class Res>
	auto graph_list<T, W>::vertex_fold(T& acc, std::function<Res(T)> fold_func) -> void
	{
		for (auto it = begin(); it != end(); it++)
			acc += fold_func(*it);
	}

	template <class T, class W>
	auto graph_list<T, W>::edge_map(VertexNode<T, W>* node, std::function<void(W&)> map_func) throw(std::invalid_argument) -> void
	{
		if (node == nullptr)
			throw std::invalid_argument("Vertex node can't be null");

		for (auto it = edge_begin(node); it != edge_end(); it++)
			map_func(*it);
	}

	template <class T, class W>
	auto graph_list<T, W>::edge_map(const T& data_to_find, std::function<void(W&)> map_func) throw(std::invalid_argument) -> void
	{
		auto found = find_vertex(data_to_find);
		if (found == nullptr)
			throw std::invalid_argument("Can't find a vertex node with given value");

		edge_map(found, map_func);
	}

	template <class T, class W>
	template <class Res>
	auto graph_list<T, W>::edge_fold(W& acc, VertexNode<T, W>* node, std::function<Res(W)> fold_func) throw(std::invalid_argument) -> void
	{
		if (node == nullptr)
			throw std::invalid_argument("Vertex node can't be null");

		for (auto it = edge_begin(node); it != edge_end(); it++)
			acc += fold_func(*it);
	}

	template <class T, class W>
	template <class Res>
	auto graph_list<T, W>::edge_fold(W& acc, const T& data_to_find, std::function<Res(W)> fold_func) throw(std::invalid_argument) -> void
	{
		auto found = find_vertex(data_to_find);
		if (found == nullptr)
			throw std::invalid_argument("Can't find a vertex node with given value");

		edge_fold(acc, found, fold_func);
	}

	template <class T, class W>
	auto edge_map(VertexNode<T, W>* node, std::function<void(W&)> map_func) -> void
	{
		for (auto it = edge_begin(node); it != VertexNode<T, W>::edge_end(); it++)
			map_func(*it);
	}

	template <class T, class W>
	auto edge_map(const T& data_to_find, std::function<void(W&)> map_func) -> void
	{
		auto found = find_vertex(data_to_find);
		if (found == nullptr)
			throw std::invalid_argument("Can't find a vertex node with given value");

		for (auto it = edge_begin(found); it != VertexNode<T, W>::edge_end(); it++)
			map_func(*it);
	}

	template <class T, class W>
	template <class Cont, class Oper>
	auto graph_list<T, W>::dfs_path_fold(Cont& container, std::function<typename Cont::value_type(T)> transform_func) -> void
	{
		using ContVal = typename Cont::value_type;
		using VNode = VertexNode<T, W>*;

		// Depth-First Traversal
		clear_visited();

		Oper operation;

		// need 'typename' before 'Cont:: value_type' because 'Cont' is a dependent scope
		ContVal acc_value{};
		
		VNode temp{};
		std::deque<VNode> v_list;

		v_list.push_back(head);
		while (!v_list.empty())
		{
			temp = v_list.back();
			temp->is_visited = true;

			if (temp->edge_head == edge_tail_junk)
			{
				// Store the given path's value
				for (auto it = v_list.begin(); it != v_list.end(); it++)
					acc_value = operation(acc_value, transform_func((*it)->data));

				container.push_back(acc_value);
				acc_value = ContVal{};
				v_list.pop_back();
			}
			else
			{
				bool all_visited = true;
				for (auto it = edge_begin(temp); it != edge_end(); it++)
				{
					if (it.get_node()->connects_to->is_visited == false)
					{
						v_list.push_back(it.get_node()->connects_to);
						all_visited = false;
						break;
					}
				}

				// Remove from the list only if all childs are visited
				if (all_visited)
				{
					// Refresh childs of this vertex node before going to a higher level
					for (auto it = edge_begin(v_list.back()); it != edge_end(); it++)
						it.get_node()->connects_to->is_visited = false;

					v_list.pop_back();
				}
			}
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::vertex_data(const VertexNode<T, W>* const node) const throw(std::invalid_argument) -> T
	{
		if (node == nullptr)
			throw std::invalid_argument("Vertex node can't be null");

		return node->data;
	}

	template <class T, class W>
	auto graph_list<T, W>::add_vertex(T vertex_data) -> VertexNode<T, W>*
	{
		auto node = new VertexNode<T, W>(vertex_data, false, tail_junk, edge_tail_junk);

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
	auto graph_list<T, W>::add_vertices(std::initializer_list<T> vertices) -> void
	{
		for (auto &v : vertices)
			add_vertex(v);
	}

	template <class T, class W>
	auto graph_list<T, W>::add_edge(W weight, VertexNode<T, W>* const from, VertexNode<T, W>* const to) -> void
	{
		auto node = new EdgeNode<T, W>(weight, to, edge_tail_junk);

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
	auto graph_list<T, W>::add_edge(W weight, T from, T to) throw(std::invalid_argument) -> void
	{
		auto first = find_vertex(from);
		auto second = find_vertex(to);

		if (first == nullptr || second == nullptr)
			throw std::invalid_argument("Given values are invalid. Can't find a vertex");

		add_edge(weight, first, second);
	}

	template <class T, class W>
	auto graph_list<T, W>::add_edges(std::initializer_list<std::tuple<W, T, T>> edges) throw(std::invalid_argument) -> void
	{
		for (auto &e : edges)
			add_edge(std::get<0>(e), std::get<1>(e), std::get<2>(e));
	}

	template <class T, class W>
	auto graph_list<T, W>::find_vertex(const T& data_to_found) const -> VertexNode<T, W>*
	{
		for (auto it = begin(); it != end(); it++)
			if (*it == data_to_found)
				return it.get_node();

		// Means "not found"
		return nullptr;
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
	auto graph_list<T, W>::edge_size(const VertexNode<T, W>* const node) const throw(std::invalid_argument) -> size_t
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
	auto graph_list<T, W>::edge_size(const T& vertex_data) const throw(std::invalid_argument) -> size_t
	{
		auto found = find_vertex(vertex_data);
		if (found == nullptr)
			throw std::invalid_argument("Can't find a vertex node with given value");
		
		return edge_size(found);
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
			std::terminate();
		}
	}

	template <class T, class W>
	auto graph_list<T, W>::fill_vertices(std::initializer_list<T> vertices) -> void
	{
		clear();

		for (auto &v : vertices)
			add_vertex(v);
	}

	template <class T, class W>
	template <class Iter, class Enable = typename std::enable_if<
		(std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::forward_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>::value ||
		std::is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value) &&
		std::is_same<typename std::iterator_traits<Iter>::value_type, T>::value
	>::type>
	auto graph_list<T, W>::fill_vertices(const Iter& _begin, const Iter& _end) -> void
	{
		clear();

		auto it = _begin;
		while (it != _end)
			add_vertex(*it++);
	}
}