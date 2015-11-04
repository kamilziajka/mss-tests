#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>

#define DEFAULT_D 9999

using namespace std;

int main() {
unsigned int d = argc > 1 ? atoi(argv[1]) : DEFAULT_D;

    cout << "Hello, World!" << endl;
    return 0;
}