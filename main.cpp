#include "bstmap.hpp"

#include <iostream>
#include <string>
#include <utility>

using namespace std;

int main() {
  bstmap<int, string> map;
  map.insert(pair<int, string>(10, "BAIBAI"));
  map.insert(pair<int, string>(2, "eins"));
  map.insert(pair<int, string>(1, "HALLO"));
  map.insert(pair<int, string>(3, ""));
  cout << map.insert(pair<int, string>(3, "")).second << endl;

  cout << map.min().first << endl;
  cout << map.min().second << endl;
  cout << map.size() << endl;

  bstmap<int, int>::iterator it;
}
