#ifndef UKKONEN_HPP
#define UKKONEN_HPP

#include <string>

class Ukkonen {
public:
    bool edit_within_k(const std::string& s,
                       const std::string& t,
                       int k);
};

#endif