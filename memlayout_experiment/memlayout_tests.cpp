

struct MyStruct {
  int field_a = 22;
  double field_b;
  // Silly comment
};

int g = 22;
int f(int x) { return x + g; }
int main() { return f(1); }
