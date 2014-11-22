#include "bstmap.hpp"

#include <map>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

#ifndef USE_STD
typedef bstmap<int, string> Map;
#else
typedef map<int, string> Map;
#endif

int main() {
  Map* map = new Map();
  map->insert(pair<int, string>(10, "ten"));
  map->insert(pair<int, string>(2, "two"));
  map->insert(pair<int, string>(1, "one"));
  map->insert(pair<int, string>(3, "three"));

  /// \todo here is still a hidden error la
  pair<Map::iterator, bool> ret;
  ret = map->insert(pair<int, string>(12, "twelve"));

  cout << "insert new\t";
  cout << ret.second << endl;
  cout << "insert again\t";
  cout << map->insert(pair<int, string>(3, "asdf")).second << endl;

#ifndef USE_STD
  cout << "Min:\t" << map->min().first << endl;
  cout << "Max:\t" << map->max().first << endl;
#endif
  
  cout << "map size  " << map->size() << endl;
  cout << "is empty? " << map->empty() << endl;

  cout << "Find 10:\t" << map->find(10)->second << endl;
  cout << "Find 3:\t\t" << map->find(3)->second << endl;
  cout << "Can find 4?\t"  << (map->find(4) == map->end()) << endl;

  cout << "-------------------" << endl;
  Map map2(*map); // Copy constructor
  
  cout << "Find2 10:\t"    << map2.find(10)->second << endl;
  cout << "Find2 3:\t"     << map2.find(3)->second << endl;
  cout << "Can find2 4?\t" << (map2.find(4) == map2.end()) << endl;

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

  del_map.clear();

  cout << "is empty?  " << del_map.empty() << endl;
  cout << "size       " << del_map.size() << endl;
  //cout << "min / max  " << del_map.min().second << " " << del_map.max().first << endl;
  cout << "-------------------" << endl;
  
  bstmap<char,int> first;
  bstmap<char,int> second;

  first['x']=8;
  first['y']=16;
  first['z']=32;

  second=first;              // second now contains 3 ints
  first=bstmap<char,int>();  // and first is now empty

  cout << "Size of first: " << first.size() << '\n';   // prints 0
  cout << "Size of second: " << second.size() << '\n'; // prints 3

  cout << "-------------------" << endl;
}
