#include "LevenshteinDp.hpp"
#include <algorithm>
#include <string>
#include <vector>

/**
 * @brief Computes the Levenshtein distance using a DP approach.
 *
 * @param sourceString The source string.
 * @param targetString The target string.
 * @return st32 The edit distance between the two strings.
 */

st32 levenshteinDistance(const std::string &sourceString,
                         const std::string &targetString) {
  st32 sourceLength = sourceString.length();
  st32 targetLength = targetString.length();

  if (sourceLength == 0)
    return targetLength;
  if (targetLength == 0)
    return sourceLength;

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