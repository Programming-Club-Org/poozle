#include "LevenshteinDp.hpp"
#include "NGram.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Computes the Levenshtein distance using a DP approach,
 *        filtered by a preliminary trigram overlap threshold.
 *
 * @param sourceString The source string.
 * @param targetString The target string.
 * @return st32 The edit distance between the two strings, or -1 if filtered
 * out.
 */
st32 levenshteinDistance(const std::string &sourceString,
                         const std::string &targetString) {
  // Edge cases
  if (sourceString.empty())
    return static_cast<st32>(targetString.length());
  if (targetString.empty())
    return static_cast<st32>(sourceString.length());

  // Pre-filter using trigram similarity (score threshold > 70%)
  double ngramScore = trigramJaccardSimilarity(sourceString, targetString);
  if (ngramScore < 0.70) {
    return -1;
  }

  st32 sourceLength = static_cast<st32>(sourceString.length());
  st32 targetLength = static_cast<st32>(targetString.length());

  std::vector<st32> prevRow(targetLength + 1);
  std::vector<st32> curRow(targetLength + 1);

  for (st32 i = 0; i <= targetLength; ++i) {
    prevRow[i] = i;
  }

  for (st32 row = 1; row <= sourceLength; ++row) {
    curRow[0] = row;

    for (st32 col = 1; col <= targetLength; ++col) {
      st32 cost = (sourceString[row - 1] == targetString[col - 1]) ? 0 : 1;
      curRow[col] = std::min({
          curRow[col - 1] + 1,    // Insertion
          prevRow[col] + 1,       // Deletion
          prevRow[col - 1] + cost // Substitution
      });
    }
    std::swap(prevRow, curRow);
  }

  return prevRow[targetLength];
}