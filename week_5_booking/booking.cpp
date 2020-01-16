#include <iostream>
#include <set>
#include <deque>
#include <unordered_map>
#include <string>

using namespace std;

class HotelManager {
private:

    struct Booking {
        int64_t time;
        int client_id;
        int room_count;
    };

    unordered_map <string, deque <Booking>> hotel_2_bookings;
    unordered_map <string, multiset <int>> hotel_2_client_ids;
    unordered_map <string, int> hotel_2_room_count;
    int64_t last_query_time = 0;


    bool NeedToDelete(int64_t time, int64_t l, int64_t r) {
        return not (l < time and time <= r);
    }

    void Update(const string& hotel_name) {
        if (hotel_2_bookings.find(hotel_name) == hotel_2_bookings.end()) {
            return;
        }

        auto& hotel_bookings = hotel_2_bookings[hotel_name];
        auto& hotel_client_ids = hotel_2_client_ids[hotel_name];
        auto& hotel_room_count = hotel_2_room_count[hotel_name];

        int64_t r_time = last_query_time;
        int64_t l_time = r_time - 86400;


        while (not hotel_bookings.empty() and NeedToDelete(hotel_bookings.front().time, l_time, r_time)) {
            auto& booking = hotel_bookings.front();
            {
                auto it = hotel_client_ids.find(booking.client_id);
                hotel_client_ids.erase(it);
            }
            {
                hotel_room_count -= booking.room_count;
            }
            hotel_bookings.pop_front();
        }
    }

public:

    void Book(int64_t time, const string& hotel_name, int client_id, int room_count) {
        last_query_time = time;

        auto& hotel_bookings = hotel_2_bookings[hotel_name];
        auto& hotel_client_ids = hotel_2_client_ids[hotel_name];
        auto& hotel_room_count = hotel_2_room_count[hotel_name];

        hotel_bookings.emplace_back(Booking{time, client_id, room_count});
        hotel_client_ids.insert(client_id);
        hotel_room_count += room_count;
    }

    int Clients(const string& hotel_name) {
        Update(hotel_name);
        return static_cast<int>(hotel_2_client_ids[hotel_name].size());
    }

    int Rooms(const string& hotel_name) {
        Update(hotel_name);
        return hotel_2_room_count[hotel_name];
    }
};

void Solve() {
    int n;
    cin >> n;
    HotelManager mng;
    string cmd;
    int64_t time;
    string hotel_name;
    int client_id;
    int room_count;

    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd[0] == 'B') {
            // book
            cin >> time >> hotel_name >> client_id >> room_count;
            mng.Book(time, hotel_name, client_id, room_count);

        } else if (cmd[0] == 'C') {
            // clients

            cin >> hotel_name;
            cout << mng.Clients(hotel_name) << '\n';

        } else if (cmd[0] == 'R') {
            // rooms

            cin >> hotel_name;
            cout << mng.Rooms(hotel_name) << '\n';

        }
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Solve();

    return 0;
}