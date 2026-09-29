#ifndef LEVENSHTEIN_DP_HPP
#define LEVENSHTEIN_DP_HPP

#include "pz_types.hpp"
#include <string>

/**
 * @brief Calculates the Levenshtein distance between two strings using Dynamic
 * Programming.
 *
 * @param sourceString The first string to compare (often the pattern).
 * @param targetString The second string to compare against (often the text
 * being searched).
 * @return st32 The minimum number of single-character edits (insertions,
 * deletions, substitutions) required to change sourceText into targetText.
 */
st32 levenshteinDistance(const std::string &sourceString,
                         const std::string &targetString);

#endif // LEVENSHTEIN_DP_HPP