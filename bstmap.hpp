#include <utility>
#include <cstddef>
#include <iterator>


using namespace std;

template <class Key, class T>
class bstmap
{
  typedef bstmap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;


private:
  class Node;
  Node* root_m;

public:
  class iterator {
    typedef input_iterator_tag iterator_category;
    // typedef value_type value_type;
    // typedef dfference_type difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    friend class bstmap;

    iterator(bstmap* map, Node* n = NULL) : map_m(map), node_m(n) {}
    iterator(const iterator& x) : map_m(x.map_m), node_m(x.node_m) {}
    
    iterator& operator=(iterator& x) {
      map_m = x.map_m;    // maybe redundant
      node_m = x.node_m;
      return *this;
    }

    reference operator*() {
      return node_m->value_m;
    }

    pointer operator->() {
      return &(node_m->value_m);
    }

    bool operator==(const iterator& x) {
      return (node_m == x.node_m);
    }

    bool operator!=(const iterator& x) {
      return (node_m != x.node_m);
    }

    bstmap* map_m;
    Node* node_m;
  };
  
  class const_iterator {
    // like iterator, but points to a const
  };

public:
  // default constructor to create an empty map
  bstmap() : root_m(NULL) {}

  // recursive destruction

  // overload copy constructor to do a deep copy
  bstmap(const Self& x) {
    // use simple for with iterator to insert/ copy the new nodes
  }

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {
    // guard against self assignment
  }

  // accessors:
  iterator begin() {
    // return iterator(this, leftmost_element())
  }
  const_iterator begin() const {
    //have to return const_iterator
  }
  iterator end() {
    //wraper of null??
  }
  const_iterator end() const {}
  bool Empty() const {}
  size_type size() const {}

  // insert/erase
  pair<iterator, bool> insert(const value_type& x) {
    pair<Node*, bool> res =_find(x, root_m);

    if (res.second == true) {  // means value is already in tree
      return pair<iterator, bool>(iterator(this, res.first), false);
    }
    
    // create new node
    Node* new_node = new Node(x, res.first);
    if (x < res.first->value_m) {
      res.first->left_m = new_node;      
    }
    else {
      res.first->right_m = new_node;
    }

    return pair<iterator, bool>(iterator(this, new_node), true);
  }

  void erase(iterator pos) {}
  size_type erase(const Key& x) {}
  void clear() {}

  // map operations:
  iterator find(const Key& x) {}
  const_iterator find(const Key& x) const {}
  size_type count(const Key& x) const {}
  T& operator[](const Key& k) {}

private:

  pair<Node*, bool> _find(value_type& val, Node* subtree, Node* parent = NULL) const {
    if (subtree == NULL) {
      return pair<Node*, bool>(parent, false);
    }
    
    if (subtree->value_m == val) {
      return pair<Node*, bool>(subtree, true);
    }
    else if (subtree->value_m > val) {
      return _find(subtree->parent_m, subtree->left_m, val);
    }
    else {
      return _find(subtree->parent_m, subtree->right_m, val);
    }
  }

};

template <typename key_T, typename mapped_T>
class bstmap<key_T, mapped_T>::Node {
public:
  typedef pair<const key_T, mapped_T> value_type;

  Node (value_type val, Node* parent = NULL,  Node* left = NULL, Node* right = NULL) :
    value_m(val), parent_m(parent), left_m(left), right_m(right) {}

/// internal implementation
public:
  value_type value_m;
  Node* parent_m;
  Node* left_m;
  Node* right_m;
};
