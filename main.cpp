#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>

#define DEFAULT_D 9999

using namespace std;

int main(int argc, char* argv[]) {
  int d = argc > 1 ? atoi(argv[1]) : DEFAULT_D;

  mpf_set_default_prec((mp_bitcnt_t) ceil(log2(10)) * d);

  vector<mpz_class> numbers;
  string input;

  while (cin >> input) {
    mpz_class number(input);
    numbers.push_back(number);
  }

  return 0;
}
