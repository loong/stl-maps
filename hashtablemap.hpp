/// \todo Make number of buckets variable!
#define NO_BUCKETS 389 //10 000th prime

#include "bstmap.hpp"

#include <sstream>

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

  bucket_type buckets_m[NO_BUCKETS]; /// the addition one will represent end() \todo
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
    typedef value_type* pointer;
    typedef value_type& reference;

    friend class hashtablemap;
    typedef bstmap<Key, Node*> bucket_type;
    
  public:
    _iterator(Node* n = NULL, const hashtablemap* ht = NULL) 
      : node_m(n), table_m(ht) {}
    
    _iterator(const _iterator& x) 
      : node_m(x.node_m), table_m(x.table_m) {}

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

    _iterator& operator++() {
      /// \todo move into helper function
      Node* next_node = NULL;

      Key k = node_m->value_m.first;
      int hash = _hash(k);

      bucket_type curr_bucket = table_m->buckets_m[hash]; 
      typename bucket_type::iterator next = ++curr_bucket.find(k);

      if (next != curr_bucket.end()) {
        node_m = next->second;
	return *this;
      }

      const bucket_type* next_bucket = table_m->_find_next_nonempty_bucket(hash);
      
      if (next_bucket == NULL) {
	node_m = NULL; /// return end()
	return *this;
      }

      node_m = next_bucket->begin()->second;
      return *this;
    }

    _iterator operator++(int) {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

    Node* node_m;
    const hashtablemap* table_m;
  };

  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  // default constructor to create an empty map
  hashtablemap() : size_m(0) {}

  // overload copy constructor to do a deep copy
  hashtablemap(const Self& x) : size_m(0) {
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// insert one by one, which will be aweful for
		       /// the treestructure, unfortunatly not enough
		       /// time to balance tree properly --> Winterproject!
    }
  }

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {
    // guard against self assignment
    if (this == &x) {
      return *this;
    }

    clear();
    
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// same balancing problem as for copy constructor above
    }
  }

  // accessors:
  iterator begin() {
    if (empty()) {
      return end();
    }

    const bucket_type* first_filled_bucket = _find_next_nonempty_bucket(0);
    assert(first_filled_bucket != NULL);

    Node* first_node = first_filled_bucket->begin()->second;
    return iterator(first_node, this);
  }
  
  const_iterator begin() const {
    if (empty()) {
      return end();
    }

    const bucket_type* next = _find_next_nonempty_bucket(0);
    return const_iterator((*next->begin()).second, this);
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

  void erase(iterator pos) {
    // erase from node list
  }
  size_type erase(const Key& x) {}
  
  void clear() {}

  // map operations:
  iterator find(const Key& x) {
    int hash = _hash(x);
    
    bucket_type b = buckets_m[hash];
    typename bucket_type::iterator it = b.find(x);

    if (it == b.end()) {
      return end();
    }

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

  /**
   * \brief Because all elements in a map container are unique, the
   * function can only return 1 (if the element is found) or zero
   * (otherwise).
   */
  size_type count(const Key& x) const {
    if (find(x) != end()) {
      return 1;
    }
    return 0;
  }

  /**
   * \brief subscript operator. Remarks: if Key not available, Key
   *        with default value will be initialised
   */
  T& operator[](const Key& k) {
    iterator it = find(k);
    
    if (it != end()) {            // found key
      return (*it).second;
    }
    else {                        // not found -> create empty
      insert(value_type(k, T()));
      return operator[](k);       // a bit ugly
    }
  }

private:
  static int _hash(Key k) {
    stringstream ss;
    ss << k;

    const char* str = ss.str().c_str();
    int hash = 0;

    for (int i = 0; str[i] != 0; ++i){
      hash = (hash + (( (int) str[i] * 128^i ) % NO_BUCKETS) ) % NO_BUCKETS;
    }

    return hash;
  }

  const bucket_type* _find_next_nonempty_bucket(int const curr_index) const {
    for (int i = curr_index + 1; i < NO_BUCKETS; ++i) {
      if (!buckets_m[i].empty()) {
	return &buckets_m[i];
      }
    }

    return NULL;
  }

};
