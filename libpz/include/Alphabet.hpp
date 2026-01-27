#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include <pz_cxx_std.hpp>
#include <pz_types.hpp>

inline std::map<std::string, char> RESERVED_SYMBOLS = {{"EOB", 0},
                                                       {"CONCATNATION", 1}};

extern const int ALPHABET_SIZE;

char get_position(char c);

#endif /* ALPHABET_HPP */
