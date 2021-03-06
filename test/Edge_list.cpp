
#include <graph/Edge_list.hpp>

#include "Graph_tester.hpp"

#include <sstream>

SCENARIO("edge sets behave properly", "[Edge_list]") {
	using G = graph::Edge_list;
	GIVEN("an empty graph") {
		G g;
		REQUIRE(g.order() == 0);
		REQUIRE(g.size() == 0);
		Graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
	}
	GIVEN("a random graph") {
		std::mt19937 r;
		G g;
		const std::size_t M = 20, N = 100;
		for (std::size_t m = 0; m < M; ++m)
			g.insert_vert();
		for (std::size_t n = 0; n < N; ++n) {
			auto s = g.random_vert(r), t = g.random_vert(r);
			g.insert_edge(s, t);
		}
		REQUIRE(g.order() == M);
		REQUIRE(g.size() == N);
		Graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("all the edges are erased") {
			while (g.size())
				gt.erase_edge(gt.random_edge(r));
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
		WHEN("viewed in reverse") {
			assert(g.size()); // sanity check on the test itself
			auto rg = g.reverse_view();
			Graph_tester rgt{rg};
		}
	}
}

SCENARIO("edge lists serialize and deserialize", "[Edge_list]") {
	using G = graph::Edge_list;
	GIVEN("an empty graph") {
		G g;
		WHEN("serializing") {
			std::ostringstream os;
			os << g.dot_format();
		}
		WHEN("deserializing") {
			std::ostringstream os;
			os << g.dot_format();
			std::istringstream is(os.str());
			g.clear();
			is >> g.dot_format();
			REQUIRE(g.order() == 0);
			REQUIRE(g.size() == 0);
		}
		WHEN("deserializing string with ignored attributes") {
			std::istringstream is(R"(digraph g { 1 [a=1]; 1 -> 1 [a="a"]; })");
			is >> g.dot_format();
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 1);
		}
		WHEN("deserializing string missing 'digraph'") {
			std::istringstream is("g { }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing graph name") {
			std::istringstream is("digraph { }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing '{'") {
			std::istringstream is("digraph g 1; { }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing ';'") {
			std::istringstream is("digraph g { 1 }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing tail") {
			std::istringstream is("digraph g { -> 1 }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing head") {
			std::istringstream is("digraph g { 1 -> }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing '->'") {
			std::istringstream is("digraph g { 1 1; }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing ','") {
			std::istringstream is("digraph g { 1 [a=1 b=2]; }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing ']'") {
			std::istringstream is("digraph g { 1 [; }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		WHEN("deserializing string missing ']' with attribute") {
			std::istringstream is("digraph g { 1 [a=1; }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
		// WHEN("deserializing string missing start '\"'") {
		// 	std::istringstream is("digraph g { 1 [a=1\"]; }");
		// 	try {
		// 		is >> g.dot_format();
		// 		REQUIRE(false);
		// 	} catch (const graph::format_error&) {}
		// }
		WHEN("deserializing string missing end '\"'") {
			std::istringstream is("digraph g { 1 [a=\"1]; }");
			try {
				is >> g.dot_format();
				REQUIRE(false);
			} catch (const graph::format_error&) {}
		}
	}
	GIVEN("a random graph") {
		std::mt19937 r;
		G g;
		const std::size_t M = 20, N = 100;
		for (std::size_t m = 0; m < M; ++m)
			g.insert_vert();
		for (std::size_t n = 0; n < N; ++n) {
			auto s = g.random_vert(r), t = g.random_vert(r);
			g.insert_edge(s, t);
		}
		WHEN("serializing") {
			std::ostringstream os;
			os << g.dot_format();
		}
		WHEN("deserializing") {
			std::ostringstream os;
			os << g.dot_format();
			std::istringstream is(os.str());
			g.clear();
			is >> g.dot_format();
			REQUIRE(g.order() == M);
			REQUIRE(g.size() == N);
		}
	}
	GIVEN("a graph with integer attributes") {
		using namespace graph::attributes;
		G g;
		auto v = g.insert_vert();
		g.insert_edge(v, v);
		auto v1 = [](auto v) { return 1; };
		auto v2 = [](auto v) { return 1; };
		auto e1 = [](auto v) { return 1; };
		auto e2 = [](auto v) { return 1; };
		WHEN("serializing") {
			std::ostringstream os;
			os << g.dot_format(
				"v1"_of_vert = v1, "v2"_of_vert = v2,
				"e1"_of_edge = e1, "e2"_of_edge = e2);
		}
		WHEN("deserializing") {
			std::ostringstream os;
			os << g.dot_format(
				"v1"_of_vert = v1, "v2"_of_vert = v2,
				"e1"_of_edge = e1, "e2"_of_edge = e2);
			std::istringstream is(os.str());
			g.clear();
			auto _v1 = g.vert_map(0), _v2 = g.vert_map(0);
			auto _e1 = g.edge_map(0), _e2 = g.edge_map(0);
			is >> g.dot_format(
				"v1"_of_vert = _v1, "v2"_of_vert = _v2,
				"e1"_of_edge = _e1, "e2"_of_edge = _e2);
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 1);
			for (auto v : g.verts()) {
				REQUIRE(_v1(v) == v1(v));
				REQUIRE(_v2(v) == v2(v));
			}
			for (auto e : g.edges()) {
				REQUIRE(_e1(e) == e1(e));
				REQUIRE(_e2(e) == e2(e));
			}
		}
	}
	GIVEN("a graph with string attributes") {
		using namespace graph::attributes;
		G g;
		auto v = g.insert_vert();
		g.insert_edge(v, v);
		auto v1 = [](auto v) { return R"(")"; };
		auto v2 = [](auto v) { return R"(\)"; };
		auto e1 = [](auto v) { return R"(")"; };
		auto e2 = [](auto v) { return R"(\)"; };
		WHEN("serializing") {
			std::ostringstream os;
			os << g.dot_format(
				"v1"_of_vert = v1, "v2"_of_vert = v2,
				"e1"_of_edge = e1, "e2"_of_edge = e2);
		}
		WHEN("deserializing") {
			std::ostringstream os;
			os << g.dot_format(
				"v1"_of_vert = v1, "v2"_of_vert = v2,
				"e1"_of_edge = e1, "e2"_of_edge = e2);
			std::istringstream is(os.str());
			g.clear();
			auto _v1 = g.vert_map<std::string>(), _v2 = g.vert_map<std::string>();
			auto _e1 = g.edge_map<std::string>(), _e2 = g.edge_map<std::string>();
			is >> g.dot_format(
				"v1"_of_vert = _v1, "v2"_of_vert = _v2,
				"e1"_of_edge = _e1, "e2"_of_edge = _e2);
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 1);
			for (auto v : g.verts()) {
				REQUIRE(_v1(v) == v1(v));
				REQUIRE(_v2(v) == v2(v));
			}
			for (auto e : g.edges()) {
				REQUIRE(_e1(e) == e1(e));
				REQUIRE(_e2(e) == e2(e));
			}
		}
	}
}

#ifdef GRAPH_BENCHMARK
TEST_CASE("edge list", "[benchmark]") {
	using G = graph::Edge_list;
	static const std::size_t order = 1000;
	static const std::size_t size = 1000;
	std::mt19937 r;
	BENCHMARK("insert vertices") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		REQUIRE(g.order() == order);
	}
	BENCHMARK("insert self-edges") {
		G g;
		auto v = g.insert_vert();
		for (std::size_t i = 0; i < size; ++i)
			g.insert_edge(v, v);
		REQUIRE(g.size() == size);
	}
	BENCHMARK("insert random edges") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		for (std::size_t i = 0; i < size; ++i) {
			auto u = g.random_vert(r), v = g.random_vert(r);
			g.insert_edge(u, v);
		}
		REQUIRE(g.order() == order);
		REQUIRE(g.size() == size);
	}
	BENCHMARK("erase vertices") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		for (std::size_t i = 0; i < order; ++i)
			g.erase_vert(g.random_vert(r));
		REQUIRE(g.order() == 0);
	}
	BENCHMARK("erase edges") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		for (std::size_t i = 0; i < size; ++i) {
			auto u = g.random_vert(r), v = g.random_vert(r);
			g.insert_edge(u, v);
		}
		for (std::size_t i = 0; i < size; ++i)
			g.erase_edge(g.random_edge(r));
		REQUIRE(g.size() == 0);
	}
}
#endif
