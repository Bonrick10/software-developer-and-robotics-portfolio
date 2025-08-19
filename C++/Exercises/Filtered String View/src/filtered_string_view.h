#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <compare>
#include <functional>
#include <iterator>
#include <optional>
#include <string>

namespace fsv {
    using filter = std::function<bool(const char&)>;

    class filtered_string_view {
        class iter {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = char;
            using reference = char&;
            using pointer = void;
            using difference_type = std::ptrdiff_t;

            iter() = default;
            iter(const char* str, std::size_t end, filter predicate, bool is_start);

            auto operator*() const -> reference;
            auto operator->() const -> pointer;

            auto operator++() -> iter&;
            auto operator++(int) -> iter {
                auto copy{*this};
                ++(*this);
                return copy;
            };
            auto operator--() -> iter&;
            auto operator--(int) -> iter {
                auto copy{*this};
                --(*this);
                return copy;
            };

            friend auto operator==(const iter& lhs, const iter& rhs) -> bool {
                return (lhs.str_ == rhs.str_ && lhs.pos_ == rhs.pos_);
            };
            friend auto operator!=(const iter& lhs, const iter& rhs) -> bool {
                return !(lhs == rhs);
            };

        private:
            const char* str_;
            std::size_t pos_;
            std::size_t end_;
            filter predicate_;
        };

    public:
        static filter default_predicate;

        using const_iterator = iter;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Constructors
        filtered_string_view();
        filtered_string_view(const std::string& str);
        filtered_string_view(const std::string& str, filter predicate);
        filtered_string_view(const char* str);
        filtered_string_view(const char* str, filter predicate);

        // Copy and Move
        filtered_string_view(const filtered_string_view& other);
        filtered_string_view(filtered_string_view&& other) noexcept;

        // Destructor
        ~filtered_string_view() noexcept = default;

        // Member Operators
        auto operator=(const filtered_string_view& other) -> filtered_string_view&;
        auto operator=(filtered_string_view&& other) noexcept -> filtered_string_view&;
        auto operator[](size_t n) -> const char&;
        explicit operator std::string();

        // Member Functions
        auto at(size_t index) -> const char&;
        auto size() const -> std::size_t;
        auto empty() const -> bool;
        auto data() const -> const char*;
        auto predicate() const -> const filter&;

        // Non-Member Operators
        friend auto operator==(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool;
        friend auto operator>(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool;
        friend auto operator<(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool;
        friend auto operator>=(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool {
            return !(lhs < rhs);
        };
        friend auto operator<=(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool {
            return !(lhs > rhs);
        };
        friend auto operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream&;

        // Iterator Functions
        auto begin() const -> const_iterator {
            return cbegin();
        };
        auto end() const -> const_iterator {
            return cend();
        };
        auto cbegin() const -> const_iterator;
        auto cend() const -> const_iterator;

        auto rbegin() const -> const_reverse_iterator {
            return crbegin();
        };
        auto rend() const -> const_reverse_iterator {
            return crend();
        };
        auto crbegin() const -> const_reverse_iterator {
            return const_reverse_iterator{cend()};
        };
        auto crend() const -> const_reverse_iterator {
            return const_reverse_iterator{cbegin()};
        };

    private:
        const char* pointer_;
        std::size_t length_;
        filter predicate_;
    };

    // Non-Member Utility Functions
    auto compose(const filtered_string_view& fsv, const std::vector<filter>& filts) -> filtered_string_view;
    auto split(const filtered_string_view& fsv, const filtered_string_view& tok) -> std::vector<filtered_string_view>;
    auto substr(const filtered_string_view& fsv,
                size_t pos = 0,
                std::optional<size_t> count = std::nullopt) -> filtered_string_view;
} // namespace fsv

#endif // COMP6771_ASS2_FSV_H
