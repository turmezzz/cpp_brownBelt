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
        {
            auto it = start_2_finishes_.find(start);
            if (it == start_2_finishes_.end() or it->second.empty()) {
                return abs(finish - start);
            }
        }
        {
            set <int>& box = start_2_finishes_.find(start)->second;
            auto it = box.lower_bound(finish);
            if (it == box.end()) {
                --it;
            }
            int dist = min(abs(finish - start) , abs(*it - finish));
            if (it != start_2_finishes_.find(start)->second.begin())
            {
                int left = abs(*(--it) - finish);
                dist = min(left, dist);
            }
            return dist;
        }
    }

    void Show() const {
        for (auto&[start, finishes] : start_2_finishes_) {
            cout << start << "->";
            for (int finish : finishes) {
                cout << finish << ' ';
            }
            cout << endl;
        }
    }
};

double StressTest()
{
    double negative = 0;

    ExpressesManager my;
    RouteManager their;

    mt19937 gen(123);
    uniform_int_distribution <int> dist(0, 2);

    int n = 10;
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
//            my_ret = my.Go(start, finish);
//            their_ret = their.FindNearestFinish(start, finish);
            my_ret = my.Go(start, 20);
            their_ret = their.FindNearestFinish(start, 20);
            log << "GO " << start << ' ' << 20 << "<-> my: " << my_ret << ", their: " << their_ret << '\n';

            if (my_ret != their_ret) {
                ++negative;
                break;
            }
        }
    }
    double failure_rate = negative / static_cast<double>(n);
    if (failure_rate != 0) {
        cerr << log.str() << endl;
    }
    return failure_rate;
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
//            mng.Show();
            cout << mng.Go(start, finish) << '\n';
        }
    }
}

int main() {
    Solve();
//    cout << StressTest() << endl;
    return 0;
}

// problem was in my head.
// I didn't know how to use lower_bound correctly

/*
7
ADD 2 2
ADD 1 0
ADD 1 2
ADD 1 1
ADD 2 1
ADD 1 1
GO 1 20
 */