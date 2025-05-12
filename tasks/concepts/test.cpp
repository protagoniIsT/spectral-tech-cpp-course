#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <bitset>

#include <predicate.h>
#include <indexable.h>

void PredicateTest() {
    auto simple = [](int) {
        return true;
    };

    struct Func {
        bool operator()(double) {
            return false;
        }
    };

    std::function<bool(std::vector<int>)> vector_pred = [&simple](const std::vector<int>&) {
        return true;
    };

    struct Int {
        int x;
    };

    static_assert(Predicate<decltype(simple), int>, "fail");
    static_assert(Predicate<Func, double>, "fail");
    static_assert(Predicate<decltype(vector_pred), std::vector<int>>, "fail");
    static_assert(!Predicate<Int, int>, "fail");
    static_assert(!Predicate<std::hash<std::string>, std::string>, "fail");
    static_assert(!Predicate<decltype(vector_pred), int>, "fail");
}

void IndexableTest() {
    static_assert(Indexable<std::vector<int>>, "fail");
    static_assert(Indexable<std::map<int, std::string>>, "fail");
    static_assert(!Indexable<std::map<std::string, int>>, "fail");
    static_assert(!Indexable<std::list<int>>, "fail");
    static_assert(Indexable<std::bitset<32>>, "fail");
    int* check[1];
    static_assert(Indexable<decltype(check)>, "fail");

    struct MyWrapper {
        std::vector<std::string> names;

        std::string& operator[](size_t ind) {
            return names[ind];
        }
    };
    static_assert(Indexable<MyWrapper>, "fail");
    static_assert(!Indexable<int>, "fail");
    static_assert(Indexable<int*>, "fail");

    struct Bad {
        std::vector<int> nums;

    private:
        int& operator[](size_t ind) {
            return nums[ind];
        }
    };

    static_assert(!Indexable<Bad>, "fail");

    struct AnotherBad {
        void operator[](size_t) {
            // need to return something
        }
    };
    static_assert(!Indexable<AnotherBad>, "fail");
    static_assert(!Indexable<std::set<int>>, "fail");
}
