#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <random>
#include <sstream>

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].push_back(finish);
        reachable_lists_[finish].push_back(start);
    }
    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        if (reachable_lists_.count(start) < 1) {
            return result;
        }
        const vector<int>& reachable_stations = reachable_lists_.at(start);
        if (!reachable_stations.empty()) {
            result = min(
                    result,
                    abs(finish - *min_element(
                            begin(reachable_stations), end(reachable_stations),
                            [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
                    ))
            );
        }
        return result;
    }
private:
    map<int, vector<int>> reachable_lists_;
};

class ExpressesManager {
private:
    unordered_map <int, set <int>> start_2_finishes_;
public:

    void Add(int start, int finish) {
        start_2_finishes_[start].insert(finish);
        start_2_finishes_[finish].insert(start);
    }

    int Go(int start, int finish) {
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
        return min(dist, abs(finish - start));
    }
};

double StressTest()
{
    double negative = 0;

    ExpressesManager my;
    RouteManager their;

    mt19937 gen(123);
    uniform_int_distribution <int> dist(0, 100);

    int n = 100;
    int start, finish;
    int my_ret, their_ret;
    stringstream log;
    for (int i = 0; i < n; ++i) {
        start = dist(gen);
        finish = dist(gen);
        if (dist(gen) % 2 == 0) {
            // ADD
            my.Add(start, finish);
            their.AddRoute(start, finish);
            log << "ADD " << start << ' ' << finish << '\n';
        } else {
            // GO
            my_ret = my.Go(start, finish);
            their_ret = their.FindNearestFinish(start, finish);
            log << "ADD " << start << ' ' << finish << '\n';

            if (my_ret != their_ret) {
                ++negative;
            }
        }
    }
    cerr << log.str() << endl;
    return negative / n;
}

void Solve() {
    int n;
    cin >> n;
    string cmd;
    int start, finish;
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
}

int main() {
//    Solve();
    cout << StressTest() << endl;
    return 0;
}