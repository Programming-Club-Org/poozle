#ifndef UKKONEN_HPP
#define UKKONEN_HPP
#include <string>
#include <vector>
#include <algorithm>

class Ukkonen {
public:
    explicit Ukkonen(int k);
    bool edit_within_k(const std::string& s, const std::string& t);

private:
    int k_;
};
#endif 