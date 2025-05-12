#include <vector>
#include <iostream>
#include <csignal>

void handler(int) {
    std::cout << "ABORT" << std::endl;
}

int main() {
    signal(SIGABRT, handler);
    std::vector<int> a(10);
    for (int i = 0; i <= 10; ++i) {
        a[i] = i;
    }
    int sum = 0;
    for (auto x : a) {
        sum += x;
    }
    std::cout << sum << std::endl;
    return 0;
}
