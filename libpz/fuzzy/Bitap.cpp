#include "../include/Bitap.hpp"
#include "../include/pz_error.hpp"
#include "../include/pz_types.hpp"
#include "../include/pz_cxx_std.hpp"

/**
 * @brief Searches for mutliple occurrences of a pattern in a text using the Bitap algorithm.
 *
 * @param word The target text string to search within.
 * @param pattern The substring pattern to search for.
 * 
 * @return std::vector<st32> The 0-based starting indices of multiple matches, or empty if no match is found.
 * 
 * @throws PzError::PzErrorType::PZ_LONG_PATTERN_ERROR If pattern length exceeds 63 characters.
 */
std::variant<st32, std::array<st32, 64>> bitap_search(const std::string& text, const std::string& pattern, st32 max_errors) {
    const std::size_t m = pattern.length();
    const std::size_t n = text.length();

    std::array<st32, 64> locations; locations.fill(-1);
    st32 index = 0;

    if (m == 0) {
        return locations;    // as no match is found
    }
    if (m > 63) {
        PzError::report_error(PzError::PzErrorType::PZ_LONG_PATTERN_ERROR, "Pattern length is greater than 63");
        return locations;
    }
    if (max_errors >= m) {
        PzError::report_error(PzError::PzErrorType::PZ_INVALID_ANALYSIS_TYPE, "All the pattern are a match with such a high max_errors.");
        return locations;
    }

    /** Precomputed bitmask for each character byte (0-255). */
    st64 p_mask[256];

    // Initialize all character masks to all 1-bits (no matches)
    std::fill(std::begin(p_mask), std::end(p_mask), ~0ULL);
    
    // State array for 0 to max_errors errors
    std::vector<st64> R(max_errors + 1, ~0ULL);

    // Initial state setup for insertion/deletion at string boundaries
    for (st32 d = 0; d <= max_errors; ++d) {
        R[d] = ~0ULL << d;
    }
    

    // Set bit 'i' to 0 wherever character pattern[i] appears
    for (std::size_t i = 0; i < m; ++i) {
        p_mask[static_cast<unsigned char>(pattern[i])] &= ~(1ULL << i);
    }

    // Process each character of the text
    for (std::size_t i = 0; i < n; ++i) {
        st64 char_mask = p_mask[static_cast<unsigned char>(text[i])];
        
        st64 R_old_prev = R[0];
        R[0] = (R[0] << 1) | char_mask;
        
        for (st32 d = 1; d <= max_errors; ++d) {
            st64 R_old_curr = R[d];
            
            // Apply Wu-Manber bitwise state transitions
            st64 sub  = R_old_prev << 1;
            st64 del  = R_old_prev;
            st64 ins  = R[d - 1] << 1;
            st64 match = (R_old_curr << 1) | char_mask;

            R[d] = match & sub & del & ins;
            R_old_prev = R_old_curr;
        }

        // If highest level state bit m is 0, a match with <= max_errors exists
        if ((R[max_errors] & (1ULL << (m - 1))) == 0) {
            locations[index++] = (static_cast<st32>(i - m + 1));
        }
    }

    return locations;
}