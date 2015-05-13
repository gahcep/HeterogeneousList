// main.cpp
#define CATCH_CONFIG_MAIN

#include <vector>
#include <list>

#include "../src/hadt_list.hpp"
#include "../src/hadt_forward_list.hpp"

#include "../modules/catch/single_include/catch.hpp"

using hadt::list;
using hadt::forward_list;

TEST_CASE("List Creation", "[Methods][Construction]")
{
	forward_list<int> slist;

	slist.fill_with({ 1, 3, 5, 7, 11, 13 });
	REQUIRE(slist.size() == 6);

	slist.clear();
	std::vector<int> fill_vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::list<int> fill_list{ 10, 20, 30, 40, 50, 60, 70, 80, 90 };

	slist.fill_with(fill_vector.begin(), fill_vector.end());
	REQUIRE(slist.size() == 9);

	slist.fill_with(fill_list.begin(), fill_list.end());
	REQUIRE(slist.size() == 9);
}

TEST_CASE("List Append / Prepand", "[Methods][Construction]")
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

TEST_CASE("List Emptyness", "[Construction]")
{
	forward_list<int> slist;

	REQUIRE(slist.empty() == true);
	slist.push_front(1);
	REQUIRE(slist.empty() == false);
}

TEST_CASE("push_xxx() vs move_xxx()", "[Methods]")
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

TEST_CASE("push_front & Iterators", "[Methods][Iterators]")
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

TEST_CASE("push_back & Iterators", "[Methods][Iterators]")
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

TEST_CASE("Items Retrieval", "[Methods]")
{
	forward_list<int> slist;

	slist.push_front(1);
	slist.push_front(3);
	slist.push_front(5);
	slist.push_front(7);
	slist.push_front(11);

	// Sequence is: 11 -> 7 -> 5 -> 3 -> 1
	REQUIRE(slist.at(0) == 11);
	REQUIRE(slist.at_front() == 11);
	REQUIRE(slist.at(3) == 3);
	REQUIRE(slist.at(slist.size() - 1) == 1);
	REQUIRE(slist.at_back() == 1);
	REQUIRE_THROWS_AS(slist.at(100), std::out_of_range);
}

TEST_CASE("Different operations on singly-linked list", "[Methods]")
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
	REQUIRE(slist.empty() == true);
}

TEST_CASE("Different operations on doubly-linked list", "[Methods]")
{
	list<int> dlist;

	dlist.push_front(1);
	dlist.push_front(3);
	dlist.push_front(5);
	dlist.push_front(7);
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

TEST_CASE("Removing items on singly-linked list", "[Methods]")
{
	forward_list<int> slist;

	slist.push_front(1);
	slist.push_front(3);
	slist.push_front(5);
	slist.push_front(7);
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

TEST_CASE("Removing items on doubly-linked list", "[Methods]")
{
	forward_list<int> slist;

	slist.push_front(1);
	slist.push_front(3);
	slist.push_front(5);
	slist.push_front(7);
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

TEST_CASE("List Search", "[Search][Iterator]")
{
	forward_list<int> slist;
	slist.fill_with({ 10, 11, 12, 13, 14, 15, 16, 17 });

	auto it = slist.find_first(15);
	REQUIRE(*it == 15);
	
	auto it_end = slist.find_first(150);
	REQUIRE(it_end == slist.end());
}

TEST_CASE("Search M-to-Last", "[Methods][Search]")
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

TEST_CASE("Reversing items on singly-linked list", "[Methods][Reverse]")
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

TEST_CASE("Reversing items on doubly-linked list", "[Methods][Reverse]")
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

TEST_CASE("List Printing", "[Methods][Print]")
{
	forward_list<int> slist;
	list<int> dlist;

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
