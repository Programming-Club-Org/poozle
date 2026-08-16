// Ukkonen Cutoff DP
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool edit_within_k(const string& s, const string& t, int k) {
    if (k < 0) return false;
    int m = s.size();
    int n = t.size();
    if(abs(m-n) > k) return false;
    int INF= k + 1;
    vector<int> prev(2*k + 3, INF);
    vector<int> curr(2*k + 3, INF);

    for(int j = 0; j <= min(n, k); j++) {
        int d=j;
        prev[d+k+1] = j;
    }
    for(int i= 1; i <= m; i++) {
        curr[0] = i;
        int left = max(1, i-k);
        int right = min(n, i+k);
        for (int j=left; j<= right; j++) {
            int d = j-i;
            int idx = d+k+1;
            int cost = (s[i-1] == t[j-1]) ? 0 : 1;
            curr[idx] = min({
                prev[idx+1] + 1,
                curr[idx-1] + 1,
                prev[idx] + cost
            });
        }
        swap(prev, curr);
    }
    return prev[n-m+k+1] <= k;
}