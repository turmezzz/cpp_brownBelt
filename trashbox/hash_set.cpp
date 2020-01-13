#include "../test_runner.h"

#include <forward_list>
#include <iterator>
#include <utility>

using namespace std;

template <typename Type, typename Hasher>
class HashSet
{
public:
	using BucketList = forward_list <Type>;

private:
	vector <BucketList> data_d;
	size_t num_buckets_d;
	Hasher hasher_d;

public:
	explicit HashSet(size_t num_buckets, const Hasher& hasher = {})
	{
		num_buckets_d = num_buckets;
		data_d = vector <BucketList>(num_buckets_d);
		hasher_d = hasher;
	}

	void Add(const Type& value)
	{
		if (Has(value)) return;
		auto index = hasher_d(value) % num_buckets_d;
		auto& bucket_list = data_d[index];
		bucket_list.push_front(value);
	}

	bool Has(const Type& value) const
	{
		auto index = hasher_d(value) % num_buckets_d;
		auto& bucket_list = data_d[index];
		for (const auto& i : bucket_list)
		{
			if (value == i) return true;
		}
		return false;
	}


	void Erase(const Type& value)
	{
		auto index = hasher_d(value) % num_buckets_d;
		auto& bucket_list = data_d[index];
		BucketList new_bucket_list;
		for (auto it = bucket_list.begin(); it != bucket_list.end(); ++it)
		{
			if (*it != value)
			{
				new_bucket_list.push_front(move(*it));
			}
		}
		data_d[index] = new_bucket_list;
	}

	const BucketList& GetBucket(const Type& value) const
	{
		auto index = hasher_d(value) % num_buckets_d;
		auto& bucket_list = data_d[index];
		return bucket_list;
	}
};

struct IntHasher
{
	size_t operator()(int value) const
	{
		// Это реальная хеш-функция из libc++, libstdc++.
		// Чтобы она работала хорошо, std::unordered_map
		// использует простые числа для числа бакетов
		return value;
	}
};

struct TestValue
{
	int value;

	bool operator==(TestValue other) const
	{
		return value / 2 == other.value / 2;
	}
};

struct TestValueHasher
{
	size_t operator()(TestValue value) const
	{
		return value.value / 2;
	}
};

void TestSmoke()
{
	HashSet <int, IntHasher> hash_set(2);
	hash_set.Add(3);
	hash_set.Add(4);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Erase(3);

	ASSERT(!hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Add(3);
	hash_set.Add(5);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(hash_set.Has(5));
}

void TestEmpty()
{
	HashSet <int, IntHasher> hash_set(10);
	for (int value = 0; value < 10000; ++value)
	{
		ASSERT(!hash_set.Has(value));
	}
}

void TestIdempotency()
{
	HashSet <int, IntHasher> hash_set(10);
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
}

void TestEquivalence()
{
	HashSet <TestValue, TestValueHasher> hash_set(10);
	hash_set.Add(TestValue{2});
	hash_set.Add(TestValue{3});

	ASSERT(hash_set.Has(TestValue{2}));
	ASSERT(hash_set.Has(TestValue{3}));

	const auto& bucket = hash_set.GetBucket(TestValue{2});
	const auto& three_bucket = hash_set.GetBucket(TestValue{3});
	ASSERT_EQUAL(&bucket, &three_bucket);

	ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
	ASSERT_EQUAL(2, bucket.front().value);
}

int main()
{
	TestRunner tr;
	RUN_TEST(tr, TestSmoke);
	RUN_TEST(tr, TestEmpty);
	RUN_TEST(tr, TestIdempotency);
	RUN_TEST(tr, TestEquivalence);
	return 0;
}
