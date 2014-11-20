#include <utility>
#include <cstddef>
#include <iterator>
#include <iostream>

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
  class Node  {
  public:
    Node (value_type val, Node* parent = NULL,  Node* left = NULL, Node* right = NULL) :
      value_m(val), parent_m(parent), left_m(left), right_m(right) {}

    /// internal implementation
  public:
    value_type value_m;
    Node* parent_m;
    Node* left_m;
    Node* right_m;
  };

  Node* root_m;

public:
  template<typename pointer_T, typename reference_T> 
  /// can't figure out how to get rid of the second one 
  
  class _iterator {
  public:
    typedef input_iterator_tag iterator_category;
    // typedef value_type value_type;
    // typedef dfference_type difference_type;
    typedef pointer_T pointer;
    typedef reference_T reference;

    friend class bstmap;

    _iterator(Node* n = NULL) : node_m(n) {}
    _iterator(const _iterator& x) : node_m(x.node_m) {}
    
    _iterator& operator=(_iterator& x) {
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
      ++(*this);
      return this;
    }

    _iterator& operator++(int) {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

    _iterator& operator--() {
      --(*this);
      return this;
    }
    
    _iterator& operator--(int) {
      _iterator temp = *this;
      --(*this);
      return temp;
    }

    Node* node_m;
  };
  
  typedef _iterator<value_type*, value_type&> iterator;
  typedef _iterator<const value_type*, const value_type&> const_iterator;

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
    return iterator(_leftmost_node());
  }
  const_iterator begin() const {
    //have to return const_iterator
  }
  iterator end() {
    //wraper of null??
    if (empty()) {
      return begin();
    }
    return iterator(NULL);
  }

  const_iterator end() const {
    
  }

  bool empty() const {
    return root_m == NULL;
  }

  size_type size() const {}

  // insert/erase
  pair<iterator, bool> insert(const value_type& x) {
    pair<Node*, bool> res =_find(x, root_m);

    if (res.second == true) {  // means value is already in tree
      return pair<iterator, bool>(iterator(res.first), false);
    }
    
    if (res.first == NULL) {
      root_m = new Node(x); // First Node insertaion
      return pair<iterator, bool>(iterator(root_m), true);
    }

    // create new node
    Node* new_node = new Node(x, res.first);
    if (x.second < (res.first->value_m).second) {
      res.first->left_m = new_node;      
    }
    else {
      res.first->right_m = new_node;
    }

    return pair<iterator, bool>(iterator(new_node), true);
  }

  void erase(iterator pos) {}
  size_type erase(const Key& x) {}
  void clear() {}

  // map operations:
  iterator find(const Key& x) {}
  const_iterator find(const Key& x) const {}
  size_type count(const Key& x) const {}
  T& operator[](const Key& k) {}


  value_type& min() {
    return _leftmost_node()->value_m;
  }

  value_type& max() {
    return _rightmost_node()->value_m;
  }
  
private:
  pair<Node*, bool> _find(const value_type& val, Node* subtree, Node* parent = NULL) const {
    if (subtree == NULL) {
      return pair<Node*, bool>(parent, false);
    }
    
    if (subtree->value_m.second == val.second) {
      return pair<Node*, bool>(subtree, true);
    }
    else if (subtree->value_m.second > val.second) {
      return _find(val, subtree->left_m, subtree);
    }
    else {
      return _find(val, subtree->right_m, subtree);
    }
  }

  Node* _leftmost_node() {
    Node* head = root_m;

    if (head != NULL) {
      while (head->left_m != NULL) {
	head = head->left_m;
      }
    }
    return head;    
  }

  Node* _rightmost_node() {
    Node* head = root_m;
    
    if (head == NULL) {
      while (head->right_m != NULL) {
	head = head->right_m;
      }
    }

    return head;
  }

};
