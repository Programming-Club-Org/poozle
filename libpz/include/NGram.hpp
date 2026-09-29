#ifndef POOZLE_NGRAM_HPP
#define POOZLE_NGRAM_HPP

#include "pz_types.hpp"
#include <string>
#include <unordered_set>
#include <vector>

/**
 * @brief Splits a string into character trigrams (3-grams).
 *
 * @param text The input string to decompose.
 * @return std::vector<std::string> Vector containing extracted trigrams.
 */
std::vector<std::string> generateTrigrams(const std::string &text);

/**
 * @brief Computes the Jaccard similarity coefficient using trigram sets.
 *
 * Formula: |A ∩ B| / |A ∪ B|
 *
 * @param text1 The first string to compare.
 * @param text2 The second string to compare.
 * @return double Jaccard index between 0.0 (disjoint) and 1.0 (identical sets).
 */
double trigramJaccardSimilarity(const std::string &text1,
                                const std::string &text2);

#endif // POOZLE_NGRAM_HPP