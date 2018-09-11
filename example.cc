#include <iostream>

int add(int a, int b) { return a + b; }

__attribute__((annotate("foo"))) void annotated(void) {
  std::cout << "Hello from foo!" << std::endl;
}

void empty(void) {}

int abs(int a) {
  if (a < 0) {
    return -a;
  }
  return a;
}

bool contains(int t, int *x, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (x[i] == t) {
      return true;
    }
  }
  return false;
}

int main(void) { std::cout << add(2, abs(-3)) << std::endl; }
