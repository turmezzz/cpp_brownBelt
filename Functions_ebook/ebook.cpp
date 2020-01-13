#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
    ReadingManager() : users_amount_(0), users_(MAX_USERS_COUNT_), pages_(MAX_PAGES_COUNT_) {

    }

    void Read(int user_id, int page_count) {
        int pages_read = users_[user_id];
        if (pages_[pages_read] > 0) {
            --pages_[pages_read];
            --users_amount_;
        }

        ++pages_[page_count];
        users_[user_id] = page_count;
        ++users_amount_;
    }

    double Cheer(int user_id) const {
        if (users_[user_id] == 0) {
            return 0;
        }
        if (users_amount_ == 1) {
            return 1;
        }
        int pages_read = users_[user_id];
        int less_then_me = 0;
        if (pages_read < 1000 - pages_read) {
            for (int i = 0; i < pages_read; ++i) {
                less_then_me += pages_[i];
            }
        } else {
            int more_or_equal_then_me = 0;
            for (int i = 1000; i >= pages_read; --i) {
                more_or_equal_then_me += pages_[i];
            }
            less_then_me = users_amount_ - more_or_equal_then_me;
        }
        return static_cast <double>(less_then_me) / static_cast <double>(users_amount_ - 1);
    }

private:
    static const int MAX_USERS_COUNT_ = 100'005;
    static const int MAX_PAGES_COUNT_ = 1'005;

    vector <int> users_;
    vector <int> pages_;
    int users_amount_;
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}