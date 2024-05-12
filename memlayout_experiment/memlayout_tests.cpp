

#include <cassert>

#define NOINLINE __attribute__((noinline))

struct MyStruct {
  int field_a = 22;
  double field_b;
  // Silly comment
};

int g = 22;
NOINLINE int f(int x) { return x + g; }

int main() { 

    assert(f(1) == 23);

}
