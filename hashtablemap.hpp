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

    /**
   * \class Node
   *
   * \brief datastructure which holds the data
   */
  class Node {
  public:
    Node(value_type val) : value_m(val) {}
    value_type value_m;
  };

  typedef bstmap<Key, Node*> bucket_type;

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
    Node* node_m;
    const hashtablemap* table_m;

    _iterator(Node* n, const hashtablemap* ht) 
      : node_m(n), table_m(ht) {}
    
    _iterator(const _iterator& x) 
      : node_m(x.node_m), table_m(x.table_m) {}
    
    _iterator& operator=(_iterator& x) {
      node_m = x.keypair_m;
      return *this;
    }

    _iterator& operator=(const _iterator& x) {
      node_m = x.node_m;
      return *this;
    }

    reference operator*() {
      return node_m->value_m;
    }

    pointer operator->() {
      return &(node_m->value_m);
    }

    bool operator==(const _iterator& x) {
      return (node_m == x.node_m);
    }

    bool operator!=(const _iterator& x) {
      return (node_m != x.node_m);
    }

    _iterator& operator++() {}

    _iterator operator++(int) {}

  };

  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  // default constructor to create an empty map
  hashtablemap() : size_m(0) {
  //    buckets_m[NO_BUCKETS].insert(pair<key_type, mapped_type>(key_type(), mapped_type()));
  }

  // overload copy constructor to do a deep copy
  hashtablemap(const Self& x) : size_m(0) {}

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {}

  // accessors:
  iterator begin() {
    if (empty()) {
      return end();
    }

    bucket_type next = _find_next_nonempty_bucket(0);
    return iterator((*next.begin()).second, this);
  }
  
  const_iterator begin() const {
    
  }
  
  iterator end() {
    return iterator(NULL, this);
  }
  const_iterator end() const {
    return const_iterator(NULL, this);
  }
  
  bool empty() const {
    return (size_m == 0);
  }

  size_type size() const {
    return size_m;
  }

  // insert/erase
  pair<iterator, bool> insert(const value_type& x) {
    iterator it = find(x.first);

    // element exists already
    if (it != end()) {
      return pair<iterator, bool>(iterator(it.node_m, this), false);
    }

    // insert new node
    int hash = _hash(x.first);
    Node* new_node = new Node(x);
      
    const pair<Key, Node*> keypair(x.first, new_node);
    buckets_m[hash].insert(keypair);

    ++size_m;

    return pair<iterator, bool>(iterator(new_node, this), true);
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
      //      cout << "not found" << endl;
      return end();
    }

    //cout << "found" << endl;
    return iterator((*it).second, this);
  }

  const_iterator find(const Key& x) const {
    int hash = _hash(x);
    
    bucket_type b = buckets_m[hash];
    typename bucket_type::iterator it = b.find(x);

    if (it == b.end()) {
      return end();
    }

    return const_iterator((*it).second, this);
  }


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

  bucket_type _find_next_nonempty_bucket(int const curr_index) const {
    int i = curr_index;

    for (int i = curr_index; i < sizeof(buckets_m); ++i) {
      if (!buckets_m[i].empty()) {
	return buckets_m[i];
      }
    }
    
    return buckets_m[NO_BUCKETS]; // similarly, end()
  }

};
