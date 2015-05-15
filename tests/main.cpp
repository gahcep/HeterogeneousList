// main.cpp
#define CATCH_CONFIG_MAIN

#include <vector>
#include <list>

#include "../src/hadt_list.hpp"
#include "../src/hadt_forward_list.hpp"
#include "../src/hadt_graph_list.hpp"

#include "../modules/catch/single_include/catch.hpp"

using hadt::list;
using hadt::forward_list;
using hadt::graph_list;

/*
	Aliases:
		-- SLL -> Singly-Linked List
		-- DLL -> Doubly-Linked List
		-- GAL -> Graph Adjacent List
*/

TEST_CASE("List Creation: ", "[Methods][Construction][SLL][DLL][GAL]")
{
	std::vector<int> fill_vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::list<int> fill_list{ 10, 20, 30, 40, 50, 60, 70, 80, 90 };

	SECTION("for SLL")
	{
		forward_list<int> lst;

		REQUIRE(lst.begin() == lst.end());

		lst.fill_with({ 1, 3, 5, 7, 11, 13 });
		REQUIRE(lst.size() == 6);

		lst.clear();
		REQUIRE(lst.begin() == lst.end());

		lst.fill_with(fill_vector.begin(), fill_vector.end());
		REQUIRE(lst.size() == 9);

		lst.fill_with(fill_list.begin(), fill_list.end());
		REQUIRE(lst.size() == 9);
	}

	SECTION("for DLL")
	{
		list<int> lst;

		REQUIRE(lst.begin() == lst.end());

		lst.fill_with({ 1, 3, 5, 7, 11, 13 });
		REQUIRE(lst.size() == 6);

		lst.clear();
		REQUIRE(lst.begin() == lst.end());

		lst.fill_with(fill_vector.begin(), fill_vector.end());
		REQUIRE(lst.size() == 9);

		lst.fill_with(fill_list.begin(), fill_list.end());
		REQUIRE(lst.size() == 9);
	}

	SECTION("for GAL")
	{
		graph_list<int, int> lst;

		REQUIRE(lst.begin() == lst.end());

		lst.fill_vertex_with({ 1, 3, 5, 7, 11, 13 });
		REQUIRE(lst.size() == 6);

		lst.clear();
		REQUIRE(lst.begin() == lst.end());

		lst.fill_vertex_with(fill_vector.begin(), fill_vector.end());
		REQUIRE(lst.size() == 9);

		lst.fill_vertex_with(fill_list.begin(), fill_list.end());
		REQUIRE(lst.size() == 9);
	}
}

TEST_CASE("Vertexes/Edges Creation", "[Methods][Construction][GAL]")
{
	graph_list<char, int> glist;

	auto node_a = glist.add_vertex('A');
	auto node_c = glist.add_vertex('C');
	auto node_e = glist.add_vertex('E');
	auto node_h = glist.add_vertex('H');
	auto node_w = glist.add_vertex('W');
	REQUIRE(glist.size() == 5);
	REQUIRE(glist.edge_size(node_a) == 0);
	REQUIRE(glist.edge_size(node_h) == 0);

	glist.add_edge(0, node_a, node_e);
	glist.add_edge(0, node_a, node_w);
	glist.add_edge(0, node_c, node_h);
	REQUIRE(glist.edge_size(node_a) == 2);
	REQUIRE(glist.edge_size(node_c) == 1);
	REQUIRE(glist.edge_size(node_w) == 0);
}

TEST_CASE("List Append / Prepand", "[Methods][Construction][SLL][DLL]")
{
	forward_list<int> slist;

	std::vector<int> fill_vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::list<int> fill_list{ 10, 20, 30, 40, 50, 60, 70, 80, 90 };

	slist.append_with(fill_vector.begin(), fill_vector.end());
	slist.prepend_with(fill_list.begin(), fill_list.end());
	REQUIRE(slist.size() == 18);
	REQUIRE(slist.at_front() == 90);
	REQUIRE(slist.at_back() == 9);
}

TEST_CASE("List Emptyness", "[Construction][SLL][DLL][GAL]")
{
	list<int> slist;
	forward_list<int> dlist;
	graph_list<int, int> glist;

	REQUIRE(slist.empty());
	REQUIRE(dlist.empty());
	REQUIRE(glist.empty());

	slist.push_front(1);
	dlist.push_front(1);
	glist.add_vertex(1);

	REQUIRE_FALSE(slist.empty());
	REQUIRE_FALSE(dlist.empty());
	REQUIRE_FALSE(glist.empty());
}

TEST_CASE("push_xxx() vs move_xxx()", "[Methods][SLL][DLL]")
{
	using std::string;

	forward_list<std::string> slist;

	REQUIRE(slist.empty() == true);

	string str_a{ "str_a" };
	string str_b{ "str_b" };

	SECTION("push_front vs move_front")
	{
		slist.push_front(str_a);
		REQUIRE(str_a == string{ "str_a" });

		slist.move_front(std::move(str_b));
		REQUIRE(str_b == string{});
	}

	SECTION("push_back vs move_back")
	{
		slist.push_back(str_a);
		REQUIRE(str_a == string{ "str_a" });

		slist.move_back(std::move(str_b));
		REQUIRE(str_b == string{});
	}

	REQUIRE(slist.empty() == false);
}

TEST_CASE("push_front & Iterators", "[Methods][Iterators][SLL][DLL]")
{
	SECTION("... for singly-linked list")
	{
		forward_list<int> slist;

		slist.push_front(10);
		slist.push_front(100);
		slist.push_front(1000);

		REQUIRE(slist.empty() == false);

		// Iterator
		auto slist_it = slist.begin();

		REQUIRE(*slist_it++ == 1000);
		REQUIRE(*slist_it++ == 100);
		REQUIRE(*slist_it++ == 10);
		REQUIRE(slist_it++ == slist.end());

		// Const Iterator
		auto slist_const_it = slist.cbegin();

		REQUIRE(*slist_const_it++ == 1000);
		REQUIRE(*slist_const_it++ == 100);
		REQUIRE(*slist_const_it++ == 10);
		REQUIRE(slist_const_it++ == slist.cend());
	}

	SECTION("... for doubly-linked list")
	{
		list<int> dlist;

		dlist.push_front(10);
		dlist.push_front(100);
		dlist.push_front(1000);

		REQUIRE(dlist.empty() == false);

		// Iterator
		auto slist_it = dlist.begin();

		REQUIRE(*slist_it++ == 1000);
		REQUIRE(*slist_it++ == 100);
		REQUIRE(*slist_it++ == 10);
		REQUIRE(slist_it++ == dlist.end());

		// Const Iterator
		auto dlist_const_it = dlist.cbegin();

		REQUIRE(*dlist_const_it++ == 1000);
		REQUIRE(*dlist_const_it++ == 100);
		REQUIRE(*dlist_const_it++ == 10);
		REQUIRE(dlist_const_it++ == dlist.cend());

		// Reverse Iterator
		auto dlist_rev_it = dlist.rbegin();

		REQUIRE(*dlist_rev_it++ == 10);
		REQUIRE(*dlist_rev_it++ == 100);
		REQUIRE(*dlist_rev_it++ == 1000);
		REQUIRE(dlist_rev_it++ == dlist.rend());

		// Const Reverse Iterator
		auto dlist_crev_it = dlist.rcbegin();

		REQUIRE(*dlist_crev_it++ == 10);
		REQUIRE(*dlist_crev_it++ == 100);
		REQUIRE(*dlist_crev_it++ == 1000);
		REQUIRE(dlist_crev_it++ == dlist.rcend());
	}
}

TEST_CASE("push_back & Iterators", "[Methods][Iterators][SLL][DLL]")
{
	SECTION("... for singly-linked list")
	{
		forward_list<int> slist;

		slist.push_back(10);
		slist.push_back(100);
		slist.push_back(1000);

		REQUIRE(slist.empty() == false);

		// Iterator
		auto slist_it = slist.begin();

		REQUIRE(*slist_it++ == 10);
		REQUIRE(*slist_it++ == 100);
		REQUIRE(*slist_it++ == 1000);
		REQUIRE(slist_it++ == slist.end());

		// Const Iterator
		auto slist_const_it = slist.cbegin();

		REQUIRE(*slist_const_it++ == 10);
		REQUIRE(*slist_const_it++ == 100);
		REQUIRE(*slist_const_it++ == 1000);
		REQUIRE(slist_const_it++ == slist.cend());
	}

	SECTION("... for doubly-linked list")
	{
		list<int> dlist;

		dlist.push_back(10);
		dlist.push_back(100);
		dlist.push_back(1000);

		REQUIRE(dlist.empty() == false);

		// Iterator
		auto slist_it = dlist.begin();

		REQUIRE(*slist_it++ == 10);
		REQUIRE(*slist_it++ == 100);
		REQUIRE(*slist_it++ == 1000);
		REQUIRE(slist_it++ == dlist.end());

		// Const Iterator
		auto slist_const_it = dlist.cbegin();

		REQUIRE(*slist_const_it++ == 10);
		REQUIRE(*slist_const_it++ == 100);
		REQUIRE(*slist_const_it++ == 1000);
		REQUIRE(slist_const_it++ == dlist.cend());

		// Reverse Iterator
		auto slist_rev_it = dlist.rbegin();

		REQUIRE(*slist_rev_it++ == 1000);
		REQUIRE(*slist_rev_it++ == 100);
		REQUIRE(*slist_rev_it++ == 10);
		REQUIRE(slist_rev_it++ == dlist.rend());

		// Const Reverse Iterator
		auto slist_crev_it = dlist.rcbegin();

		REQUIRE(*slist_crev_it++ == 1000);
		REQUIRE(*slist_crev_it++ == 100);
		REQUIRE(*slist_crev_it++ == 10);
		REQUIRE(slist_crev_it++ == dlist.rcend());
	}
}

TEST_CASE("Vertex and Edge Iterators", "[Methods][Iterators][GAL]")
{
	graph_list<char, int> glist;

	auto node_a = glist.add_vertex('A');
	auto node_b = glist.add_vertex('B');
	auto node_c = glist.add_vertex('C');
	auto node_d = glist.add_vertex('D');

	glist.add_edge(4, node_a, node_b);
	glist.add_edge(5, node_a, node_c);
	glist.add_edge(2, node_b, node_c);

	auto vit = glist.begin();
	REQUIRE(*vit++ == 'A');
	REQUIRE(*vit++ == 'B');
	REQUIRE(*vit++ == 'C');
	REQUIRE(*vit++ == 'D');
	REQUIRE(vit == glist.end());

	auto eit_a = glist.edge_begin(node_a);
	REQUIRE(*eit_a++ == 4);
	REQUIRE(*eit_a++ == 5);
	REQUIRE(eit_a == glist.edge_end());

	auto eit_b = glist.edge_begin(node_b);
	REQUIRE(*eit_b++ == 2);
	REQUIRE(eit_b == glist.edge_end());

	// Complex check: 
	// First check before any edge created
	auto eit_d_1 = glist.edge_begin(node_d);
	REQUIRE(eit_d_1 == glist.edge_end());

	// Then check after one edge created and deleted
	REQUIRE(glist.edge_size(node_d) == 0);
	glist.add_edge(7, node_d, node_a);
	glist.remove_egde(node_d, node_a);
	REQUIRE(glist.edge_size(node_d) == 0);

	auto eit_d_2 = glist.edge_begin(node_d);
	REQUIRE(eit_d_2 == glist.edge_end());
}

TEST_CASE("Items Retrieval", "[Methods][SLL][DLL]")
{
	list<int> dlist;
	forward_list<int> slist;

	slist.push_front(1);
	dlist.push_front(1);
	slist.push_front(3);
	dlist.push_front(3);
	slist.push_front(5);
	dlist.push_front(5);
	slist.push_front(7);
	dlist.push_front(7);
	slist.push_front(11);
	dlist.push_front(11);

	// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
	REQUIRE(slist.at(0) == 11);
	REQUIRE(dlist.at(0) == 11);
	REQUIRE(slist.at_front() == 11);
	REQUIRE(dlist.at_front() == 11);
	REQUIRE(slist.at(3) == 3);
	REQUIRE(dlist.at(3) == 3);
	REQUIRE(slist.at(slist.size() - 1) == 1);
	REQUIRE(dlist.at(slist.size() - 1) == 1);
	REQUIRE(slist.at_back() == 1);
	REQUIRE(dlist.at_back() == 1);
	REQUIRE_THROWS_AS(slist.at(100), std::out_of_range);
	REQUIRE_THROWS_AS(dlist.at(100), std::out_of_range);
}

TEST_CASE("Different operations on singly-linked list", "[Methods][SLL]")
{
	forward_list<int> slist;

	slist.push_front(1);
	slist.push_front(3);
	slist.push_front(5);
	slist.push_front(7);
	slist.push_front(11);

	REQUIRE(slist.size() == 5);
	REQUIRE(slist.empty() == false);

	// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
	int val = slist.pop_front();
	REQUIRE(val == 11);
	REQUIRE(slist.size() == 4);

	// Sequence is: 7 -> 5 -> 3 -> 1
	val = slist.pop_back();
	REQUIRE(val == 1);
	REQUIRE(slist.size() == 3);

	// Sequence is: 7 -> 5 -> 3
	val = slist.pop_at(2);
	REQUIRE(val == 3);
	REQUIRE(slist.size() == 2);

	// Sequence is: 7 -> 5
	slist.pop_front();
	slist.pop_front();
	REQUIRE(slist.size() == 0);
	REQUIRE(slist.empty());
	REQUIRE(slist.begin() == slist.end());
}

TEST_CASE("Different operations on doubly-linked list", "[Methods][DLL]")
{
	list<int> dlist;

	dlist.prepend_with({ 1, 3, 5, 7 });
	dlist.push_front(11);

	REQUIRE(dlist.size() == 5);
	REQUIRE(dlist.empty() == false);

	// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
	int val = dlist.pop_front();
	REQUIRE(val == 11);
	REQUIRE(dlist.size() == 4);

	// Sequence is: 7 -> 5 -> 3 -> 1
	val = dlist.pop_back();
	REQUIRE(val == 1);
	REQUIRE(dlist.size() == 3);

	// Sequence is: 7 -> 5 -> 3
	val = dlist.pop_at(2);
	REQUIRE(val == 3);
	REQUIRE(dlist.size() == 2);

	// Sequence is: 7 -> 5
	dlist.pop_front();
	dlist.pop_front();
	REQUIRE(dlist.size() == 0);
	REQUIRE(dlist.empty() == true);
}

TEST_CASE("Removing items on singly-linked list", "[Methods][SLL]")
{
	forward_list<int> slist;

	slist.prepend_with({ 1, 3, 5, 7 });
	slist.push_front(11);

	SECTION("... via pop_front() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		REQUIRE_THROWS_AS(slist.pop_front(), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}
	
	SECTION("... via pop_back() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		REQUIRE_THROWS_AS(slist.pop_back(), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}

	SECTION("... via pop_at() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_at(1);
		slist.pop_at(1);
		slist.pop_at(1);
		slist.pop_at(0);
		slist.pop_at(0);
		REQUIRE_THROWS_AS(slist.pop_at(0), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}
}

TEST_CASE("Removing items on doubly-linked list", "[Methods][DLL]")
{
	forward_list<int> slist;

	slist.prepend_with({ 1, 3, 5, 7 });
	slist.push_front(11);

	SECTION("... via pop_front() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		slist.pop_front();
		REQUIRE_THROWS_AS(slist.pop_front(), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}

	SECTION("... via pop_back() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		slist.pop_back();
		REQUIRE_THROWS_AS(slist.pop_back(), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}

	SECTION("... via pop_at() only")
	{
		// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
		REQUIRE(slist.size() == 5);
		REQUIRE(slist.empty() == false);

		slist.pop_at(1);
		slist.pop_at(1);
		slist.pop_at(1);
		slist.pop_at(0);
		slist.pop_at(0);
		REQUIRE_THROWS_AS(slist.pop_at(0), std::length_error);
		REQUIRE(slist.size() == 0);
		REQUIRE(slist.empty() == true);
	}
}

TEST_CASE("List Search", "[Search][Iterator][SLL][DLL]")
{
	list<int> dlist;
	forward_list<int> slist;
	slist.fill_with({ 10, 11, 12, 13, 14, 15, 16, 17 });
	dlist.fill_with({ 10, 11, 12, 13, 14, 15, 16, 17 });

	auto sit = slist.find_first(15);
	REQUIRE(*sit == 15);
	auto dit = dlist.find_first(15);
	REQUIRE(*dit == 15);
	
	auto sit_end = slist.find_first(150);
	REQUIRE(sit_end == slist.end());
	auto dit_end = dlist.find_first(150);
	REQUIRE(dit_end == dlist.end());
}

TEST_CASE("Search M-to-Last", "[Methods][Search][SLL][DLL]")
{
	list<int> dlist;
	forward_list<int> slist;

	slist.fill_with({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });
	dlist.fill_with({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170 });

	auto s_it_1 = slist.find_nth_to_last(0);
	auto d_it_1 = dlist.find_nth_to_last(0);
	REQUIRE(*s_it_1 == 17);
	REQUIRE(*d_it_1 == 170);

	auto s_it_2 = slist.find_nth_to_last(slist.size() - 1);
	auto d_it_2 = dlist.find_nth_to_last(dlist.size() - 1);
	REQUIRE(*s_it_2 == 1);
	REQUIRE(*d_it_2 == 10);

	auto s_it_3 = slist.find_nth_to_last(5);
	auto d_it_3 = dlist.find_nth_to_last(5);
	REQUIRE(*s_it_3 == 12);
	REQUIRE(*d_it_3 == 120);

	REQUIRE_THROWS_AS(slist.find_nth_to_last(100), std::out_of_range);
	REQUIRE_THROWS_AS(dlist.find_nth_to_last(100), std::out_of_range);
}

TEST_CASE("Reversing items on singly-linked list", "[Methods][Reverse][SLL]")
{
	forward_list<int> slist, slist_empty, slist_one;
	
	slist_one.fill_with({ 777 });
	slist.fill_with({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

	// Reverse to external list
	SECTION("... into external list")
	{
		forward_list<int> slist_empty_reversed, slist_one_reversed, slist_reversed;

		slist_empty.reverse(slist_empty_reversed);
		slist_one.reverse(slist_one_reversed);
		slist.reverse(slist_reversed);

		REQUIRE(slist_empty.size() == 0);
		REQUIRE(slist_empty_reversed.size() == 0);

		REQUIRE(slist_one.size() == 1);
		REQUIRE(slist_one.at_front() == 777);
		REQUIRE(slist_one.at_back() == 777);
		REQUIRE(slist_one_reversed.at_front() == 777);
		REQUIRE(slist_one_reversed.at_back() == 777);

		REQUIRE(slist.size() == 17);
		REQUIRE(slist.at_front() == 1);
		REQUIRE(slist.at_back() == 17);
		REQUIRE(slist_reversed.size() == 17);
		REQUIRE(slist_reversed.at_front() == 17);
		REQUIRE(slist_reversed.at_back() == 1);
	}

	// Reverse in-place
	SECTION("... in-place")
	{
		REQUIRE(slist_empty.size() == 0);
		REQUIRE(slist_one.size() == 1);
		REQUIRE(slist_one.at_front() == 777);
		REQUIRE(slist_one.at_back() == 777);
		REQUIRE(slist.size() == 17);
		REQUIRE(slist.at_front() == 1);

		slist_empty.reverse_inplace();
		slist_one.reverse_inplace();
		slist.reverse_inplace();

		REQUIRE(slist_empty.size() == 0);
		REQUIRE(slist_one.size() == 1);
		REQUIRE(slist_one.at_front() == 777);
		REQUIRE(slist_one.at_back() == 777);
		REQUIRE(slist.size() == 17);
		REQUIRE(slist.at_front() == 17);
	}
}

TEST_CASE("Reversing items on doubly-linked list", "[Methods][Reverse][DLL]")
{
	list<int> dlist, dlist_empty, dlist_one;

	dlist_one.fill_with({ 777 });
	dlist.fill_with({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

	// Reverse to external list
	SECTION("... into external list")
	{
		list<int> dlist_empty_reversed, dlist_one_reversed, dlist_reversed;

		dlist_empty.reverse(dlist_empty_reversed);
		dlist_one.reverse(dlist_one_reversed);
		dlist.reverse(dlist_reversed);

		REQUIRE(dlist_empty.size() == 0);
		REQUIRE(dlist_empty_reversed.size() == 0);

		REQUIRE(dlist_one.size() == 1);
		REQUIRE(dlist_one.at_front() == 777);
		REQUIRE(dlist_one.at_back() == 777);
		REQUIRE(dlist_one_reversed.at_front() == 777);
		REQUIRE(dlist_one_reversed.at_back() == 777);

		REQUIRE(dlist.size() == 17);
		REQUIRE(dlist.at_front() == 1);
		REQUIRE(dlist.at_back() == 17);
		REQUIRE(dlist_reversed.size() == 17);
		REQUIRE(dlist_reversed.at_front() == 17);
		REQUIRE(dlist_reversed.at_back() == 1);
	}

	// Reverse in-place
	SECTION("... in-place")
	{
		REQUIRE(dlist_empty.size() == 0);
		REQUIRE(dlist_one.size() == 1);
		REQUIRE(dlist_one.at_front() == 777);
		REQUIRE(dlist_one.at_back() == 777);
		REQUIRE(dlist.size() == 17);
		REQUIRE(dlist.at_front() == 1);

		dlist_empty.reverse_inplace();
		dlist_one.reverse_inplace();
		dlist.reverse_inplace();

		REQUIRE(dlist_empty.size() == 0);
		REQUIRE(dlist_one.size() == 1);
		REQUIRE(dlist_one.at_front() == 777);
		REQUIRE(dlist_one.at_back() == 777);
		REQUIRE(dlist.size() == 17);
		REQUIRE(dlist.at_front() == 17);
	}
}

TEST_CASE("List Printing", "[Methods][Print][SLL][DLL]")
{
	list<int> dlist;
	forward_list<int> slist;
	
	std::ostringstream str_stream;

	slist.fill_with({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });
	dlist.fill_with({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

	slist.print(str_stream);
	REQUIRE(str_stream.str() == std::string("1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17"));

	str_stream.str("");
	str_stream.clear();
	dlist.print(str_stream);
	REQUIRE(str_stream.str() == std::string("1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17"));

	str_stream.str("");
	str_stream.clear();
	slist.print_reverse(str_stream);
	REQUIRE(str_stream.str() == std::string("17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1"));

	str_stream.str("");
	str_stream.clear();
	dlist.print_reverse(str_stream);
	REQUIRE(str_stream.str() == std::string("17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1"));
}

TEST_CASE("Removing / Adding vertexes / edges", "[Methods][GAL]")
{
	graph_list<char, int> glist;

	auto node_a = glist.add_vertex('A');
	auto node_b = glist.add_vertex('B');
	auto node_f = glist.add_vertex('F');
	auto node_z = glist.add_vertex('Z');
	auto node_w = glist.add_vertex('W');
	auto node_v = glist.add_vertex('V');

	glist.add_edge(0, node_a, node_f);
	glist.add_edge(0, node_a, node_z);
	glist.add_edge(0, node_a, node_w);
	glist.add_edge(0, node_a, node_v);
	glist.add_edge(0, node_f, node_b);
	glist.add_edge(0, node_f, node_a);
	glist.add_edge(0, node_f, node_w);
	glist.add_edge(0, node_w, node_v);

	//glist.print_graph();
	//glist.print_edges(node_a);
	//glist.print_edges(node_f);
	//glist.print_edges(node_w);
	//glist.print_edges(node_z);

	REQUIRE(glist.size() == 6);
	REQUIRE(glist.edge_size(node_a) == 4);
	REQUIRE(glist.edge_size(node_f) == 3);
	REQUIRE(glist.edge_size(node_v) == 0);

	glist.remove_vertex(node_w);
	REQUIRE(glist.size() == 5);
	REQUIRE(glist.edge_size(node_a) == 3);
	REQUIRE(glist.edge_size(node_f) == 2);

	glist.remove_egde(node_a, node_f);
	glist.remove_egde(node_a, node_z);
	REQUIRE(glist.edge_size(node_a) == 1);

	glist.remove_egde(node_a, node_v);
	REQUIRE(glist.edge_size(node_a) == 0);

	glist.clear();
	REQUIRE(glist.size() == 0);
}