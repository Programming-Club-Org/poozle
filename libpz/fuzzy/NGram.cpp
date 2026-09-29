#include "NGram.hpp"
#include <algorithm>
#include <unordered_set>

std::vector<std::string> generateTrigrams(const std::string &text) {
  std::vector<std::string> trigrams;
  st32 length = static_cast<st32>(text.length());

  if (length < 3) {
    return trigrams;
  }

  trigrams.reserve(length - 2);
  for (st32 i = 0; i <= length - 3; ++i) {
    trigrams.emplace_back(text.substr(i, 3));
  }
  return trigrams;
}

/**
 * @brief Computes containment / overlap similarity for trigrams.
 *
 * Divides intersection by the smaller set size so short queries
 * matching inside longer sentences score close to 1.0.
 */
double trigramJaccardSimilarity(const std::string &text1,
                                const std::string &text2) {
  if (text1.empty() && text2.empty())
    return 1.0;
  if (text1.empty() || text2.empty())
    return 0.0;

  std::vector<std::string> grams1 = generateTrigrams(text1);
  std::vector<std::string> grams2 = generateTrigrams(text2);

  if (grams1.empty() || grams2.empty()) {
    return (text1 == text2) ? 1.0 : 0.0;
  }

  std::unordered_set<std::string> set1(grams1.begin(), grams1.end());
  std::unordered_set<std::string> set2(grams2.begin(), grams2.end());

  // Iterate over the smaller set for faster hash lookups
  const auto &smaller = (set1.size() < set2.size()) ? set1 : set2;
  const auto &larger = (set1.size() < set2.size()) ? set2 : set1;

  st32 intersection_cnt = 0;
  for (const auto &gram : smaller) {
    if (larger.find(gram) != larger.end()) {
      intersection_cnt++;
    }
  }

  // Normalizing by min(|A|, |B|)
  st32 min_size = static_cast<st32>(smaller.size());
  if (min_size == 0)
    return 0.0;

  return static_cast<double>(intersection_cnt) / min_size;
}