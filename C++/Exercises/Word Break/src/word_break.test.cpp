#include "word_break.h"
#include <algorithm>

#include <catch2/catch.hpp>

// Helper to check if two vectors are equal where order doesn't matter
auto same_sentences(
	std::vector<std::vector<std::string>> expected, 
	std::vector<std::vector<std::string>> result
) -> bool {
	std::sort(expected.begin(), expected.end());
	std::sort(result.begin(), result.end());
	return expected == result;
}

TEST_CASE("word_break::read_lexicon works as expected") {
	auto const english_lexicon = word_break::read_lexicon("./english.txt");
	auto check_set_1 = (english_lexicon.contains("abider"));
	auto check_set_2 = (english_lexicon.contains("profiteering"));
	auto check_set_3 = (english_lexicon.contains("upboiling"));
	auto check_set_4 = (english_lexicon.contains("zyzzyvas"));

	REQUIRE(check_set_1);
	REQUIRE(check_set_2);
	REQUIRE(check_set_3);
	REQUIRE(check_set_4);
}

TEST_CASE("word_break::read_lexicon takes a custom file") {
	auto const english_lexicon = word_break::read_lexicon("./my_english.txt");

	auto check_set_1 = (english_lexicon.contains("abider"));
	auto check_set_2 = (english_lexicon.contains("profiteering"));
	auto check_set_3 = (english_lexicon.contains("upboiling"));
	auto check_set_4 = (english_lexicon.contains("zyzzyvas"));

	REQUIRE_FALSE(check_set_1);
	REQUIRE_FALSE(check_set_2);
	REQUIRE_FALSE(check_set_3);
	REQUIRE_FALSE(check_set_4);

	auto check_set_5 = (english_lexicon.contains("oneword"));

	REQUIRE(check_set_5);
	CHECK(english_lexicon.size() == 1);
}

TEST_CASE("word_break::read_lexicon rejects unknown file") {
	auto const english_lexicon = word_break::read_lexicon("./some_file.txt");

	REQUIRE(english_lexicon.empty() == true);
}

TEST_CASE("word_break::read_lexicon accepts empty file") {
	auto const english_lexicon = word_break::read_lexicon("./empty.txt");

	REQUIRE(english_lexicon.empty() == true);
}

TEST_CASE("at -> it") {
	auto const lexicon = std::unordered_set<std::string>{
		"at",
		"it"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"at", "it"}
	};

	auto const ladders = word_break::word_break("atit", lexicon);

	CHECK(same_sentences(expected, ladders));
}

TEST_CASE("Provided example") {
	auto const lexicon = std::unordered_set<std::string>{
		"dogs",
		"and",
		"rag",
		"on",
		"fly",
		"dog",
		"sand",
		"an",
		"dragonfly",
		"drag",
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"dogs", "an", "dragonfly"}
	};

	auto const ladders = word_break::word_break("dogsandragonfly", lexicon);

	CHECK(same_sentences(expected, ladders));
}

TEST_CASE("word break one word") {
	auto const lexicon = std::unordered_set<std::string>{
		"dogs",
		"and",
		"rag",
		"on",
		"fly",
		"dog",
		"sand",
		"an",
		"dragonfly",
		"drag",
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"dragonfly"}
	};

	auto const ladders = word_break::word_break("dragonfly", lexicon);

	CHECK(same_sentences(expected, ladders));
}

TEST_CASE("word break multiple valid sentences") {
	auto const lexicon = std::unordered_set<std::string>{
		"trees", "tree", "and", "sand", "bulls"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"trees", "and", "bulls"},
		{"tree", "sand", "bulls"},
	};

	auto const ladders = word_break::word_break("treesandbulls", lexicon);

	CHECK(same_sentences(expected, ladders));
}

TEST_CASE("word break empty lexicon") {
	auto const lexicon = std::unordered_set<std::string>{ };

	const auto expected = std::vector<std::vector<std::string>>{ };

	auto const ladders = word_break::word_break("shouldbeempty", lexicon);

	CHECK(same_sentences(expected, ladders));
}

TEST_CASE("word break empty input") {
	auto const lexicon = std::unordered_set<std::string>{
		"trees", "tree", "and", "sand", "bulls"
	};

	const auto expected = std::vector<std::vector<std::string>>{ };

	auto const ladders = word_break::word_break("", lexicon);

	CHECK(same_sentences(expected, ladders));
}
