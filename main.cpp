#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>

#define DEFAULT_D 100

using namespace std;

int main(int argc, char* argv[]) {
  int d = argc > 1 ? atoi(argv[1]) : DEFAULT_D;

  mpf_set_default_prec((mp_bitcnt_t) ceil(log2(10)) * d);

  vector<mpq_class> numbers;
  string input;

  while (cin >> input) {
    mpq_class number(input);
    number.canonicalize();
    numbers.push_back(number);
  }

  sort(numbers.begin(), numbers.end());

  return 0;
}
