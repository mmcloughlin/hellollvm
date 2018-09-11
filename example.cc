#include <iostream>

// add adds two numbers.
int add(int a, int b) { return a + b; }

// annotated demonstrates function annotation.
__attribute__((annotate("foo"))) void annotated(void) {
  std::cout << "Hello from foo!" << std::endl;
}

// empty is completely void.
void empty(void) {}

// abs demonstrates multiple return statements.
int abs(int a) {
  if (a < 0) {
    return -a;
  }
  return a;
}

// contains checks if t is in x. Another example of multiple return statements.
bool contains(int t, int *x, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (x[i] == t) {
      return true;
    }
  }
  return false;
}

// dist computes the absolute value of the difference between a and b. This was
// an attempt to produce a tail call.
int dist(int a, int b) { return abs(a - b); }

// fib computes the nth Fibonacci number.
int fib(int n) {
  if (n < 2) {
    return 1;
  }
  return fib(n - 2) + fib(n - 1);
}

// gcd computes the greatest common divisor of a and b.
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

// tailcall is an artificial attempt to produce a tail call.
int tailcall(int (*f)(int)) { return f(-42); }

// verify executes an assertion as a poor-man's unit test.
#define verify(test)                                                           \
  do {                                                                         \
    if (test) {                                                                \
      std::cerr << "pass\t" << #test << std::endl;                             \
    } else {                                                                   \
      std::cerr << "FAIL\t" << #test << std::endl;                             \
      return 1;                                                                \
    }                                                                          \
  } while (0)

int main(void) {
  if (add(3, 3) == 9) {
    std::cerr << "early exit for the mutation example" << std::endl;
    return 0;
  }

  verify(add(2, 5) == 7);

  verify(abs(-3) == 3);
  verify(abs(10) == 10);

  verify(dist(3, 5) == 2);
  verify(dist(5, 3) == 2);

  verify(fib(1) == 1);
  verify(fib(5) == 8);

  verify(gcd(54, 24) == 6);

  verify(tailcall(abs) == 42);

  return 0;
}
