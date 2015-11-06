#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>

using namespace std;

#define DEFAULT_D 100

void print_mpf(mpf_class number, const unsigned int d) {
  mp_exp_t exp;
  string result = number.get_str(exp);

  // adding leading zeroes
  if (exp < 0) {
    result = string(-exp, '0').append(result);
    exp = 0;
  }
  // adding trailing zeroes
  else if (result.size() < exp) {
    result = result.append(string(exp - result.size(), '0'));
    exp = result.size();
  }

  // adding point on the front
  if (exp == 0) {
    result = string("0.").append(result);
    exp = 1;
  }
  // adding point in the middle
  else if (exp < result.size()) {
    result = result
      .substr(0, exp)
      .append(".")
      .append(result.substr(exp, result.size()));
  }

  // rounding the number
  if (result.size() - exp > d) {
    result = result.substr(0, exp + d + 1);
  }

  cout << result << endl;
}

void print_mpq(mpq_class number, const unsigned int d) {
  print_mpf(mpf_class(number), d);
}

void kolmogorov(vector<mpq_class> &numbers, const unsigned int d) {
  mpq_class kPlusMax;
  mpq_class kMinusMax;
  unsigned long int n = numbers.size();

  {
    unsigned long int j, jBest = 0;
    for (j = 1; j <= n; j++) {

      mpq_class &FXjn = numbers[j - 1];
      mpq_class kPlusCandidate = ((mpq_class) j / (mpq_class) n) - FXjn;
      if (j == 1 || kPlusMax < kPlusCandidate) {
        kPlusMax = kPlusCandidate;
      }
      mpq_class kMinusCandidate = FXjn - (((mpq_class) j - 1) / (mpq_class) n);
      if (j == 1 || kMinusMax < kMinusCandidate) {
        kMinusMax = kMinusCandidate;
      }
    }
  }

  print_mpq(kPlusMax, d);
  print_mpq(kMinusMax, d);
}

class ChiSquaredTest {
public:
  ChiSquaredTest(vector<mpq_class> numbers) : numbers(numbers) {
    k = 10;
  }

  void execute(const unsigned int d) {
    compute_y();
    print_mpq(compute_v(), d);
  }

protected:
  vector<mpq_class> numbers;
  vector<unsigned long int> y;
  unsigned int k;

  mpq_class compute_v() {
    mpq_class n = numbers.size();
    mpq_class V = 0;

    for (int i = 1; i <= k; i++) {
      mpq_class nPi = n * compute_p_i(i);
      mpq_class up = y[i] - nPi;
      V += (up * up) / nPi;
    }

    return V;
  }

  virtual void compute_y() {
    y.assign(k + 1, 0);

    int i = 1;
    mpq_class bound = compute_a_i(i);
    for (mpq_class number : numbers) {
      while (number.get_d() >= bound.get_d()) {
        i++;
        bound = compute_a_i(i);
      }

      if (i > k) {
        break;
      }

      y[i]++;
    }
  }

  mpq_class compute_a_i(int i) {
    return (mpq_class(i) * mpq_class(i)) / mpq_class(100);
  }

  virtual mpq_class compute_p_i(int i) {
    return compute_a_i(i) - compute_a_i(i - 1);
  }
};

class PokerTest: public ChiSquaredTest {
public:
  PokerTest(vector<mpq_class> numbers) : ChiSquaredTest(numbers) {
    k = 7;
  }

protected:
  virtual void compute_y() {
    y.assign(7, 0);

    for (unsigned int i = 0; i + 4 < numbers.size(); i += 5) {
      y[whichHand(i)]++;
    }
  }

  virtual mpq_class compute_p_i(int i) {
    mpq_class m(numbers.size());
    mpq_class result;

    switch (i) {
      case pair:
        result = (10 * m * (m - 1) * (m - 2) * (m - 3)) / (m * m * m * m * m); break;
      case twoPair:
        result = (15 * m * (m - 1) * (m - 2)) / (m * m * m * m * m); break;
      case three:
        result = (10 * m * (m - 1) * (m - 2)) / (m * m * m * m * m); break;
      case full:
        result = (10 * m * (m - 1)) / (m * m * m * m * m); break;
      case four:
        result = (5 * m * (m - 1)) / (m * m * m * m * m); break;
      case same:
        result = 1 / (m * m * m * m); break;
      default:
        result = (m * (m - 1) * (m - 2) * (m - 3) * (m - 4)) / (m * m * m * m * m); break;
    }

    return result;
  }

private:
  enum Hand {
    nothing = 0, pair = 1, twoPair = 2, three = 3, full = 4, four = 5, same = 6
  };

  Hand whichHand(const unsigned int n) {
    if (numbers[n] == numbers[n + 1] && numbers[n + 1] == numbers[n + 2] && numbers[n + 2] == numbers[n + 3]
        && numbers[n + 3] == numbers[n + 4]) {
      return same;
    }

    if ((numbers[n] == numbers[n + 1] && numbers[n + 1] == numbers[n + 2] && numbers[n + 2] == numbers[n + 3])
        || (numbers[n + 1] == numbers[n + 2] && numbers[n + 2] == numbers[n + 3]
            && numbers[n + 3] == numbers[n + 4])) {
      return four;
    }

    if ((numbers[n] == numbers[n + 1] && numbers[n + 1] == numbers[n + 2] && numbers[n + 3] == numbers[n + 4])
        || (numbers[n] == numbers[n + 1] && numbers[n + 2] == numbers[n + 3]
            && numbers[n + 3] == numbers[n + 4])) {
      return full;
    }

    if ((numbers[n] == numbers[n + 1] && numbers[n + 1] == numbers[n + 2])
        || (numbers[n + 1] == numbers[n + 2] && numbers[n + 2] == numbers[n + 3])
        || (numbers[n + 2] == numbers[n + 3] && numbers[n + 3] == numbers[n + 4])) {
      return three;
    }

    if ((numbers[n] == numbers[n + 1] && numbers[n + 2] == numbers[n + 3])
        || (numbers[n] == numbers[n + 1] && numbers[n + 3] == numbers[n + 4])
        || (numbers[n + 1] == numbers[n + 2] && numbers[n + 3] == numbers[n + 4])) {
      return twoPair;
    }

    if (numbers[n] == numbers[n + 1] || numbers[n + 1] == numbers[n + 2] || numbers[n + 2] == numbers[n + 3]
        || numbers[n + 3] == numbers[n + 4]) {
      return pair;
    }

    return nothing;
  }
};

int main(int argc, char* argv[]) {
  const unsigned int d = argc > 1 ? atoi(argv[1]) : DEFAULT_D;

  mpf_set_default_prec((mp_bitcnt_t) ceil(log2(10)) * d);

  vector<mpq_class> numbers;
  string input;

  while (cin >> input) {
    mpq_class number(input);
    number.canonicalize();
    numbers.push_back(number);
  }

  sort(numbers.begin(), numbers.end());

  ChiSquaredTest chiSquaredTest(numbers);
  chiSquaredTest.execute(d);

  kolmogorov(numbers, d);

  PokerTest pokerTest(numbers);
  pokerTest.execute(d);

  return 0;
}
