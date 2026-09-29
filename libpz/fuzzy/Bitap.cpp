#include "../include/Bitap.hpp"
#include "../include/pz_cxx_std.hpp"
#include "../include/pz_error.hpp"
#include "../include/pz_types.hpp"

/**
 * @brief Searches for multiple occurrences of a pattern in a text using the
 * Bitap algorithm.
 *
 * @param pattern The target pattern string to search for.
 *
 * Time Complexity: bitap O(n * k) and local_dp O(C * (m + k) * m)
 * This is layered fuzzy as we perform expensive dp only on known candidates.
 */

LayeredFuzzyMatcher::LayeredFuzzyMatcher(const std::string &pattern)
    : pattern_(pattern), m_(static_cast<st32>(pattern.length())) {
  if (m_ > 64) {
    PzError::report_error(PzError::PzErrorType::PZ_LONG_PATTERN_ERROR,
                          "Pattern length is greater than 64");
  }
}

std::vector<LayeredFuzzyMatcher::Match>
LayeredFuzzyMatcher::search(const std::string &text, st32 max_errors) const {
  std::vector<Match> results;
  if (m_ == 0 || text.empty())
    return results;

  if (max_errors < 0 || max_errors >= m_) {
    PzError::report_error(
        PzError::PzErrorType::PZ_INVALID_ANALYSIS_TYPE,
        "max_errors must satisfy 0 <= max_errors < pattern length");
  }

  const st32 n = static_cast<st32>(text.length());
  if (n < m_ - max_errors)
    return results;

  // Stage 1: Bitap Filter to collect candidate end positions
  std::vector<st32> candidate_ends = run_bitap_filter(text, max_errors);

  // Stage 2: Local Bounded DP Verification (appends ALL valid matches)
  for (st32 end_idx : candidate_ends) {
    verify_local_dp(text, end_idx, max_errors, results);
  }

  return results;
}

std::vector<st32>
LayeredFuzzyMatcher::run_bitap_filter(const std::string &text,
                                      st32 max_errors) const {
  std::vector<st32> candidates;
  const st32 n = static_cast<st32>(text.length());

  const st64 full_mask = (m_ == 64) ? ~0ULL : ((1ULL << m_) - 1);
  const st64 end_bit = 1ULL << (m_ - 1);

  st64 p_mask[256];
  std::fill(std::begin(p_mask), std::end(p_mask), full_mask);
  for (st32 i = 0; i < m_; ++i) {
    p_mask[static_cast<unsigned char>(pattern_[i])] &= ~(1ULL << i);
  }

  std::vector<st64> R(max_errors + 1, full_mask);

  for (st32 i = 0; i < n; ++i) {
    R[0] &= ~1ULL;
    for (st32 d = 1; d <= max_errors; ++d) {
      R[d] &= (R[d - 1] << 1) | ~full_mask;
    }

    st64 char_mask = p_mask[static_cast<unsigned char>(text[i])];
    std::vector<st64> newR(max_errors + 1, full_mask);

    newR[0] = ((R[0] << 1) | char_mask) & full_mask;

    for (st32 d = 1; d <= max_errors; ++d) {
      st64 match = (R[d] << 1) | char_mask;
      st64 sub = R[d - 1] << 1;
      st64 del = R[d - 1];
      st64 ins = newR[d - 1] << 1;

      newR[d] = (match & sub & del & ins) & full_mask;
    }

    R = std::move(newR);

    if ((R[max_errors] & end_bit) == 0) {
      candidates.push_back(i);
    }
  }
  return candidates;
}

void LayeredFuzzyMatcher::verify_local_dp(
    const std::string &text, st32 end_idx, st32 max_errors,
    std::vector<Match> &out_matches) const {
  st32 w_start = std::max(0, end_idx - m_ - max_errors + 1);
  st32 w_len = end_idx - w_start + 1;

  std::string W = text.substr(w_start, w_len);
  std::string W_rev(W.rbegin(), W.rend());
  std::string P_rev(pattern_.rbegin(), pattern_.rend());

  // dp[L][j] stores the edit distance between text[end_idx - L + 1 ... end_idx]
  // and pattern[0 ... j-1]
  std::vector<std::vector<st32>> dp(w_len + 1, std::vector<st32>(m_ + 1, 0));

  for (st32 j = 0; j <= m_; ++j)
    dp[0][j] = j;
  for (st32 i = 0; i <= w_len; ++i)
    dp[i][0] = i;

  for (st32 i = 1; i <= w_len; ++i) {
    for (st32 j = 1; j <= m_; ++j) {
      st32 cost = (W_rev[i - 1] == P_rev[j - 1]) ? 0 : 1;
      dp[i][j] = std::min({dp[i - 1][j - 1] + cost, // Match / Substitution
                           dp[i - 1][j] + 1,        // Deletion from text
                           dp[i][j - 1] + 1});      // Insertion into text
    }
  }

  // Iterate L backwards so that start_index is emitted in ascending order
  for (st32 L = w_len; L >= 1; --L) {
    st32 start_idx = end_idx - L + 1;
    st32 dist = dp[L][m_];
    if (dist <= max_errors) {
      out_matches.push_back({start_idx, end_idx, dist});
    }
  }
}