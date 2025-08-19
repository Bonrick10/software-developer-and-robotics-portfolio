#include "gdwg_graph.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <catch2/catch.hpp>

TEST_CASE("basic test") {
	auto g = gdwg::Graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}

TEST_CASE("Testing constructors") {
	SECTION("Default") {
		auto g = gdwg::Graph<int, std::string>{};

		CHECK(g.empty());
	}

	SECTION("Initializer List") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g = gdwg::Graph<int, int>(values);

		auto expected = std::vector<int>{1, 50, -20, 43, 999};
		std::sort(expected.begin(), expected.end());

		CHECK(g.nodes() == expected);
	}

	SECTION("N Iterator") {
		auto words = std::list<std::string>{"alpha", "beta", "gamma", "delta"};
		auto it1 = words.begin();
		auto it2 = words.begin();
		std::advance(it2, 2);

		auto g = gdwg::Graph<std::string, int>(it1, it2);

		auto expected = std::vector<std::string>{"alpha", "beta"};
		std::sort(expected.begin(), expected.end());

		CHECK(g.nodes() == expected);
	}

	SECTION("Move method") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);

		auto g2 = std::move(g1);

		CHECK(g1.empty());

		auto expected = std::vector<int>{1, 50, -20, 43, 999};
		std::sort(expected.begin(), expected.end());

		CHECK(g2.nodes() == expected);
	}

	SECTION("Move assign") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		auto g2 = gdwg::Graph<int, int>();
		g2 = std::move(g1);

		CHECK(g1.empty());

		auto expected = std::vector<int>{1, 50, -20, 43, 999};
		std::sort(expected.begin(), expected.end());
		CHECK(g2.nodes() == expected);
	}

	SECTION("Copy method") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		auto g2 = g1;

		auto expected = std::vector<int>{1, 50, -20, 43, 999};
		std::sort(expected.begin(), expected.end());
		CHECK(g1.nodes() == expected);
		CHECK(g2.nodes() == expected);
	}

	SECTION("Copy assign") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		auto g2 = gdwg::Graph<int, int>();

		g2 = g1;
		auto expected = std::vector<int>{1, 50, -20, 43, 999};
		std::sort(expected.begin(), expected.end());
		CHECK(g1.nodes() == expected);
		CHECK(g2.nodes() == expected);
	}
}

TEST_CASE("Testing Weighted Edges") {
	auto w_edge = gdwg::WeightedEdge<int, int>(5, 5, 10);

	SECTION("Print edge") {
		auto edge_str = w_edge.print_edge();
		auto expected = std::string{"5 -> 5 | W | 10"};

		CHECK(edge_str == expected);
	}

	SECTION("Is weighted") {
		CHECK(w_edge.is_weighted());
	}

	SECTION("Get weight") {
		CHECK(w_edge.get_weight() == 10);
	}

	SECTION("Get nodes") {
		auto expected = std::pair<int, int>{5, 5};
		CHECK(w_edge.get_nodes() == expected);
	}

	SECTION("Equality") {
		using EdgeType = gdwg::Edge<int, int>;

		auto w_edge1 = gdwg::WeightedEdge<int, int>(5, 5, 10);
		auto w_edge2 = gdwg::WeightedEdge<int, int>(5, 10, 5);

		EdgeType const& base = w_edge;
		EdgeType const& base1 = w_edge1;
		EdgeType const& base2 = w_edge2;

		CHECK(base == base1);
		CHECK_FALSE(base == base2);
	}
}

TEST_CASE("Testing Unweighted Edges") {
	auto u_edge = gdwg::UnweightedEdge<int, int>(5, 2);

	SECTION("Print edge") {
		auto expected = std::string{"5 -> 2 | U"};
		CHECK(u_edge.print_edge() == expected);
	}

	SECTION("Is weighted") {
		CHECK_FALSE(u_edge.is_weighted());
	}

	SECTION("Get weight") {
		CHECK(u_edge.get_weight() == std::nullopt);
	}

	SECTION("Get nodes") {
		auto expected = std::pair<int, int>{5, 2};
		CHECK(u_edge.get_nodes() == expected);
	}

	SECTION("Equality") {
		using EdgeType = gdwg::Edge<int, int>;

		auto u_edge1 = gdwg::UnweightedEdge<int, int>(5, 2);
		auto u_edge2 = gdwg::UnweightedEdge<int, int>(5, 5);

		EdgeType const& base = u_edge;
		EdgeType const& base1 = u_edge1;
		EdgeType const& base2 = u_edge2;

		CHECK(base == base1);
		CHECK_FALSE(base == base2);
	}
}

TEST_CASE("Testing modifiers") {
	SECTION("Insert node") {
		auto g = gdwg::Graph<int, std::string>{};
		auto n = 5;
		CHECK_FALSE(g.is_node(n));

		CHECK(g.insert_node(n));
		CHECK(g.is_node(n));

		CHECK_FALSE(g.insert_node(n));
	}

	SECTION("Insert edge") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g = gdwg::Graph<int, int>(values);

		CHECK(g.insert_edge(1, -20, 10));
		CHECK_FALSE(g.insert_edge(1, -20, 10));
		CHECK(g.insert_edge(1, -20, 11));
		CHECK(g.insert_edge(50, 50, 11));
		CHECK(g.insert_edge(999, 43));

		CHECK(g.is_connected(1, -20));
		CHECK(g.is_connected(50, 50));
		CHECK(g.is_connected(999, 43));

		REQUIRE_THROWS_MATCHES(g.insert_edge(100, -20, 10),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
		REQUIRE_THROWS_MATCHES(g.insert_edge(1, 100),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
	}

	SECTION("Replace node") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g = gdwg::Graph<int, int>(values);

		CHECK(g.replace_node(1, 2));
		CHECK(g.is_node(2));
		CHECK_FALSE(g.is_node(1));

		CHECK_FALSE(g.replace_node(2, -20));
		CHECK(g.replace_node(2, 1));

		CHECK_FALSE(g.replace_node(43, 43));

		REQUIRE_THROWS_MATCHES(g.replace_node(100, -20),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::replace_node on a node that "
		                                                "doesn't exist"));
		REQUIRE_THROWS_MATCHES(g.replace_node(100, 43),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::replace_node on a node that "
		                                                "doesn't exist"));
	}

	SECTION("Merge replace node provided example 1") {
		auto values = std::initializer_list<std::string>{"A", "B", "C", "D"};
		auto g = gdwg::Graph<std::string, int>(values);

		g.insert_edge("A", "B", 3);
		g.insert_edge("C", "B", 2);
		g.insert_edge("D", "B", 4);

		g.merge_replace_node("B", "A");
		REQUIRE_FALSE(g.is_node("B"));

		auto it1 = g.find("A", "A", 3);
		auto it2 = g.find("C", "A", 2);

		auto const& val1 = *it1;
		CHECK(val1.from == "A");
		CHECK(val1.to == "A");
		CHECK(val1.weight.value() == 3);

		auto const& val2 = *it2;
		CHECK(val2.from == "C");
		CHECK(val2.to == "A");
		CHECK(val2.weight.value() == 2);
	}

	SECTION("Merge replace node provided example 2") {
		auto values = std::initializer_list<std::string>{"A", "B", "C", "D"};
		auto g = gdwg::Graph<std::string, int>(values);

		g.insert_edge("B", "A", 3);
		g.insert_edge("B", "C", 2);
		g.insert_edge("B", "D", 4);

		g.merge_replace_node("B", "A");
		REQUIRE_FALSE(g.is_node("B"));

		auto it1 = g.find("A", "A", 3);
		auto it2 = g.find("A", "C", 2);

		auto const& val1 = *it1;
		CHECK(val1.from == "A");
		CHECK(val1.to == "A");
		CHECK(val1.weight.value() == 3);

		auto const& val2 = *it2;
		CHECK(val2.from == "A");
		CHECK(val2.to == "C");
		CHECK(val2.weight.value() == 2);
	}

	SECTION("Merge replace node cycle") {
		auto values = std::initializer_list<std::string>{"A", "B", "C"};
		auto g = gdwg::Graph<std::string, int>(values);

		g.insert_edge("B", "A", 3);
		g.insert_edge("A", "C", 2);
		g.insert_edge("C", "B", 4);

		g.merge_replace_node("B", "A");
		REQUIRE_FALSE(g.is_node("B"));

		auto it1 = g.find("A", "A", 3);
		auto it2 = g.find("A", "C", 2);
		auto it3 = g.find("C", "A", 4);

		auto const& val1 = *it1;
		CHECK(val1.from == "A");
		CHECK(val1.to == "A");
		CHECK(val1.weight.value() == 3);

		auto const& val2 = *it2;
		CHECK(val2.from == "A");
		CHECK(val2.to == "C");
		CHECK(val2.weight.value() == 2);

		auto const& val3 = *it3;
		CHECK(val3.from == "C");
		CHECK(val3.to == "A");
		CHECK(val3.weight.value() == 4);
	}

	SECTION("Merge replace node duplicates") {
		auto values = std::initializer_list<std::string>{"A", "B", "C"};
		auto g = gdwg::Graph<std::string, int>(values);

		g.insert_edge("B", "C", 2);
		g.insert_edge("B", "C", 4);
		g.insert_edge("A", "C", 2);

		g.merge_replace_node("B", "A");
		REQUIRE_FALSE(g.is_node("B"));

		auto edges = g.edges("A", "C");
		CHECK(edges.size() == 2);
	}

	SECTION("Merge replace node throw") {
		auto values = std::initializer_list<std::string>{"A", "B", "C", "D"};
		auto g = gdwg::Graph<std::string, int>(values);

		g.insert_edge("B", "A", 3);
		g.insert_edge("B", "C", 2);
		g.insert_edge("B", "D", 4);

		REQUIRE_THROWS_MATCHES(g.merge_replace_node("E", "A"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.merge_replace_node("D", "E"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.merge_replace_node("F", "E"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
	}

	SECTION("Erase node") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g = gdwg::Graph<int, int>(values);

		CHECK(g.erase_node(1));
		CHECK_FALSE(g.erase_node(1));

		CHECK_FALSE(g.is_node(1));
	}

	SECTION("Erase edge specific") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g = gdwg::Graph<int, int>(values);

		g.insert_edge(1, 50, 10);
		CHECK(g.erase_edge(1, 50, 10));
		CHECK_FALSE(g.erase_edge(1, 50, 10));

		CHECK_FALSE(g.is_connected(1, 50));

		REQUIRE_THROWS_MATCHES(g.erase_edge(1, 100),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.erase_edge(100, -20),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.erase_edge(-1, 100),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
	}

	using graph = gdwg::Graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 4, 2},
	    {2, 1, 1},
	    {4, 1, std::nullopt},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}

	SECTION("Erase edge iterator") {
		auto it = g.begin();
		auto expected = std::next(it);
		auto next = g.erase_edge(it);

		CHECK(expected == next);
		CHECK_FALSE(g.is_connected(1, 5));

		auto last = --g.end();
		auto end = g.end();
		auto next_last = g.erase_edge(last);

		CHECK(end == next_last);
		CHECK_FALSE(g.is_connected(6, 3));
	}

	SECTION("Erase edge range within") {
		auto start = g.begin();
		auto i = std::next(start, 4);
		auto s = std::next(start, 6);

		auto out_it = g.erase_edge(i, s);

		CHECK(s == out_it);

		CHECK_FALSE(g.is_connected(3, 2));
		CHECK_FALSE(g.is_connected(3, 6));
		CHECK(g.is_connected(4, 1));
	}

	SECTION("Erase edge range to end") {
		auto start = g.begin();
		auto i = std::next(start, 4);
		auto s = g.end();

		auto out_it = g.erase_edge(i, s);
		REQUIRE(out_it == g.end());
		CHECK_FALSE(g.is_connected(3, 2));
		CHECK_FALSE(g.is_connected(3, 6));
		CHECK_FALSE(g.is_connected(6, 3));
	}

	SECTION("Clear") {
		g.clear();
		REQUIRE(g.empty());
	}
}

TEST_CASE("Testing accessors") {
	SECTION("Is node") {
		auto g = gdwg::Graph<int, std::string>{};
		g.insert_node(5);
		CHECK(g.is_node(5));
		CHECK_FALSE(g.is_node(4));
	}

	SECTION("Empty") {
		auto g = gdwg::Graph<int, std::string>{};
		CHECK(g.empty());

		g.insert_node(5);
		CHECK_FALSE(g.empty());
	}

	SECTION("Is connected") {
		auto values = std::initializer_list<std::string>{"A", "B", "C", "D"};
		auto g = gdwg::Graph<std::string, int>(values);
		CHECK_FALSE(g.is_connected("A", "B"));

		g.insert_edge("A", "B");
		CHECK(g.is_connected("A", "B"));

		REQUIRE_THROWS_MATCHES(g.is_connected("E", "A"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.is_connected("A", "E"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.is_connected("E", "F"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
	}

	SECTION("Nodes") {
		auto values = std::initializer_list<int>{5, 3, 2, 1, 4};
		auto g = gdwg::Graph<int, int>(values);
		auto expected = std::vector<int>{1, 2, 3, 4, 5};
		auto incorrect = std::vector<int>{5, 3, 2, 1, 4};

		CHECK(g.nodes() == expected);
		CHECK_FALSE(g.nodes() == incorrect);
	}

	using graph = gdwg::Graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 4, 2},
	    {2, 1, 1},
	    {4, 1, std::nullopt},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}

	SECTION("Edges") {
		auto edges1 = g.edges(4, 1);
		auto edges2 = g.edges(2, 1);
		auto edges3 = g.edges(1, 5);
		auto edges4 = g.edges(4, 6);

		auto expected1 = std::string{"4 -> 1 | U"};
		auto expected2 = std::string{"2 -> 1 | W | 1"};
		auto expected3 = std::string{"1 -> 5 | W | -1"};

		CHECK(edges1.size() == 2);
		CHECK((*(edges1[0])).print_edge() == expected1);

		CHECK(edges2.size() == 1);
		CHECK((*(edges2[0])).print_edge() == expected2);

		CHECK(edges3.size() == 1);
		CHECK((*(edges3[0])).print_edge() == expected3);

		CHECK(edges4.size() == 0);

		REQUIRE_THROWS_MATCHES(g.edges(1, 100),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.edges(100, 4),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.edges(999, 100),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
	}

	SECTION("Find") {
		auto it1 = g.find(4, 1);

		auto const& val1 = *it1;
		CHECK(val1.from == 4);
		CHECK(val1.to == 1);
		CHECK_FALSE(val1.weight.has_value());

		auto it2 = g.find(2, 4, 2);

		auto const& val2 = *it2;
		CHECK(val2.from == 2);
		CHECK(val2.to == 4);
		CHECK(val2.weight.value() == 2);

		auto it3 = g.find(1, 5, 100);
		CHECK(it3 == g.end());

		auto it4 = g.find(1, 5);
		CHECK(it4 == g.end());

		auto it5 = g.find(50, 5);
		CHECK(it5 == g.end());
	}

	SECTION("Connections") {
		auto expected = std::vector<int>{1, 4};

		CHECK(g.connections(2) == expected);
		REQUIRE_THROWS_MATCHES(g.connections(999),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::Graph<N, E>::connections if src doesn't "
		                                                "exist in the graph"));
	}
}

TEST_CASE("Testing Comparator") {
	SECTION("Same inserts") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		g1.insert_edge(1, 999);

		auto g2 = gdwg::Graph<int, int>{};
		g2.insert_node(-20);
		g2.insert_node(43);
		g2.insert_node(1);
		g2.insert_node(999);
		g2.insert_node(50);
		g2.insert_edge(1, 999);

		CHECK(g1 == g2);
	}

	SECTION("Copied") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		g1.insert_edge(1, 999);

		auto g2 = g1;
		CHECK(g1 == g2);
	}

	SECTION("Unequal") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		g1.insert_edge(1, 999);

		auto g2 = gdwg::Graph<int, int>{};
		g2.insert_node(-20);
		g2.insert_node(50);

		CHECK_FALSE(g1 == g2);
	}

	SECTION("Blank") {
		auto values = std::initializer_list<int>{1, 50, -20, 43, 999};
		auto g1 = gdwg::Graph<int, int>(values);
		auto g2 = gdwg::Graph<int, int>{};

		g1.clear();
		CHECK(g1 == g2);
	}
}

TEST_CASE("Testing Extractor") {
	using graph = gdwg::Graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 4, 2},
	    {2, 1, 1},
	    {4, 1, std::nullopt},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}

	g.insert_node(64);

	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(1 (
  1 -> 5 | W | -1
)
2 (
  2 -> 1 | W | 1
  2 -> 4 | U
  2 -> 4 | W | 2
)
3 (
  3 -> 2 | W | 2
  3 -> 6 | W | -8
)
4 (
  4 -> 1 | U
  4 -> 1 | W | -4
  4 -> 5 | W | 3
)
5 (
  5 -> 2 | U
)
6 (
  6 -> 2 | W | 5
  6 -> 3 | W | 10
)
64 (
)
)");
	std::cout << expected_output;
	CHECK(out.str() == expected_output);

	auto blank = gdwg::Graph<int, int>{};

	auto blank_os = std::ostringstream{};
	blank_os << blank;
	auto const blank_output = std::string_view("");
	CHECK(blank_os.str() == blank_output);
}

TEST_CASE("Testing iterator") {
	using graph = gdwg::Graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 4, 2},
	    {2, 1, 1},
	    {4, 1, std::nullopt},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}

	auto it = g.begin();
	SECTION("Begin and Reference") {
		auto const& val = *it;
		CHECK(val.from == 1);
		CHECK(val.to == 5);
		CHECK(val.weight.value() == -1);
	}

	SECTION("Pre and post increment") {
		++it;
		auto const& val1 = *it;
		CHECK(val1.from == 2);
		CHECK(val1.to == 1);
		CHECK(val1.weight.value() == 1);

		auto old = it++;
		auto const& val2 = *old;
		CHECK(val2.from == 2);
		CHECK(val2.to == 1);
		CHECK(val2.weight.value() == 1);

		auto const& val3 = *it;
		CHECK(val3.from == 2);
		CHECK(val3.to == 4);
		CHECK_FALSE(val3.weight.has_value());
	}

	SECTION("Pre and post decrement") {
		auto back = std::next(it, 6);
		--back;
		auto const& val1 = *back;
		CHECK(val1.from == 3);
		CHECK(val1.to == 6);
		CHECK(val1.weight.value() == -8);

		auto old = back--;
		auto const& val2 = *old;
		CHECK(val2.from == 3);
		CHECK(val2.to == 6);
		CHECK(val2.weight.value() == -8);

		auto const& val3 = *back;
		CHECK(val3.from == 3);
		CHECK(val3.to == 2);
		CHECK(val3.weight.value() == 2);
	}

	SECTION("End") {
		auto it = --g.end();
		auto const& val = *it;
		CHECK(val.from == 6);
		CHECK(val.to == 3);
		CHECK(val.weight.value() == 10);
	}

	SECTION("Empty") {
		auto blank = gdwg::Graph<int, int>{};
		CHECK(blank.begin() == blank.end());
	}
}