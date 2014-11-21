#include "bstmap.hpp"

#include <iostream>
#include <string>
#include <utility>

using namespace std;

typedef bstmap<int, string> Map;

int main() {
  Map map;
  map.insert(pair<int, string>(10, "ten"));
  map.insert(pair<int, string>(2, "two"));
  map.insert(pair<int, string>(1, "one"));
  map.insert(pair<int, string>(3, "three"));
  map.insert(pair<int, string>(12, "twelve"));

  cout << map.insert(pair<int, string>(3, "asdf")).second << endl;

  cout << map.min().first << endl;
  cout << map.min().second << endl;
  cout << map.size() << endl;

  cout << "Find 10:\t" << map.find(10)->second << endl;
  cout << "Find 3:\t" << map.find(3)->second << endl;
  cout << "Find 4:\t"  << (map.find(4) == map.end()) << endl;

  Map map2(map); // Copy constructor
  
  cout << "Find2 10:\t" << map2.find(10)->second << endl;
  cout << "Find2 3:\t" << map2.find(3)->second << endl;
  cout << "Find2 4:\t"  << (map2.find(4) == map.end()) << endl;

  map2[11] = "eleven";

  cout << "map[2]\t"  << map2[2]  << endl;
  cout << "map[11]\t" << map2[11] << endl;

  cout << "-----------------" << endl;
  
  Map del_map;

  del_map.insert(pair<int, string>(10, "ten"));
  del_map.insert(pair<int, string>(5, "five"));
  del_map.insert(pair<int, string>(2, "two"));
  del_map.insert(pair<int, string>(13, "thirteen"));
  del_map.insert(pair<int, string>(3, "three"));
  del_map.insert(pair<int, string>(2, "two"));
  del_map.insert(pair<int, string>(11, "eleven"));
  del_map.insert(pair<int, string>(7, "seven"));

  del_map.erase(10);
  del_map.erase(2);

  for (Map::iterator i = del_map.begin(); i != del_map.end(); ++i) {
    cout << "del map\t" << (*i).second << endl;
  }
}
