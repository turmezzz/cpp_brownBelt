#include "../test_runner.h"
#include "../profile.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <list>
#include <string_view>

using namespace std;

struct Record
{
	string id;
	string title;
	string user;
	int timestamp;
	int karma;
};

class Database
{
	using Record_it = list <Record>::iterator;
	using erase_it = multimap <int, Record_it>::iterator;

	struct PairsSucks
	{
		erase_it first;
		erase_it second;
	};

	list <Record> data_d;
	unordered_map <string, Record_it> id_index_d;
	multimap <int, Record_it> timestamp_secondary_index_d;
	multimap <int, Record_it> karma_secondary_index_d;
	unordered_multimap <string, Record_it> user_secondary_index_d;
	unordered_map <string, PairsSucks> erase_iterators_index_d;

	bool in(const string& id) const
	{
		return id_index_d.find(id) != id_index_d.end();
	}

public:
	bool Put(const Record& record)
	{
		{
			LOG_DURATION("Put")
			if (in(record.id)) return false;
			data_d.push_back(record);
			auto last = data_d.end();
			--last;

			id_index_d[record.id] = last;

			auto time_it = timestamp_secondary_index_d.insert({record.timestamp, last});
			auto karma_it = karma_secondary_index_d.insert({record.karma, last});
			user_secondary_index_d.insert({record.user, last});

			erase_iterators_index_d.insert({record.id, {time_it, karma_it}});
			return true;
		}
	}

	const Record* GetById(const string& id) const
	{
		{
			LOG_DURATION("GetById")
			if (not in(id)) return nullptr;
			return &*id_index_d.at(id);
		}
	}

	bool Erase(const string& id)
	{
		{
			LOG_DURATION("Erase")
			if (not in(id)) return false;
			auto record_erase_it = id_index_d.find(id);
			auto erase_it = erase_iterators_index_d.find(id);
			auto[time_erase_it, karma_erase_it] = erase_it->second;
			auto range = user_secondary_index_d.equal_range(record_erase_it->second->user);

			erase_iterators_index_d.erase(erase_it);

			timestamp_secondary_index_d.erase(time_erase_it);
			karma_secondary_index_d.erase(karma_erase_it);
			for (auto user_erase_it = range.first; user_erase_it != range.second; ++user_erase_it)
			{
				if (user_erase_it->second->id == id)
				{
					user_secondary_index_d.erase(user_erase_it);
					break;
				}
			}

			data_d.erase(record_erase_it->second);
			id_index_d.erase(record_erase_it);
			return true;
		}
	}

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const
	{
		{
			LOG_DURATION("RangeByTimestamp")
			auto begin = timestamp_secondary_index_d.lower_bound(low);
			auto end = timestamp_secondary_index_d.upper_bound(high);
			for (auto it = begin; it != end; ++it)
			{
				if (not callback(*it->second)) return;
			}
		}

	}

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const
	{
		{
			LOG_DURATION("RangeByKarma")
			auto begin = karma_secondary_index_d.lower_bound(low);
			auto end = karma_secondary_index_d.upper_bound(high);
			for (auto it = begin; it != end; ++it)
			{
				if (not callback(*it->second)) return;
			}
		}
	}

	template <typename Callback>
	void AllByUser(const string& user, Callback callback) const
	{
		{
			LOG_DURATION("AllByUser")
			auto[begin, end] = user_secondary_index_d.equal_range(user);
			for (auto rec_it = begin; rec_it != end; ++rec_it)
			{
				if (not callback(*rec_it->second)) return;
			}
		}
	}
};

void TestRangeBoundaries()
{
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
	db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&)
	{
		++count;
		return true;
	});

	ASSERT_EQUAL(2, count);
}

void TestSameUser()
{
	Database db;
	db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
	db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

	int count = 0;
	db.AllByUser("master", [&count](const Record&)
	{
		++count;
		return true;
	});

	ASSERT_EQUAL(2, count);
}

void TestReplacement()
{
	const string final_body = "Feeling sad";

	Database db;
	db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
	db.Erase("id");
	db.Put({"id", final_body, "not-master", 1536107260, -10});

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}

int main()
{
	TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
	return 0;
}
