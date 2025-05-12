 #include <catch.hpp>
 #include <unistd.h>
 #include <vector>
 #include <random>
 #include <algorithm>
 #include <static_stl_allocator.h>

 static const size_t kMaxSize = 1e6;
 static char kMempool[kMaxSize];

 TEST_CASE("basic") {
     StaticStlAllocator<int> alloc(kMempool, kMaxSize);
     Set<int> values(alloc);
     auto border = sbrk(0);
     for (int i = 5; i >= 0; --i) {
         values.insert(i);
     }
     for (int i = 0; i <= 5; ++i) {
         REQUIRE(values.count(i) == 1);
     }
     values.clear();
     REQUIRE(border == sbrk(0));
 }

 struct Item {
     alignas(16) int x;
     static int destructed;

     bool operator>(Item rhs) const {
         return x > rhs.x;
     }
     ~Item() {
         ++destructed;
     }
 };
 int Item::destructed = 0;

 TEST_CASE("stress") {
     const int elems = 10000;
     StaticStlAllocator<Item> alloc(kMempool, kMaxSize);
     Set<Item, std::greater<Item>> s(alloc);
     std::mt19937 gen(63275);
     std::vector<int> perm(elems);
     for (int i = 0; i < elems; ++i) {
         perm[i] = i;
     }
     std::shuffle(perm.begin(), perm.end(), gen);

     auto border = sbrk(0);
     for (int i = 0; i < elems; ++i) {
         s.insert(Item{i});
     }
     Item::destructed = 0;
     for (int i = 0; i < elems; ++i) {
         REQUIRE(s.begin()->x == elems - i - 1);
         s.erase(s.begin());
     }
     REQUIRE(s.empty());
     REQUIRE(Item::destructed == elems);
     REQUIRE(border == sbrk(0));
 }
