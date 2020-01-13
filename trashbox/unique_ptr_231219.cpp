#include "test_runner.h"

#include <cstddef>

using namespace std;

template <typename T>
class UniquePtr {
private:
    T* ptr_d;

public:

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr& operator=(const UniquePtr&) = delete;


    UniquePtr() : ptr_d(nullptr) {

    }

    UniquePtr(T* ptr) : ptr_d(ptr) {

    }

    UniquePtr(UniquePtr&& other) {
        ptr_d = other.ptr_d;
        other.ptr_d = nullptr;
    }

    UniquePtr& operator=(nullptr_t) {
        delete ptr_d;
        ptr_d = nullptr;
        return *this;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        UniquePtr <T> box(move(other));
        this->Swap(box);
        return *this;
    }

    ~UniquePtr() {
        delete ptr_d;
        ptr_d = nullptr;
    }

    T& operator*() const {
        return *ptr_d;
    }

    T* operator->() const {
        return ptr_d;
    }

    T* Release() {
        T* box = ptr_d;
        ptr_d = nullptr;
        return box;
    }

    void Reset(T* ptr) {
        if(ptr == ptr_d) {
            return;
        }
        delete ptr_d;
        ptr_d = ptr;
    }

    void Swap(UniquePtr& other) {
        swap(ptr_d, other.ptr_d);
    }

    T* Get() const {
        return ptr_d;
    }
};


struct Item {
    static int counter;
    int value;

    Item(int v = 0) : value(v) {
        ++counter;
    }

    Item(const Item& other) : value(other.value) {
        ++counter;
    }

    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr <Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr <Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr <Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}