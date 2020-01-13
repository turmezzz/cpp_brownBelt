#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

namespace ExternalCode {
    enum class Gender {
        FEMALE,
        MALE
    };


    struct Person {
        int age;  // возраст
        Gender gender;  // пол
        bool is_employed;  // имеет ли работу
    };


// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
    template <typename InputIt>
    int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
        if (range_begin == range_end) {
            return 0;
        }
        vector <typename InputIt::value_type> range_copy(range_begin, range_end);
        auto middle = begin(range_copy) + range_copy.size() / 2;
        nth_element(
                begin(range_copy), middle, end(range_copy),
                [](const Person& lhs, const Person& rhs) {
                    return lhs.age < rhs.age;
                }
        );
        return middle->age;
    }
}

//using namespace ExternalCode;

void PrintStats(vector <Person> persons) {
    {
        // ALL
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return true;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age = " << ret << '\n';
    }
    {
        // FEMALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::FEMALE;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for females = " << ret << '\n';
    }
    {
        // MALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::MALE;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for males = " << ret << '\n';
    }
    {
        // NO WORK FEMALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::FEMALE and p.is_employed;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for employed females = " << ret << '\n';
    }
    {
        // WORK FEMALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::FEMALE and not p.is_employed;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for unemployed females = " << ret << '\n';
    }
    {
        // NO WORK MALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::MALE and p.is_employed;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for employed males = " << ret << '\n';
    }
    {
        // WORK MALE
        auto end = partition(persons.begin(), persons.end(), [](Person& p) {
            return p.gender == Gender::MALE and not p.is_employed;
        });
        auto size = end - persons.begin();
        int ret = 0;
        if (size != 0) {
            nth_element(persons.begin(), persons.begin() + size / 2, end,
                        [](Person& first, Person& second) {
                            return first.age < second.age;
                        });
            ret = (persons.begin() + size / 2)->age;
        }
        cout << "Median age for unemployed males = " << ret << '\n';
    }
}

int main() {
    {
        vector <Person> persons = {
                {31, Gender::MALE,   false},
                {40, Gender::FEMALE, true},
                {24, Gender::MALE,   true},
                {20, Gender::FEMALE, true},
                {80, Gender::FEMALE, false},
                {78, Gender::MALE,   false},
                {10, Gender::FEMALE, false},
                {55, Gender::MALE,   true},
        };
        PrintStats(persons);
    }
    {
        vector <Person> persons = {
        };
        PrintStats(persons);
    }
    {
        vector <Person> persons = {
                {31, Gender::MALE, false},
        };
        PrintStats(persons);
    }
    {
        vector <Person> persons = {
                {31, Gender::FEMALE, false},
        };
        PrintStats(persons);
    }
    {
        vector <Person> persons = {
                {31, Gender::MALE, true},
        };
        PrintStats(persons);
    }


    return 0;
}
