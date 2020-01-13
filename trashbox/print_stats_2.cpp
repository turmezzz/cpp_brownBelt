#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

template <typename Iterator>
class IteratorRange
{
public:
	IteratorRange(Iterator begin, Iterator end)
			: first(begin), last(end)
	{
	}

	Iterator begin() const
	{
		return first;
	}

	Iterator end() const
	{
		return last;
	}

private:
	Iterator first, last;
};

template <typename Collection>
auto Head(const Collection& v, size_t top)
{
	return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person
{
	string name;
	int age, income;
	bool is_male;
};

vector <Person> ReadPeople(istream& input)
{
	int count;
	input >> count;

	vector <Person> result(count);
	for (Person& p : result)
	{
		char gender;
		input >> p.name >> p.age >> p.income >> gender;
		p.is_male = gender == 'M';
	}

	return result;
}

int main()
{
	vector <Person> all_people = ReadPeople(cin);

	const vector <Person> people_sorted_by_age = [&all_people]
	{
		auto box(all_people);
		sort(begin(box), end(box), [](const Person& lhs, const Person& rhs)
		{
			return lhs.age < rhs.age;
		});
		return move(box);
	}();

	const vector <Person> people_sorted_by_income = [&all_people]
	{
		auto box(all_people);
		sort(begin(box), end(box), [](const Person& lhs, const Person& rhs)
		{
			return lhs.income > rhs.income;
		});
		return move(box);
	}();

	vector <Person>::const_iterator male_end;
	const vector <Person> people_sorted_by_name = [&all_people, &male_end]
	{
		auto box(all_people);

		IteratorRange range{
				begin(box),
				partition(begin(box), end(box), [](const Person& p)
				{
					return p.is_male;
				})
		};
		male_end = box.end();
		sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs)
		{
			return lhs.name < rhs.name;
		});
		sort(range.end(), box.end(), [](const Person& lhs, const Person& rhs)
		{
			return lhs.name < rhs.name;
		});
		return move(box);
	}();

	for (string command; cin >> command;)
	{
		if (command == "AGE")
		{
			int adult_age;
			cin >> adult_age;

			auto adult_begin = lower_bound(
					begin(people_sorted_by_age), end(people_sorted_by_age), adult_age, [](const Person& lhs, int age)
					{
						return lhs.age < age;
					}
			);

			cout << "There are " << std::distance(adult_begin, end(people_sorted_by_age))
				 << " adult people for maturity age " << adult_age << '\n';
		}
		else if (command == "WEALTHY")
		{
			int count;
			cin >> count;

			auto head = Head(people_sorted_by_income, count);

			int total_income = accumulate(
					head.begin(), head.end(), 0, [](int cur, const Person& p)
					{
						return p.income + cur;
					}
			);
			cout << "Top-" << count << " people have total income " << total_income << '\n';
		}
		else if (command == "POPULAR_NAME")
		{
			char gender;
			cin >> gender;

			pair <vector <Person>::const_iterator, vector <Person>::const_iterator> box = [&people_sorted_by_name, gender, male_end]
			{
				if (gender == 'M')
				{
					return make_pair(people_sorted_by_name.begin(), male_end);
				}
				else
				{
					return make_pair(male_end, people_sorted_by_name.end());
				}
			}();

			IteratorRange range{box.first, box.second};

			if (range.begin() == range.end())
			{
				cout << "No people of gender " << gender << '\n';
			}
			else
			{
				const string* most_popular_name = &range.begin()->name;
				int count = 1;
				for (auto i = range.begin(); i != range.end();)
				{
					auto same_name_end = find_if_not(i, range.end(), [i](const Person& p)
					{
						return p.name == i->name;
					});
					auto cur_name_count = std::distance(i, same_name_end);
					if (cur_name_count > count)
					{
						count = cur_name_count;
						most_popular_name = &i->name;
					}
					i = same_name_end;
				}
				cout << "Most popular name among people of gender " << gender << " is "
					 << *most_popular_name << '\n';
			}
		}
	}
}
