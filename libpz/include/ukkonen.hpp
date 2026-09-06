#ifndef UKKONEN_HPP
#define UKKONEN_HPP

#include <string>
#include <vector>
#include <utility> //for std::pair

class Ukkonen {
public:
    /**
     * Finds whether two strings are within edit distance k 
     * of each other using Ukkonen's algorithm.
     * @param s The first string.
     * @param t The second string.
     * @param k The maximum edit distance.
     * @return True if the edit distance between s and t is 
     * less than or equal to k, false otherwise.
     */

    bool edit_within_k(const std::string& s,
                       const std::string& t,
                       int k);
    
    /**
     * Finds every non-empty substring of text whose edit distance 
     * from pattern is at most k.
     * 
     * Each match is represented as a pair of integers (start, end) 
     * where start is the starting index of the match in text and end is the ending index of the match in text.
     * Both the indices are 0-based, and inclusive.
     *
     * Different substrings are returned as different pairs, even
     * when they have the same starting position.
     * 
     * @param pattern The pattern string to match against.
     * @param text The text string to search within.
     * @param k The maximum edit distance allowed for a match.
     * @return A vector of pairs, where each pair represents the starting and ending indices 
     * of a substring in text that matches pattern within edit distance k.
    */

    std::vector<std::pair<int, int>> locate(
        const std::string &pattern,
        const std::string &text,
        int k);
    
    /** 
     * Counts every non-empty substring whose edit distance from pattern is at most k.
     * 
     * @param pattern The pattern string to match against.
     * @param text The text string to search within.
     * @param k The maximum edit distance allowed for a match.
     * @return The count of substrings in text that match pattern within edit distance k.
     */
    
    int count(const std::string &pattern,
              const std::string &text,
              int k);
    
};

#endif //UKKONEN_HPP