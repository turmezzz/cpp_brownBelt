#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class ExpressesManager {
private:
    unordered_map <int64_t, set <int64_t>> start_2_finishes_;
public:

    void Add(int64_t start, int64_t finish) {
        start_2_finishes_[start].insert(finish);
        start_2_finishes_[finish].insert(start);
    }

    int64_t Go(int64_t start, int64_t finish) {
        if (start_2_finishes_.find(start) == start_2_finishes_.end()) {
            return abs(finish - start);
        }
        auto it = start_2_finishes_.find(start)->second.lower_bound(finish);
        int dist = abs(*it - finish);
        if (it != start_2_finishes_.find(start)->second.begin())
        {
            int left = abs(*(--it) - finish);
            dist = min(left, dist);
        }
        return min<int64_t>(dist, abs(finish - start));
    }
};

int main() {
    int n;
    cin >> n;
    string cmd;
    int64_t start, finish;
    ExpressesManager mng;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        cin >> start >> finish;
        if (cmd[0] == 'A') {
            mng.Add(start, finish);
        } else if (cmd[0] == 'G') {
            cout << mng.Go(start, finish) << '\n';
        }
    }
    return 0;
}