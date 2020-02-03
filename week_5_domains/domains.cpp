#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;


bool IsSubDomain(const string& rdomain, const string& rsub_domain) {
    auto rd_it = rdomain.begin();
    auto rsd_it = rsub_domain.begin();

    while (rd_it != rdomain.end() and rsd_it != rsub_domain.end()) {
        if (*rd_it != *rsd_it) {
            return false;
        }
        ++rd_it;
        ++rsd_it;
    }
    if (rd_it == rdomain.end() and rsd_it == rsub_domain.end()) {
        return true;
    }
    return rd_it == rdomain.end() and *rsd_it == '.';
}


void TestIsSubDomain() {
    {
        string domain = "ya.ru";
        string sub_domain = "maps.ya.ru";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }
    {
        string domain = "ya.ru";
        string sub_domain = "maps.google.ru";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (not IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }
    {
        string domain = "ya.ru";
        string sub_domain = "yya.ru";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (not IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }
    {
        string domain = "ya.ru";
        string sub_domain = "ya.ru";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }
    {
        string domain = "com";
        string sub_domain = "ya.com";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }
    {
        string domain = "ya.ru";
        string sub_domain = "maps.ya.ru";
        reverse(domain.begin(), domain.end());
        reverse(sub_domain.begin(), sub_domain.end());

        if (IsSubDomain(domain, sub_domain)) {
            cout << "ok" << endl;
        } else {
            cout << "fail" << endl;
        }
    }

}

void Show(const set <string>& bad_domains) {
    for (const auto& line : bad_domains) {
        cerr << line << endl;
    }
    cerr << endl;
}


set <string> ReadBadDomains(int n) {
    set <string> domains;
    string line;
    for (int i = 0; i < n; ++i) {
        cin >> line;
        reverse(line.begin(), line.end());
        domains.insert(line);
    }

    auto domain_it = domains.begin();
    auto sub_domain_it = domains.begin();
    ++sub_domain_it;

    while (sub_domain_it != domains.end()) {
        if (IsSubDomain(*domain_it, *sub_domain_it)) {
            domains.erase(sub_domain_it);
        } else {
            domain_it = sub_domain_it;
        }
        sub_domain_it = domain_it;
        ++sub_domain_it;
    }

    return domains;
}



void Solve() {
    int bad_domains_amount;
    cin >> bad_domains_amount;
    set <string> bad_domains = ReadBadDomains(bad_domains_amount);

    int check_domains_amount;
    cin >> check_domains_amount;
    string domain;
    for (int i = 0; i < check_domains_amount; ++i) {
        cin >> domain;
//        cerr << domain << ' ';

        reverse(domain.begin(), domain.end());
//        cerr << domain << endl;

        auto l_it = bad_domains.lower_bound(domain);

//        if (l_it != bad_domains.end()) {
//            cerr << *l_it << endl;
//        }

        if (l_it != bad_domains.end() and IsSubDomain(*l_it, domain)) {
            cout << "Bad" << endl;
        } else if (l_it-- != bad_domains.begin() and IsSubDomain(*l_it, domain)) {
            cout << "Bad" << endl;
        } else {
            cout << "Good" << endl;
        }
    }
}


int main() {

    Solve();
//    TestIsSubDomain();
    return 0;
}