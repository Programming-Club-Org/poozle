#ifndef BITAP_HPP
#define BITAP_HPP

#include "../include/pz_cxx_std.hpp"
#include "../include/pz_types.hpp"

/**
 * @file Bitap.hpp
 * @brief 2-Stage Layered Fuzzy Search Engine (Bitap Filter + Multi-Match Local
 * DP Verifier).
 */

class LayeredFuzzyMatcher {
public:
  struct Match {
    st32 start_index;
    st32 end_index;
    st32 distance;
  };

  /**
   * @brief Constructs matcher for a given search pattern.
   * @param pattern The target pattern string to search for (max length 64).
   * @throws PzError::PzErrorType::PZ_LONG_PATTERN_ERROR If pattern length
   * exceeds 64 characters.
   */
  explicit LayeredFuzzyMatcher(const std::string &pattern);

  /**
   * @brief Performs layered fuzzy search across input text within error bounds.
   * @param text Target text string to search within.
   * @param max_errors Maximum allowed Levenshtein edit distance (0 <= max_errors
   * < pattern.length()).
   * @return std::vector<Match> Collection of matches with start/end indices and
   * edit distances.
   * @throws PzError::PzErrorType::PZ_INVALID_ANALYSIS_TYPE If max_errors is
   * invalid.
   */
  std::vector<Match> search(const std::string &text, st32 max_errors) const;

private:
  std::string pattern_;
  st32 m_;

  /**
   * @brief Stage 1: Fast Bitap bit-parallel filter to identify match candidate
   * end positions.
   */
  std::vector<st32> run_bitap_filter(const std::string &text,
                                     st32 max_errors) const;

  /**
   * @brief Stage 2: Local bounded DP verification to evaluate exact candidate
   * windows.
   */
  void verify_local_dp(const std::string &text, st32 end_idx, st32 max_errors,
                       std::vector<Match> &out_matches) const;
};

#endif // BITAP_HPP