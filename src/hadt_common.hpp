#pragma once
/* Contains 
 *   -- nodes definitions for different kind of lists
 *   -- helpers for defining iterators
 *	 -- helper functions
 */

namespace hadt {

	template <class T, class W>
	static auto make_edge(W a1, T a2, T a3) -> std::tuple<W, T, T>
	{
		return std::make_tuple(a1, a2, a3);
	};

	namespace nodes {

		template <class T, class W>
		class EdgeNode;

		template <class T, class W>
		class VertexNode;

		/* External class: Node Data */
		template <class T>
		class HNode
		{
		public:
			T data;

			HNode *next;
			HNode *prev;

			explicit HNode(T d) :next{ nullptr }, prev{ nullptr } { data = d; };
			HNode(T d, HNode *n, HNode *p) : next{ n }, prev{ p } { data = d; };

			// copy ctor; move ctor; copy assign; move assign
			HNode(const HNode& node) = delete;
			HNode& operator=(const HNode& node) = delete;
			HNode(HNode&& node) = delete;
			HNode& operator=(HNode&& node) = delete;
		};

		/* External class : Vertex Data */
		template <class T, class W>
		class VertexNode
		{
		public:
			T data;
			bool is_visited;
			VertexNode *next;
			EdgeNode<T, W> *edge_head;

			explicit VertexNode(T d)
				: is_visited{ false }, next{ nullptr }, edge_head{ nullptr }
			{
				data = d;
			};
			VertexNode(T d, bool vis, VertexNode* vn, EdgeNode<T, W>* en)
				: is_visited{ vis }, next{ vn }, edge_head{ en } {
				data = d;
			};
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

	}

	namespace iterators {

		template <class T, bool is_const>
		struct node_iterator_base {};

		template <class T, class W, bool is_const>
		struct vertex_iterator_base {};

		template <class T, class W, bool is_const>
		struct edge_iterator_base {};

		/* Specializations for iterator */

		template <class T>
		struct node_iterator_base<T, false>
		{
			typedef T& iterator_reference;
			typedef T* iterator_pointer;
			typedef hadt::nodes::HNode<T>* iterator_value_type_ptr;
		};

		template <class T, class W>
		struct vertex_iterator_base<T, W, false>
		{
			typedef T& iterator_reference;
			typedef T* iterator_pointer;
			typedef hadt::nodes::VertexNode<T, W>* iterator_value_type_ptr;
		};

		template <class T, class W>
		struct edge_iterator_base<T, W, false>
		{
			typedef W& iterator_reference;
			typedef W* iterator_pointer;
			typedef hadt::nodes::EdgeNode<T, W>* iterator_value_type_ptr;
		};

		/* Specializations for const_iterator */

		template <class T>
		struct node_iterator_base<T, true>
		{
			typedef const T& iterator_reference;
			typedef const T* iterator_pointer;
			typedef const hadt::nodes::HNode<T>* iterator_value_type_ptr;
		};

		template <class T, class W>
		struct vertex_iterator_base<T, W, true>
		{
			typedef const T& iterator_reference;
			typedef const T* iterator_pointer;
			typedef const hadt::nodes::VertexNode<T, W>* iterator_value_type_ptr;
		};

		template <class T, class W>
		struct edge_iterator_base<T, W, true>
		{
			typedef const W& iterator_reference;
			typedef const W* iterator_pointer;
			typedef const hadt::nodes::EdgeNode<T, W>* iterator_value_type_ptr;
		};

	}

	template <class T>
	using HNode = nodes::HNode<T>;

	template <class T, class W>
	using VertexNode = nodes::VertexNode<T, W>;

	template <class T, class W>
	using EdgeNode = nodes::EdgeNode<T, W>;

	template <class T, bool is_const>
	using node_iterator_base = iterators::node_iterator_base<T, is_const>;

	template <class T, class W, bool is_const>
	using vertex_iterator_base = iterators::vertex_iterator_base<T, W, is_const>;

	template <class T, class W, bool is_const>
	using edge_iterator_base = iterators::edge_iterator_base<T, W, is_const>;

}