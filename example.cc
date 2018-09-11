#include <iostream>

int add(int a, int b) { return a + b; }

__attribute__((annotate("foo"))) void annotated(void) {
  std::cout << "Hello from foo!" << std::endl;
}

void empty(void) {}

int main(void) { std::cout << add(2, 3) << std::endl; }
