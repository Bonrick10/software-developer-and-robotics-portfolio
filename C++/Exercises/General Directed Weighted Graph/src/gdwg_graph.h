#ifndef GDWG_GRAPH_H
#define GDWG_GRAPH_H

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace gdwg {
	template<typename N, typename E>
	class Graph;

	template<typename N, typename E>
	class Edge {
	 public:
		Edge(N const& src, N const& dst, std::optional<E> weight)
		: src_{src}
		, dst_{dst}
		, weight_{weight} {};

		virtual ~Edge() = default;

		virtual auto print_edge() const noexcept -> std::string = 0;
		virtual auto is_weighted() const noexcept -> bool = 0;
		virtual auto get_weight() const -> std::optional<E> {
			return weight_;
		};
		virtual auto get_nodes() const noexcept -> std::pair<N, N> {
			return std::pair<N, N>{src_, dst_};
		};
		virtual auto operator==(Edge<N, E> const& other) const -> bool = 0;

	 private:
		N src_;
		N dst_;
		std::optional<E> weight_;
		friend class Graph<N, E>;
	};

	template<typename N, typename E>
	class WeightedEdge : public Edge<N, E> {
	 public:
		WeightedEdge(N const& src, N const& dst, E const& weight)
		: Edge<N, E>(src, dst, std::make_optional(weight)){};

		auto print_edge() const noexcept -> std::string override {
			auto nodes = this->get_nodes();
			auto weight = this->get_weight();
			auto str = std::string{std::format("{} -> {} | W | {}", nodes.first, nodes.second, weight.value())};
			return str;
		};
		auto is_weighted() const noexcept -> bool override {
			return true;
		};
		auto operator==(Edge<N, E> const& other) const -> bool override {
			if (!other.is_weighted()) {
				return false;
			}

			return this->get_nodes() == other.get_nodes() && this->get_weight() == other.get_weight();
		};

	 private:
	};

	template<typename N, typename E>
	class UnweightedEdge : public Edge<N, E> {
	 public:
		UnweightedEdge(N const& src, N const& dst)
		: Edge<N, E>(src, dst, std::nullopt){};

		auto print_edge() const noexcept -> std::string override {
			auto nodes = this->get_nodes();
			auto str = std::string{std::format("{} -> {} | U", nodes.first, nodes.second)};
			return str;
		};
		auto is_weighted() const noexcept -> bool override {
			return false;
		};
		auto operator==(Edge<N, E> const& other) const -> bool override {
			if (other.is_weighted()) {
				return false;
			}

			return this->get_nodes() == other.get_nodes();
		};

	 private:
	};

	template<typename N, typename E>
	class Graph {
		class iterator {
		 public:
			using value_type = struct {
				N from;
				N to;
				std::optional<E> weight;
			};
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			using IterTuple = std::tuple<N, N, std::optional<E>>;
			// Iterator constructor
			iterator() = default;

			static auto init(std::set<IterTuple>::iterator curr) -> iterator {
				return iterator(curr);
			}

			// Iterator source
			auto operator*() const -> reference {
				auto data = *curr_;
				return {std::get<0>(data), std::get<1>(data), std::get<2>(data)};
			};

			// Iterator traversal
			auto operator++() -> iterator& {
				++curr_;
				return *this;
			};
			auto operator++(int) -> iterator {
				auto copy = *this;
				++(*this);
				return copy;
			};
			auto operator--() -> iterator& {
				--curr_;
				return *this;
			};
			auto operator--(int) -> iterator {
				auto copy = *this;
				--(*this);
				return copy;
			};

			// Iterator comparison
			auto operator==(iterator const& other) const -> bool {
				return curr_ == other.curr_;
			};

		 private:
			explicit iterator(std::set<IterTuple>::iterator curr)
			: curr_(curr){};

			std::set<IterTuple>::iterator curr_;
		};

	 public:
		Graph()
		: nodes_{}
		, edges_{} {};

		Graph(std::initializer_list<N> il)
		: nodes_{}
		, edges_{} {
			for (auto const& value : il) {
				insert_node(value);
			}
		};

		template<typename InputIt>
		Graph(InputIt first, InputIt last) {
			for (auto it = first; it != last; ++it) {
				insert_node(*it);
			}
		}

		Graph(Graph&& other) noexcept
		: nodes_(std::move(other.nodes_))
		, edges_(std::move(other.edges_)) {
			other.clear();
		};

		auto operator=(Graph&& other) noexcept -> Graph& {
			if (this != &other) {
				nodes_ = std::move(other.nodes_);
				edges_ = std::move(other.edges_);

				other.clear();
			}

			return *this;
		};

		Graph(Graph const& other)
		: nodes_(other.nodes_)
		, edges_{other.edges_} {};

		auto operator=(Graph const& other) -> Graph& {
			if (this != &other) {
				nodes_ = other.nodes_;
				edges_ = other.edges_;
			}

			return *this;
		};

		~Graph() = default;

		// Modifiers
		auto insert_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}

			nodes_.insert(value);
			return true;
		};
		auto insert_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::insert_edge when either src or dst node does "
				                         "not exist");
			}

			auto new_edge = EdgeTuple{src, dst, weight};
			if (std::find(edges_.begin(), edges_.end(), new_edge) != edges_.end()) {
				return false;
			}

			edges_.insert(new_edge);
			return true;
		};
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::replace_node on a node that doesn't exist");
			}

			if (is_node(new_data)) {
				return false;
			}

			insert_node(new_data);
			auto replace_edges = std::vector<EdgeTuple>{};
			for (auto it = edges_.begin(); it != edges_.end();) {
				auto [from, to, weight] = *it;

				if (from == old_data || to == old_data) {
					auto new_from = (from == old_data) ? new_data : from;
					auto new_to = (to == old_data) ? new_data : to;

					replace_edges.emplace_back(new_from, new_to, weight);

					it = edges_.erase(it);
				}
				else {
					++it;
				}
			}

			for (auto&& new_edge : replace_edges) {
				edges_.insert(std::move(new_edge));
			}

			erase_node(old_data);
			return true;
		};

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::merge_replace_node on old or new data if they "
				                         "don't exist in the graph");
			}

			auto replace_edges = std::vector<EdgeTuple>{};
			for (auto it = edges_.begin(); it != edges_.end();) {
				auto [from, to, weight] = *it;

				if (from == old_data || to == old_data) {
					auto new_from = (from == old_data) ? new_data : from;
					auto new_to = (to == old_data) ? new_data : to;

					replace_edges.emplace_back(new_from, new_to, weight);

					it = edges_.erase(it);
				}
				else {
					++it;
				}
			}

			for (auto&& new_edge : replace_edges) {
				edges_.insert(std::move(new_edge));
			}

			erase_node(old_data);
			return;
		};

		auto erase_node(N const& value) -> bool {
			if (!is_node(value)) {
				return false;
			}

			nodes_.erase(value);
			return true;
		};

		auto erase_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::erase_edge on src or dst if they don't exist "
				                         "in the graph");
			}

			auto find_edge = EdgeTuple{src, dst, weight};
			auto it = std::find(edges_.begin(), edges_.end(), find_edge);

			if (it == edges_.end()) {
				return false;
			}

			edges_.erase(it);
			return true;
		};

		auto erase_edge(iterator i) -> iterator {
			auto next = std::next(i);
			auto data = *i;
			erase_edge(data.from, data.to, data.weight);

			return next;
		};

		auto erase_edge(iterator i, iterator s) -> iterator {
			for (auto it = i; it != s;) {
				it = erase_edge(it);
			}

			return s;
		};

		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
			return;
		};

		// Accessors
		[[nodiscard]] auto is_node(N const& value) -> bool {
			return (std::find(nodes_.begin(), nodes_.end(), value) != nodes_.end());
		};
		[[nodiscard]] auto empty() -> bool {
			return nodes_.size() == 0;
		};
		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::is_connected if src or dst node don't exist "
				                         "in the graph");
			}
			return std::any_of(edges_.begin(), edges_.end(), [&](auto const& edge) {
				return std::get<0>(edge) == src && std::get<1>(edge) == dst;
			});
		};
		[[nodiscard]] auto nodes() -> std::vector<N> {
			auto v = std::vector<N>{nodes_.begin(), nodes_.end()};
			return v;
		};
		[[nodiscard]] auto edges(N const& src, N const& dst) -> std::vector<std::unique_ptr<Edge<N, E>>> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::edges if src or dst node don't exist in the "
				                         "graph");
			}

			auto vec_edge = std::vector<std::unique_ptr<Edge<N, E>>>{};
			for (auto const& [s, d, w] : edges_) {
				if (s == src && d == dst) {
					if (w == std::nullopt) {
						vec_edge.push_back(std::make_unique<UnweightedEdge<N, E>>(s, d));
					}
					else {
						vec_edge.push_back(std::make_unique<WeightedEdge<N, E>>(s, d, w.value()));
					}
				}
			}

			return vec_edge;
		};
		[[nodiscard]] auto find(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> iterator {
			for (auto it = begin(); it != end(); ++it) {
				auto edge = *it;
				if (edge.from == src && edge.to == dst && edge.weight == weight) {
					return it;
				}
			}

			return end();
		};
		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::Graph<N, E>::connections if src doesn't exist in the "
				                         "graph");
			}

			auto node_set = std::set<N>{};
			for (auto const& [s, d, _] : edges_) {
				if (s == src) {
					node_set.insert(d);
				}
			}
			auto v = std::vector<N>{node_set.begin(), node_set.end()};
			return v;
		};

		[[nodiscard]] auto begin() const -> iterator {
			return iterator::init(edges_.begin());
		};
		[[nodiscard]] auto end() const -> iterator {
			return iterator::init(edges_.end());
		};

		[[nodiscard]] auto operator==(Graph const& other) const -> bool {
			return nodes_ == other.nodes_ && edges_ == other.edges_;
		};

		template<typename NodeType, typename EdgeType>
		friend auto operator<<(std::ostream& os, Graph<NodeType, EdgeType> const& g) -> std::ostream&;

	 private:
		using EdgeTuple = std::tuple<N, N, std::optional<E>>;
		std::set<N> nodes_;
		std::set<EdgeTuple> edges_;
	};

	template<typename N, typename E>
	auto operator<<(std::ostream& os, Graph<N, E> const& g) -> std::ostream& {
		auto& g_mutable = const_cast<Graph<N, E>&>(g);

		auto node_set = g_mutable.nodes();
		for (const auto& src : node_set) {
			os << src << " (" << std::endl;
			auto connect_set = g_mutable.connections(src);
			for (const auto& dst : connect_set) {
				auto edge_set = g_mutable.edges(src, dst);
				for (const auto& edge_ptr : edge_set) {
					const auto& edge = *edge_ptr;
					os << "  " << edge.print_edge() << std::endl;
				}
			}
			os << ')' << std::endl;
		}

		return os;
	}
} // namespace gdwg

#endif // GDWG_GRAPH_H
