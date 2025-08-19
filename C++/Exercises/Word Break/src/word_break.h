#ifndef COMP6771_WORD_BREAK_H
#define COMP6771_WORD_BREAK_H

#include <string>
#include <unordered_set>
#include <vector>

namespace word_break {
    // Given a file path to a newline-separated list of words...
    // Loads those words into an unordered set and returns it.
    auto read_lexicon(const std::string &path) -> std::unordered_set<std::string>;

    // Given a string of words that have been concatenated, returns all possible ways
    // of 'breaking' the string into 'sentences' using a minimal number of words. Each
    // 'sentence' is made up of valid words in the provided lexicon. 
    auto word_break(
        const std::string &string_to_break,
        const std::unordered_set<std::string> &lexicon
    ) -> std::vector<std::vector<std::string>>;
} // namespace word_break

#endif // COMP6771_WORD_BREAK_H
