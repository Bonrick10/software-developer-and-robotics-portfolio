#include "./filtered_string_view.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>

// Helpers
namespace {
    auto combine_filters(const std::vector<fsv::filter>& filts) -> fsv::filter {
        return ([=](const char& c) {
            for (const auto& filt : filts) {
                if (!filt(c))
                    return false;
            }
            return true;
        });
    }

    auto get_raw_range(const std::string str,
                       const std::size_t start,
                       const std::size_t end,
                       fsv::filter predicate) -> std::pair<std::size_t, std::size_t> {
        std::size_t filt_pos = 0;
        std::size_t raw_start = std::string::npos;
        std::size_t raw_end = std::string::npos;

        for (std::size_t i = 0; i < str.size(); ++i) {
            if (predicate(str[i])) {
                if (filt_pos == start) {
                    raw_start = i;
                }
                if (filt_pos == end) {
                    raw_end = i;
                }
                ++filt_pos;
            }
        }
        return std::pair<std::size_t, std::size_t>{raw_start, raw_end};
    }

    auto get_ranged_pred(const fsv::filtered_string_view& fsv, std::size_t start, std::size_t end) -> fsv::filter {
        auto orig_ptr = fsv.data();
        auto orig_pred = fsv.predicate();

        // Materialised only to find the raw range to 'slice' the string
        // Lifetime of string should just end in this function only
        auto data_str = std::string{orig_ptr};

        auto range_pair = get_raw_range(data_str, start, end, orig_pred);
        return [range_pair, orig_pred, orig_ptr](const char& c) {
            auto offset = std::ptrdiff_t{&c - orig_ptr};
            auto i = static_cast<std::size_t>(offset);
            bool include = i >= range_pair.first && i < range_pair.second && orig_pred(c);
            ++i;
            return include;
        };
    }

    auto is_token(fsv::filtered_string_view::const_iterator it,
                  const fsv::filtered_string_view::const_iterator end,
                  const fsv::filtered_string_view& tok) -> bool {
        for (auto tok_it = tok.begin(); tok_it != tok.end(); ++tok_it) {
            if (it == end) {
                return false;
            }
            if (*it != *tok_it) {
                return false;
            }
            ++it;
        }
        return true;
    }

    auto skip_steps(fsv::filtered_string_view::const_iterator it,
                    const std::size_t steps) -> fsv::filtered_string_view::const_iterator {
        for (auto i = std::size_t{}; i < steps; ++i) {
            ++it;
        }
        return it;
    }
} // namespace

namespace fsv {
    filtered_string_view::iter::iter(const char* str, std::size_t end, filter predicate, bool is_start)
    : str_(str)
    , pos_(0)
    , end_(end)
    , predicate_(predicate) {
        if (end_ == 0) {
            return;
        }

        if (is_start) {
            while (pos_ < end_ && !predicate_(str_[pos_])) {
                ++pos_;
            }
        }
        else {
            pos_ = end_ - 1;
            while (pos_ > 0 && !predicate_(str_[pos_])) {
                --pos_;
            }

            if (pos_ == 0 && !predicate_(str_[pos_])) {
                pos_ = end_;
            }
            else {
                ++pos_;
            }
        }
    }

    auto filtered_string_view::iter::operator*() const -> reference {
        return const_cast<reference>(str_[pos_]);
    }

    auto filtered_string_view::iter::operator->() const -> pointer {
        return;
    }

    auto filtered_string_view::iter::operator++() -> iter& {
        // This indicates we are already at .end() as it is one-past-the-last which should be invalid
        if (!predicate_(str_[pos_]) || str_[pos_] == '\0') {
            throw std::out_of_range("Cannot increment further");
        }

        auto pos_temp = ++pos_;
        while (pos_ < end_ && !predicate_(str_[pos_])) {
            ++pos_;
        }

        if (pos_ >= end_) {
            pos_ = pos_temp;
        }
        return *this;
    }

    auto filtered_string_view::iter::operator--() -> iter& {
        auto pos_temp = pos_;
        --pos_temp;
        while (pos_temp > 0 && !predicate_(str_[pos_temp])) {
            --pos_temp;
        }

        if (pos_temp == 0 && !predicate_(str_[pos_temp])) {
            throw std::out_of_range("Cannot decrement further");
        }

        pos_ = pos_temp;
        return *this;
    }

    filter filtered_string_view::default_predicate = [](const char&) { return true; };

    filtered_string_view::filtered_string_view()
    : pointer_(nullptr)
    , length_(0)
    , predicate_(filtered_string_view::default_predicate) {}

    filtered_string_view::filtered_string_view(const std::string& str)
    : filtered_string_view(str, filtered_string_view::default_predicate) {}

    filtered_string_view::filtered_string_view(const std::string& str, filter predicate)
    : pointer_(&str[0])
    , length_(str.size())
    , predicate_(predicate) {}

    filtered_string_view::filtered_string_view(const char* str)
    : filtered_string_view(str, filtered_string_view::default_predicate) {}

    filtered_string_view::filtered_string_view(const char* str, filter predicate)
    : pointer_(str)
    , length_(std::strlen(str))
    , predicate_(predicate) {}

    filtered_string_view::filtered_string_view(const filtered_string_view& other)
    : pointer_(other.pointer_)
    , length_(other.length_)
    , predicate_(other.predicate_) {}

    filtered_string_view::filtered_string_view(filtered_string_view&& other) noexcept
    : pointer_(std::move(other.pointer_))
    , length_(std::move(other.length_))
    , predicate_(std::move(other.predicate_)) {
        other.pointer_ = nullptr;
        other.length_ = 0;
    }

    auto filtered_string_view::operator=(const filtered_string_view& other) -> filtered_string_view& {
        if (this != &other) {
            pointer_ = other.pointer_, length_ = other.length_, predicate_ = other.predicate_;
        }
        return *this;
    }

    auto filtered_string_view::operator=(filtered_string_view&& other) noexcept -> filtered_string_view& {
        if (this != &other) {
            pointer_ = std::move(other.pointer_);
            length_ = std::move(other.length_);
            predicate_ = std::move(other.predicate_);

            other.pointer_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    auto filtered_string_view::operator[](size_t n) -> const char& {
        auto it_index = std::ranges::next(begin(), static_cast<std::iter_difference_t<decltype(begin())>>(n));
        if (it_index != end()) {
            return *it_index;
        }

        // undefined behaviour
        throw std::domain_error("filtered_string_view::operator[](" + std::to_string(n) + "): invalid index");
    }

    filtered_string_view::operator std::string() {
        auto str = std::string{begin(), end()};
        return str;
    }

    auto filtered_string_view::at(size_t index) -> const char& {
        auto filt_pos = std::size_t(0);
        for (auto it = begin(); it != end(); ++it) {
            if (filt_pos == index) {
                return *it;
            }
            ++filt_pos;
        }

        throw std::domain_error("filtered_string_view::at(" + std::to_string(index) + "): invalid index");
    }

    auto filtered_string_view::size() const -> std::size_t {
        return static_cast<std::size_t>(std::ranges::distance(begin(), end()));
    }

    auto filtered_string_view::empty() const -> bool {
        return (size() == 0);
    }

    auto filtered_string_view::data() const -> const char* {
        return pointer_;
    }

    auto filtered_string_view::predicate() const -> const filter& {
        return predicate_;
    }

    auto operator==(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool {
        return std::ranges::equal(lhs, rhs);
    }

    auto operator>(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool {
        auto l_it = lhs.begin();
        for (auto r_it = rhs.begin(); r_it != rhs.end(); ++r_it) {
            if (l_it == lhs.end()) {
                return false;
            }
            if (*l_it > *r_it) {
                return true;
            }
            else {
                return false;
            }

            ++l_it;
        }

        if (l_it == lhs.end()) {
            return false;
        }
        return true;
    }

    auto operator<(const filtered_string_view& lhs, const filtered_string_view& rhs) -> bool {
        auto l_it = lhs.begin();
        for (auto r_it = rhs.begin(); r_it != rhs.end(); ++r_it) {
            if (l_it == lhs.end()) {
                return true;
            }
            if (*l_it < *r_it) {
                return true;
            }
            else {
                return false;
            }
            ++l_it;
        }

        if (l_it == lhs.end()) {
            return false;
        }
        return true;
    }

    auto operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream& {
        for (auto it = fsv.begin(); it != fsv.end(); ++it) {
            os << *it;
        }
        return os;
    }

    auto compose(const filtered_string_view& fsv, const std::vector<filter>& filts) -> filtered_string_view {
        auto new_pred = combine_filters(filts);
        auto sv = filtered_string_view{fsv.data(), new_pred};
        return sv;
    }

    auto split(const filtered_string_view& fsv, const filtered_string_view& tok) -> std::vector<filtered_string_view> {
        auto tok_size = tok.size();
        auto fsv_end = fsv.end();

        auto sv_vec = std::vector<filtered_string_view>{};

        auto split_start = std::size_t{};
        auto curr_pos = std::size_t{};
        for (auto it = fsv.begin(); it != fsv_end; ++it) {
            if (!tok.empty() && is_token(it, fsv_end, tok)) {
                auto split_sv = substr(fsv, split_start, curr_pos - split_start);
                sv_vec.push_back(split_sv);

                it = skip_steps(it, tok_size - 1);

                curr_pos += tok_size;
                split_start = curr_pos;
            }
            else {
                ++curr_pos;
            }
        }

        if (split_start != fsv.size()) {
            auto split_sv = substr(fsv, split_start, curr_pos - split_start);
            sv_vec.push_back(split_sv);
        }
        else {
            auto ranged_pred = get_ranged_pred(fsv, split_start - 1, split_start - 1);
            auto split_sv = filtered_string_view{fsv.data(), ranged_pred};
            sv_vec.push_back(split_sv);
        }

        return sv_vec;
    }

    auto substr(const filtered_string_view& fsv, size_t pos, std::optional<size_t> count) -> filtered_string_view {
        if (pos >= fsv.size()) {
            throw std::out_of_range{"filtered_string_view::substr(" + std::to_string(pos)
                                    + "): position out of range for filtered string of size "
                                    + std::to_string(fsv.size())};
        }

        auto end = std::size_t{};
        if (count == std::nullopt) {
            end = fsv.size();
        }
        else {
            end = *count + pos;
            if (end > fsv.size()) {
                end = fsv.size();
            }
        }

        if (*count == 0 || pos == fsv.size()) {
            return filtered_string_view{""};
        }

        auto ranged_pred = get_ranged_pred(fsv, pos, end);
        auto sv = filtered_string_view{fsv.data(), ranged_pred};
        return sv;
    }

    auto filtered_string_view::cbegin() const -> const_iterator {
        auto const_iter = const_iterator{pointer_, length_, predicate_, true};
        return const_iter;
    }
    auto filtered_string_view::cend() const -> const_iterator {
        auto const_iter = const_iterator{pointer_, length_, predicate_, false};
        return const_iter;
    }
} // namespace fsv