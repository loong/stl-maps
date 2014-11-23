/// \todo Make number of buckets variable!
#define NO_BUCKETS 389 //10 000th prime

#include <cstddef>
#include <sstream>
#include <list>

using namespace std;

template <class Key, class T>
class hashtablemap
{
  typedef hashtablemap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef T                  mapped_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;
  
  typedef list<value_type>   bucket_type;
  
  bucket_type buckets[NO_BUCKETS];
  
public:
  class iterator {
    typedef input_iterator_tag iterator_category;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    
    pointer* keypair_m;

    iterator(pointer* kp = NULL) : keypair_m(kp) {}
    iterator(const iterator& x) : keypair_m(x.keypair_m) {}
    
    iterator& operator=(iterator& x) {
      keypair_m = x.keypair_m;
      return *this;
    }

    reference operator*() {
      return *keypair_m;
    }

    pointer operator->() {
      return keypair_m;
    }

    bool operator==(const iterator& x) {
      return (keypair_m == x.keypair_m);
    }

    bool operator!=(const iterator& x) {
      return (keypair_m != x.keypair_m);
    }

    iterator& operator++() {}

    iterator operator++(int) {}

  };

  class const_iterator {
    // like iterator, but points to a const
  };

public:
  // default constructor to create an empty map
  hashtablemap() {}

  // overload copy constructor to do a deep copy
  hashtablemap(const Self& x) {}

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {}

  // accessors:
  iterator begin() {}
  const_iterator begin() const {}
  iterator end() {}
  const_iterator end() const {}
  bool empty() const {}
  size_type size() const {}

  // insert/erase
  pair<iterator,bool> insert(const value_type& x) {}
  void erase(iterator pos) {}
  size_type erase(const Key& x) {}
  void clear() {}

  // map operations:
  iterator find(const Key& x) {}
  const_iterator find(const Key& x) const {}
  size_type count(const Key& x) const {}
  T& operator[](const Key& k) {}

private:
  int _hash(Key k) {
    stringstream ss;
    ss << k;

    const char* str = ss.str().c_str();

    int hash = 0;

    for (int i = 0; str[i] != 0; i++){
      hash = hash + (( (int) str[i] * 128^i ) % NO_BUCKETS ) % NO_BUCKETS;
    }

    cout << "DEBUG: hash: " << hash << endl;

    return hash;
  }

};
