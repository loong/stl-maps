#include "bstmap.hpp"

#include <iostream>
#include <string>
#include <utility>

using namespace std;

int main() {
  bstmap<string, int> map;
  map.insert(pair<string, int>("eins", 1));

  cout << "Hello World" << endl;
}
