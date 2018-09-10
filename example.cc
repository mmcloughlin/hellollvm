#include <iostream>

__attribute__((annotate("foo"))) int add(int a, int b) { return a + b; }

int main(void) { std::cout << add(2, 3) << std::endl; }
