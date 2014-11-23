/// \todo Make number of buckets variable!
#define NO_BUCKETS 389 //10 000th prime

#include <cstddef>
#include <sstream>
#include "bstmap.hpp"

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
  
  typedef bstmap<key_type, mapped_type> bucket_type;
  
  bucket_type buckets_m[NO_BUCKETS+1]; /// the addition one will represent end() \todo
  int size_m;

  ////////////////////////////////////////////////////////////////////////////////
  /// Iterators
  ////////////////////////////////////////////////////////////////////////////////
public:
  /**
   * \brief Template in order to create iterator and const_iterator
   *        without copy pasting
   */
  template <typename _T>
  class _iterator {
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef const value_type* pointer;
    typedef const value_type& reference;


    friend class hashtablemap;
    
  public:
    const value_type keypair_m;
    const int bucket_m;

    const hashtablemap* table_m;

    _iterator(const value_type kp, int b, const hashtablemap* ht) 
      : keypair_m(kp), bucket_m(b), table_m(ht) {}
    
    _iterator(const _iterator& x) 
      : keypair_m(x.keypair_m), bucket_m(x.bucket_m), table_m(x.table_m) {}
    
    _iterator& operator=(_iterator& x) {
      keypair_m = x.keypair_m;
      return *this;
    }

    reference operator*() {
      return keypair_m;
    }

    pointer operator->() {
      return &keypair_m;
    }

    bool operator==(const _iterator& x) {
      return (keypair_m == x.keypair_m);
    }

    bool operator!=(const _iterator& x) {
      return (keypair_m != x.keypair_m);
    }

    _iterator& operator++() {}

    _iterator operator++(int) {}

  };

  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  // default constructor to create an empty map
  hashtablemap() : size_m(0) {}

  // overload copy constructor to do a deep copy
  hashtablemap(const Self& x) :size_m(0) {}

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {}

  // accessors:
  iterator begin() {}
  const_iterator begin() const {}
  iterator end() {
    return iterator(value_type(key_type(), mapped_type()), NO_BUCKETS+1, this);
  }
  const_iterator end() const {
    return const_iterator(value_type(key_type(), mapped_type()), NO_BUCKETS+1, this);
  }
  
  bool empty() const {
    return (size_m == 0);
  }

  size_type size() const {
    return size_m;
  }

  // insert/erase
  pair<iterator, bool> insert(const value_type& x) {
    int hash = _hash(x.first);
    
    pair<typename bucket_type::iterator, bool> res = buckets_m[hash].insert(x);

    if (res.second) {
      ++size_m;
    }

    return pair<iterator, bool>(iterator(x, hash, this), res.second);
  }

  void erase(iterator pos) {}
  size_type erase(const Key& x) {}
  void clear() {}

  // map operations:
  iterator find(const Key& x) {
    int hash = _hash(x);
    
    bucket_type b = buckets_m[hash];
    typename bucket_type::iterator it = b.find(x);

    if (it == b.end()) {
      cout << "not found" << endl;
      return end();
    }

    cout << "found" << endl;
    return iterator(*it, hash, this);
  }
  const_iterator find(const Key& x) const {}
  size_type count(const Key& x) const {}
  T& operator[](const Key& k) {}

private:
  static int _hash(Key k) {
    stringstream ss;
    ss << k;

    const char* str = ss.str().c_str();
    int hash = 0;

    for (int i = 0; str[i] != 0; ++i){
      hash = (hash + (( (int) str[i] * 128^i ) % NO_BUCKETS) ) % NO_BUCKETS;
    }

    cout << "\tDEBUG: hash: " << hash << endl;

    return hash;
  }

};
