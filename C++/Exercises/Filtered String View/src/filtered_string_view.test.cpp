#include "./filtered_string_view.h"

#include <catch2/catch.hpp>
#include <iostream>
#include <set>

TEST_CASE("Testing constructors") {
    SECTION("Default") {
        auto sv = fsv::filtered_string_view{};
        REQUIRE(sv.size() == 0);
    }

    SECTION("Implicit string") {
        auto s = std::string{"dog"};
        auto sv = fsv::filtered_string_view{s};
        REQUIRE(sv.size() == 3);
    }

    SECTION("String with predicate") {
        auto s = std::string{"dog"};
        auto pred = [](const char& c) { return c == 'o'; };
        auto sv = fsv::filtered_string_view{s, pred};
        REQUIRE(sv.size() == 1);
    }

    SECTION("Implicit null-terminated string") {
        auto sv = fsv::filtered_string_view{"dog"};
        REQUIRE(sv.size() == 3);
    }

    SECTION("Implicit null-terminated string with predicate") {
        auto pred = [](const char& c) { return c == 'o'; };
        auto sv = fsv::filtered_string_view{"dog", pred};
        REQUIRE(sv.size() == 1);
    }
}

TEST_CASE("Testing copy and move constructors") {
    SECTION("Copy") {
        auto sv1 = fsv::filtered_string_view{"yippie"};
        auto copy = sv1;

        REQUIRE(copy.data() == sv1.data());
        CHECK(copy == sv1);
    }

    SECTION("Move") {
        auto sv1 = fsv::filtered_string_view{"yippie"};
        const auto move = std::move(sv1);

        REQUIRE(sv1.data() == nullptr);
        CHECK_FALSE(move.data() == nullptr);
        CHECK(move.size() == 6);
    }
}

TEST_CASE("Testing member operators") {
    SECTION("Copy assignment") {
        auto sv1 = fsv::filtered_string_view{"yippie"};
        auto sv2 = fsv::filtered_string_view{};

        sv2 = sv1;

        REQUIRE(sv2.data() == sv1.data());
        CHECK(sv2 == sv1);
    }

    SECTION("Move assigment") {
        auto sv1 = fsv::filtered_string_view{"yippie"};
        auto sv2 = fsv::filtered_string_view{};

        sv2 = std::move(sv1);

        REQUIRE(sv1.data() == nullptr);
        CHECK_FALSE(sv2.data() == nullptr);
        CHECK(sv2.size() == 6);
    }

    SECTION("Subscript") {
        auto pred = [](const char& c) { return c == 'b' || c == 'o' || c == 'h'; };
        auto sv = fsv::filtered_string_view{"brochacho", pred};

        CHECK(sv[2] == 'h');
        CHECK(sv[3] == 'h');
        CHECK(sv[4] == 'o');
    }

    SECTION("String type conversion") {
        auto pred = [](const char& c) { return c == 'b' || c == 'o' || c == 'h'; };
        auto sv = fsv::filtered_string_view{"brochacho", pred};
        auto s = static_cast<std::string>(sv);
        REQUIRE_FALSE(sv.data() == s.data());
        CHECK(s == "bohho");
    }
}

TEST_CASE("Testing member functions") {
    SECTION(".at() valid index") {
        auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
        auto is_vowel = [&vowels](const char& c) { return vowels.contains(c); };
        auto sv = fsv::filtered_string_view{"raaAAaH", is_vowel};

        CHECK(sv.at(0) == 'a');
        CHECK(sv.at(3) == 'A');
    }

    SECTION(".at() invalid index") {
        auto pred = [](const char& c) { return c == 'o'; };
        auto sv = fsv::filtered_string_view{"lol", pred};

        REQUIRE_THROWS_MATCHES(sv.at(2),
                               std::domain_error,
                               Catch::Matchers::Message("filtered_string_view::at(2): invalid index"));
    }

    SECTION(".size() without predicate") {
        auto sv = fsv::filtered_string_view{"yippie"};
        CHECK(sv.size() == 6);
    }

    SECTION(".size() with predicate") {
        auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
        auto is_vowel = [&vowels](const char& c) { return vowels.contains(c); };
        auto sv = fsv::filtered_string_view{"yippie", is_vowel};

        CHECK(sv.size() == 3);
    }

    SECTION(".empty() no predicate") {
        auto sv1 = fsv::filtered_string_view{};
        auto sv2 = fsv::filtered_string_view{""};
        auto sv3 = fsv::filtered_string_view{"nAHHH"};

        CHECK(sv1.empty());
        CHECK(sv2.empty());
        CHECK_FALSE(sv3.empty());
    }

    SECTION(".empty() with predicate") {
        auto pred = [](const char& c) { return c == 'o'; };
        auto sv = fsv::filtered_string_view{"nAHHH", pred};

        CHECK(sv.empty());
    }

    SECTION(".data()") {
        auto s = "nAHHH";
        auto pred = [](const char&) { return false; };
        auto sv = fsv::filtered_string_view{s, pred};

        auto ptr = sv.data();
        CHECK(*ptr == 'n');
    }

    SECTION(".predicate()") {
        auto pred1 = [](const char&) { return false; };
        auto pred2 = [](const char& c) {
            std::cout << "This should run!";
            return c == 'h';
        };
        auto sv1 = fsv::filtered_string_view{"false?", pred1};
        auto sv2 = fsv::filtered_string_view{"hah", pred2};

        auto test_pred1 = sv1.predicate();
        auto test_pred2 = sv2.predicate();

        REQUIRE_FALSE(test_pred1('c'));
        REQUIRE_FALSE(test_pred1('?'));

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        REQUIRE(test_pred2('h'));

        std::cout.rdbuf(old);
        CHECK(buffer.str() == "This should run!");

        REQUIRE_FALSE(test_pred2('a'));
        REQUIRE_FALSE(test_pred2('H'));
    }
}

TEST_CASE("Testing non-member operators") {
    SECTION("Equality comparison") {
        auto pred1 = [](const char&) { return false; };
        auto pred2 = [](const char& c) { return c == 'y' || c == 'e'; };
        auto sv1 = fsv::filtered_string_view{"yippie", pred2};
        auto sv2 = fsv::filtered_string_view{"ye"};
        auto sv3 = fsv::filtered_string_view{"yippie", pred1};
        auto sv4 = fsv::filtered_string_view{""};

        REQUIRE(sv1 == sv2);
        CHECK(sv3 == sv4);
        REQUIRE(sv1 != sv3);
        CHECK(sv2 != sv4);
    }

    SECTION("Relational comparison") {
        auto pred = [](char const& c) { return c != 'a' and c != ' '; };
        auto lo = fsv::filtered_string_view{"horn"};
        auto hi = fsv::filtered_string_view{"a mule", pred};
        auto same = fsv::filtered_string_view{"a horn", pred};

        REQUIRE(lo < hi);
        CHECK(lo <= hi);
        REQUIRE_FALSE(lo >= hi);
        CHECK_FALSE(lo > hi);

        CHECK(lo <= same);
        CHECK(lo >= same);
        CHECK_FALSE(lo > same);
        CHECK_FALSE(lo < same);
    }

    SECTION("Output stream") {
        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        auto pred = [](const char& c) { return c == 'c' || c == '+'; };
        auto sv = fsv::filtered_string_view{"c++ > rust > java > python :)", pred};
        std::cout << sv;

        std::cout.rdbuf(old);
        CHECK(buffer.str() == "c++");
        REQUIRE_THAT(buffer.str(), !Catch::Matchers::Contains("\n"));
    }
}

TEST_CASE("Testing non-member functions") {
    SECTION(".compose()") {
        auto false_pred = [](const char&) { return false; };
        auto best_languages = fsv::filtered_string_view{"c / c++", false_pred};

        auto pred1 = [](const char& c) { return c == '+' || c == '/'; };
        auto pred2 = [](const char& c) { return c > ' '; };
        auto pred3 = [](const char&) { return true; };
        auto vf = std::vector<fsv::filter>{pred1, pred2, pred3};

        auto sv = fsv::compose(best_languages, vf);
        auto expcected_sv = fsv::filtered_string_view{"/++"};

        CHECK(sv == expcected_sv);
        CHECK_FALSE(sv == best_languages);
    }

    SECTION(".split() simple test") {
        auto wentworth = fsv::filtered_string_view{"Malcolm? Bligh? Turnbull", [](const char& c) { return c != '?'; }};
        auto token = fsv::filtered_string_view{" 2015", [](const char& c) { return c == ' '; }};
        auto representative = fsv::split(wentworth, token);

        auto expected = std::vector<fsv::filtered_string_view>{"Malcolm", "Bligh", "Turnbull"};
        CHECK(representative == expected);
    }

    SECTION(".split() token not appearing") {
        auto fsv = fsv::filtered_string_view{"fishing"};
        auto empty_tok = fsv::filtered_string_view{""};
        auto empty_split = fsv::split(fsv, empty_tok);

        auto expected = std::vector<fsv::filtered_string_view>{"fishing"};
        CHECK(empty_split == expected);

        auto fill_tok = fsv::filtered_string_view{"robert"};
        auto fill_split = fsv::split(fsv, fill_tok);

        CHECK(fill_split == expected);
    }

    SECTION(".split() simple no filtering") {
        auto fsv = fsv::filtered_string_view{"baabbaaboo"};
        auto tok = fsv::filtered_string_view{"b"};
        auto split = fsv::split(fsv, tok);

        auto expected = std::vector<fsv::filtered_string_view>{"", "aa", "", "aa", "oo"};
        CHECK(split == expected);
    }

    SECTION(".split() every character") {
        auto fsv = fsv::filtered_string_view{"baabbaaboo", [](const char& c) { return c == 'b'; }};
        auto tok = fsv::filtered_string_view{"b"};
        auto split = fsv::split(fsv, tok);

        auto expected = std::vector<fsv::filtered_string_view>{"", "", "", "", ""};
        CHECK(split == expected);
    }

    SECTION(".substr() all parameters standard") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        auto subsv = fsv::substr(sv, 4, 6);
        auto expected = fsv::filtered_string_view{"s craz"};

        CHECK(subsv == expected);
    }

    SECTION(".substr() pos only valid") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        auto subsv = fsv::substr(sv, 6);
        auto expected = fsv::filtered_string_view{"crazy man"};

        CHECK(subsv == expected);
    }

    SECTION(".substr() pos only invalid") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        REQUIRE_THROWS_MATCHES(fsv::substr(sv, 20),
                               std::out_of_range,
                               Catch::Matchers::Message("filtered_string_view::substr(20): position out of range for "
                                                        "filtered string of size 15"));
    }

    SECTION(".substr() no parameters") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        auto subsv = fsv::substr(sv);

        CHECK(subsv == sv);
    }

    SECTION(".substr() all parameters invalid") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        REQUIRE_THROWS_MATCHES(fsv::substr(sv, 55, 100),
                               std::out_of_range,
                               Catch::Matchers::Message("filtered_string_view::substr(55): position out of range for "
                                                        "filtered string of size 15"));
    }

    SECTION(".substr() all parameters extended count") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        auto subsv = fsv::substr(sv, 2, 60);
        auto expected = fsv::filtered_string_view{"ats crazy man"};

        CHECK(subsv == expected);
    }

    SECTION(".substr() empty valid") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        auto subsv = fsv::substr(sv, 2, 0);
        auto expected = fsv::filtered_string_view{""};

        CHECK(subsv == expected);
    }

    SECTION(".substr() empty invalid") {
        auto sv = fsv::filtered_string_view{"thats crazy man"};
        REQUIRE_THROWS_MATCHES(fsv::substr(sv, 55, 0),
                               std::out_of_range,
                               Catch::Matchers::Message("filtered_string_view::substr(55): position out of range for "
                                                        "filtered string of size 15"));
    }

    SECTION(".substr() with predicate") {
        auto pred = [](const char& c) { return c == 't' || c == 'a'; };
        auto sv = fsv::filtered_string_view{"thats crazy man", pred};
        auto subsv = fsv::substr(sv, 1, 3);
        auto expected = fsv::filtered_string_view{"ata"};

        CHECK(subsv == expected);
    }

    SECTION(".substr() with predicate invalid") {
        auto pred = [](const char& c) { return c == 't' || c == 'a'; };
        auto sv = fsv::filtered_string_view{"thats crazy man", pred};
        REQUIRE_THROWS_MATCHES(fsv::substr(sv, 5, 3),
                               std::out_of_range,
                               Catch::Matchers::Message("filtered_string_view::substr(5): position out of range for "
                                                        "filtered string of size 5"));
    }
}

TEST_CASE("Testing custom iterator") {
    SECTION(".cbegin() no filter") {
        auto sv = fsv::filtered_string_view{"thaw"};

        auto it = sv.cbegin();
        CHECK(*it == 't');
        CHECK(*std::next(it) == 'h');
    }

    SECTION(".cend() no filter") {
        auto sv = fsv::filtered_string_view{"thaw"};

        auto it = sv.cend();
        CHECK(*std::prev(it) == 'w');
        CHECK(*std::prev(it, 2) == 'a');
    }

    SECTION(".cbegin() with predicate") {
        auto pred = [](const char& c) { return c == 't' || c == 'a'; };
        auto sv = fsv::filtered_string_view{"thaw", pred};

        auto it = sv.cbegin();
        CHECK(*it == 't');
        CHECK(*std::next(it) == 'a');
    }

    SECTION(".cend() with predicate") {
        auto pred = [](const char& c) { return c == 't' || c == 'a'; };
        auto sv = fsv::filtered_string_view{"thaw", pred};

        auto it = sv.cend();
        CHECK(*std::prev(it) == 'a');
        CHECK(*std::prev(it, 2) == 't');
    }

    SECTION("iterator empty string") {
        auto sv = fsv::filtered_string_view{""};
        CHECK(sv.cbegin() == sv.cend());
    }

    SECTION("iterator empty filtered string") {
        auto pred = [](const char&) { return false; };
        auto sv = fsv::filtered_string_view{"this will all go away", pred};
        CHECK(sv.cbegin() == sv.cend());
    }
}