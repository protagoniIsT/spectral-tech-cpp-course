 #include <catch.hpp>
 #include <static_allocator.h>

 #include <unistd.h>
 #include <string>
 #include <random>
 #include <algorithm>

 TEST_CASE("basic") {
     StaticAllocator<std::string> alloc(3);
     auto s1 = alloc.New(5, 'a');
     auto s2 = alloc.New("hello");
     auto s3 = alloc.New(*s2);
     REQUIRE("aaaaa" == *s1);
     REQUIRE("hello" == *s2);
     REQUIRE(*s2 == *s3);
     auto s4 = alloc.New();
     REQUIRE(s4 == nullptr);
     alloc.Delete(s4);
     alloc.Delete(s2);
     s4 = alloc.New();
     REQUIRE(s4->empty());
 }

 struct X {
     int x;
     static int destructed;
     explicit X(int _x) : x(_x) {}
     ~X() {
         ++destructed;
     }
 };
 int X::destructed = 0;

 TEST_CASE("delete_ctor") {
     {
         StaticAllocator<X> alloc(10);
         std::vector<X*> ptrs;
         for (int i = 0; i < 10; ++i) {
             auto ptr = alloc.New(i);
             REQUIRE(ptr->x == i);
             ptrs.push_back(ptr);
         }
         X::destructed = 0;
         while (!ptrs.empty()) {
             alloc.Delete(ptrs.back());
             ptrs.pop_back();
         }
         REQUIRE(X::destructed == 10);
         for (int i = 0; i < 10; ++i) {
             REQUIRE(alloc.New(i)->x == i);
         }
         X::destructed = 0;
     }
     REQUIRE(X::destructed == 10);
 }

 struct CacheLine {
     alignas(64) char data[20];
 };

 TEST_CASE("align") {
     StaticAllocator<CacheLine> alloc(3);
     for (int i = 0; i < 3; ++i) {
         auto ptr = alloc.New();
         auto iptr = reinterpret_cast<size_t>(ptr);
         REQUIRE(iptr % 64 == 0);
     }
 }

 TEST_CASE("stress") {
     const int elems = 1e5;
     const double delete_prob = 0.5;
     std::mt19937 gen(734785);
     StaticAllocator<int> alloc(elems);
     std::uniform_real_distribution<double> dist;
     std::vector<int*> perm(elems);
     auto border = sbrk(0);

     for (int i = 0; i < elems; ++i) {
         perm[i] = alloc.New(i);
         REQUIRE(*perm[i] == i);
     }

     std::shuffle(perm.begin(), perm.end(), gen);
     for (int i = 0; i < elems / 2; ++i) {
         alloc.Delete(perm[i]);
     }
     for (int i = elems / 2; i < elems; ++i) {
         double p = dist(gen);
         if (p < delete_prob) {
             alloc.Delete(perm[i]);
         } else {
             int elem = *perm[i] + elems;
             REQUIRE(*alloc.New(elem) == elem);
         }
     }

     REQUIRE(border == sbrk(0));
 }
