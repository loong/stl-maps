#include "bstmap.hpp"

#include <iostream>
#include <string>
#include <utility>

using namespace std;

int main() {
  bstmap<string, int> map;
  map.insert(pair<string, int>("eins", 1));
  map.insert(pair<string, int>("HALLO", 2));
  map.insert(pair<string, int>("BAIBAI", 3));

  cout << map.min().first << endl;
  cout << map.min().second << endl;
  cout << map.size() << endl;

  bstmap<string, int>::iterator it;
}
