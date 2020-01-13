#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;

    virtual void Process(unique_ptr <Email> email) = 0;

    virtual void Run() {

        throw logic_error("Unimplemented");
    }

protected:

    void PassOn(unique_ptr <Email> email) const {
        if (next_d) {
            next_d->PassOn(move(email));
        }
    }

public:
    void SetNext(unique_ptr <Worker> next) {
        next_d = move(next);
    }

private:
    unique_ptr <Worker> next_d;
};


class Reader : public Worker {
public:
    explicit Reader(istream& in) : in_d(in) {
    }

    void Process(unique_ptr <Email> email) override {
        PassOn(move(email));
    }

    void Run() override {
        string from;
        string to;
        string body;
        in_d >> from >> to >> body;
        auto email = unique_ptr <Email>(new Email{from, to, body});
        Process(move(email));
    }


private:
    istream& in_d;
};


class Filter : public Worker {
public:
    using Function = function <bool(const Email&)>;

public:
    explicit Filter(Function func) : func_d(func) {
    }

    void Process(unique_ptr <Email> email) {
        if (func_d(*email)) PassOn(move(email));
    }

private:
    Function func_d;
};


class Copier : public Worker {
public:
    explicit Copier(string copyTo) : copyTo_d(move(copyTo)) {
    }

    void Process(unique_ptr <Email> email) override {
        if (email->to != copyTo_d) {
            auto copy = make_unique <Email>();
            copy->from = email->from;
            copy->to = copyTo_d;
            copy->body = email->body;
            PassOn(move(copy));
        }
        PassOn(move(email));
    }

private:
    string copyTo_d;
};


class Sender : public Worker {
public:
    explicit Sender(ostream& out) : out_d(out) {
    }

    void Process(unique_ptr <Email> email) override {
        out_d << email->from << '\n';
        out_d << email->to << '\n';
        out_d << email->body << '\n';
        out_d << '\n';
    }

private:
    ostream& out_d;
};


class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in) {
        head_d = make_unique <Reader>(in);
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        func_d = move(filter);
    }

    PipelineBuilder& CopyTo(string copyTo) {
        copyTo_d = move(copyTo);
    }

    PipelineBuilder& Send(ostream& out) {
        sender_d = make_unique <Sender>(out);
    }

    unique_ptr <Worker> Build()
    {
        auto copier = make_unique <Copier>(copyTo_d);
        copier->SetNext(move(sender_d));

        auto filter = make_unique <Filter>(func_d);
        filter->SetNext(move(copier));

        head_d->SetNext(move(filter));

        return move(head_d);
    }

private:
    unique_ptr <Worker> head_d;
    Filter::Function func_d;
    string copyTo_d;
    unique_ptr <Sender> sender_d;
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}