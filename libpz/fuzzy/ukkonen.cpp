// Ukkonen Cutoff DP
#include "ukkonen.hpp"
#include <vector>
#include <algorithm>
#include <cstdlib>


bool Ukkonen::edit_within_k(const std::string& s, const std::string& t, int k) {
    if (k < 0) return false;
    int m = s.size();
    int n = t.size();
    if(std::abs(m-n) > k) return false;
    int INF= k + 1;
    std::vector<int> prev(2*k + 3, INF);
    std::vector<int> curr(2*k + 3, INF);

    for(int j = 0; j <= std::min(n, k); j++) {
        int d=j;
        prev[d+k+1] = j;
    }
    for(int i= 1; i <= m; i++) {
        curr[0] = i;
        int left = std::max(1, i-k);
        int right = std::min(n, i+k);
        for (int j=left; j<= right; j++) {
            int d = j-i;
            int idx = d+k+1;
            int cost = (s[i-1] == t[j-1]) ? 0 : 1;
            curr[idx] = std::min({
                prev[idx+1] + 1,
                curr[idx-1] + 1,
                prev[idx] + cost
            });
        }
        swap(prev, curr);
    }
    return prev[n-m+k+1] <= k;
}