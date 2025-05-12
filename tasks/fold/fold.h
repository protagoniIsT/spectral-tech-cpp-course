#pragma once

struct Sum {
    int operator()(int a, int b) const {
        return a + b;
    }
};

struct Prod {
    int operator()(int a, int b) const {
        return a * b;
    }
};

struct Concat {
    std::vector<int> operator()(std::vector<int> v1, std::vector<int> v2) {
        v1.insert(v1.end(), v2.begin(), v2.end());
        return v1;
    }
};

class Length {
public:
    Length(int* cnt) : cnt_(cnt) {}

    template<typename T, typename U>
    T operator()(T val, const U&) {
        ++(*cnt_);
        return val;
    }

private:
    int* cnt_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    Iterator it;
    T res = init;
    for (it = first; it != last; it++) {
        res = func(res, *it);
    }
    return res;
}
