#include "word_break.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <sstream>
#include <unordered_map>

auto word_break::read_lexicon(
	const std::string &path
) -> std::unordered_set<std::string> {
	std::ifstream lexicon_file(path);
	if (!lexicon_file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		return {};
	}
	
	std::string word;
	std::unordered_set<std::string> lexicon_set;
	while (std::getline(lexicon_file, word)) {
		if (!word.empty() && (word.back() == '\n' || word.back() == '\r')) {
			word.pop_back();
		}

		if (word.empty()) {
			continue;
		}
		lexicon_set.insert(word);
	}
	
	return lexicon_set;
}

auto get_sentences(
	const std::string& string_to_break,
    const std::unordered_set<std::string>& lexicon
) -> std::vector<std::string> {
	auto n = string_to_break.length();
	std::vector<std::vector<std::string>> dp(n + 1);
	std::unordered_map<size_t, size_t> min_words_at;

	dp[n] = {""};

	for (auto i = n - 1; i-- > 0; ) {
		for (auto j = i + 1; j <= n; j++) {
			auto word = string_to_break.substr(i, j - i);
			if (lexicon.contains(word)) {
				for (auto &sub_sentence : dp[j]) {
					size_t new_word_count = 1 + sub_sentence.size();

					if (sub_sentence.empty()) {
						dp[i].push_back(word);
						min_words_at[i] = new_word_count;
					} else if (!min_words_at.contains(i) || new_word_count < min_words_at[i]) {
						min_words_at[i] = new_word_count;
						dp[i].push_back(word + " " + sub_sentence);
					}
				}
			}
		}
	}

	return dp[0];
}

auto get_min_spaces (
	const std::vector<std::string>& all_sentences
) -> int {
	int min_space = std::numeric_limits<int>::max();
	for (auto sentence : all_sentences) {
		auto space_count = static_cast<int>(std::count(sentence.begin(), sentence.end(), ' '));
		if (space_count < min_space)
			min_space = space_count;
	}

	return min_space;
}

auto split_by_spaces (
	const std::vector<std::string>& all_sentences
) -> std::vector<std::vector<std::string>> {
	std::vector<std::vector<std::string>> min_sentences;

	for (auto sentence : all_sentences) {
		std::vector<std::string> split_sentence;
		std::stringstream ss(sentence);
		std::string word;

		while (ss >> word) {
			split_sentence.push_back(word);
		}

		min_sentences.push_back(split_sentence);
	}

	return min_sentences;
}

auto word_break::word_break(
    const std::string& string_to_break,
    const std::unordered_set<std::string>& lexicon
) -> std::vector<std::vector<std::string>> {
	if (string_to_break.empty()) {
		return {};
	}

	// Gets all possible sentences in the string, ignores sentences that have "composite" words
	auto all_sentences = get_sentences(string_to_break, lexicon);

	if (all_sentences.empty()) {
		return {};
	}

	// Counts the spaces; Counts the minimal words of the smallest sentences
	auto min_space = get_min_spaces(all_sentences);

	// Filters collection to only have those that have the minimal amount of words
	all_sentences.erase(
		std::remove_if(all_sentences.begin(), all_sentences.end(),
			[min_space](const std::string& sentence) {
				return std::count(sentence.begin(), sentence.end(), ' ') != min_space;
			}),
		all_sentences.end()
	);

	// Splits and packs into a the result
	return split_by_spaces(all_sentences);
}
