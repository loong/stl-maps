#include <utility>
#include <cstddef>
#include <iterator>
#include <iostream>

#include <cassert>

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

    value_type value_m;
    Node* parent_m;
    Node* left_m;
    Node* right_m;
  };

  Node* root_m;

public:

  /// can't figure out how to get rid of the second one 
  template <typename _T>
  class _iterator {
  public:
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

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
      node_m = _successor(node_m);
      return *this;
    }

    _iterator operator++(int) {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

    Node* node_m;
  };
  
  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  // default constructor to create an empty map
  bstmap() : root_m(NULL) {}

  ~bstmap() {
    clear();
  }

  // overload copy constructor to do a deep copy
  bstmap(const Self& x) {
    root_m = x.root_m; // Why doesn't it work without that!?

    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);
    }
  }

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) {
    // guard against self assignment
  }

  // accessors:
  iterator begin() {
    return iterator(_leftmost_node());
  }

  const_iterator begin() const {
    return const_iterator(_leftmost_node());
  }

  iterator end() {
    if (empty()) {
      return begin();
    }

    return iterator(NULL);
  }

  const_iterator end() const {
    if (empty()) {
      return begin();
    }

    return const_iterator(NULL);
  }

  bool empty() const {
    return root_m == NULL;
  }

  size_type size() const {
    return _size(root_m);
  }

  // insert/erase
  pair<iterator, bool> insert(const value_type& x) {
    pair<Node*, bool> res = _find(x.first, root_m);

    if (res.second == true) {  // means value is already in tree
      return pair<iterator, bool>(iterator(res.first), false);
    }
    
    if (res.first == NULL) {
      root_m = new Node(x); // First Node insertaion
      return pair<iterator, bool>(iterator(root_m), true);
    }

    // create new node
    Node* new_node = new Node(x, res.first);
    if (x.first < (res.first->value_m).first) {
      res.first->left_m = new_node;      
    }
    else {
      res.first->right_m = new_node;
    }

    return pair<iterator, bool>(iterator(new_node), true);
  }

  void erase(iterator pos) {
    assert(pos != NULL);

    Node* n = pos.node_m;
    
    if (n == NULL) {
      return;
    }

    // Case 3: x has 2 childs
    if (n->left_m != NULL && n->right_m != NULL) {
      cout << "Case 3" << endl;

      Node* successor = _successor(n);

      // replace to deleted node with successor
      Node* _parent = n->parent_m;
      Node* _left = n->left_m;
      Node* _right = n->right_m;

      delete n;
      n = new Node(successor->value_m, _parent, _left, _right);
      
      // delete original successor
      erase(iterator(successor));

      return;
    }
    
    // Case 1: x is a leaf
    if (n->left_m == NULL && n->right_m == NULL) {
      cout << "Case 1" << endl;

      Node* parent = n->parent_m;

      if (parent->left_m == n) {
	parent->left_m = NULL;
      }
      else {
	parent->right_m = NULL;
      }
      
      return;
    }

    // Case 2: x has exactly one child
    cout << "Case 2" << endl;
    Node* child = n->left_m;
    if (child == NULL) {
      child = n->right_m;
    }

    // special case: deleting root
    Node* parent = n->parent_m;
    if (n == root_m) {
      root_m = child;
    }
    else {
      if (n == parent->left_m) {
	parent->left_m = child;
      }
      else {
	parent->right_m = child;
      }
    }

    delete n;

    return;
  }
  
  size_type erase(const Key& x) {
    iterator it = find(x);

    if (it == end()) { // Key not found
      return 0;
    }

    erase(it);

    return 1; // since Key in maps are unique, can only be 1
  }
  
  void clear() {
    _recursive_delete(root_m);
  }

  // map operations:
  iterator find(const Key& x) {
    pair<Node*, bool> res = _find(x, root_m);

    if (res.second) { // found key
      return iterator(res.first);
    }
    
    return end();
  }

  const_iterator find(const Key& x) const {
    pair<Node*, bool> res = _find(x, root_m);

    if (res.second) { // found key
      return const_iterator(res.first);
    }
    
    return end();
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

  T& operator[](const Key& k) {
    iterator it = find(k);
    
    if (it != end()) {   // found key
      return (*it).second;
    }
    else {               // not found -> create empty
      insert( value_type(k, T()));
      return operator[](k);   // a bit ugly
    }
  }

  value_type& min() {
    return _leftmost_node()->value_m;
  }

  value_type& max() {
    return _rightmost_node()->value_m;
  }
  
public:

  /**
   * \return Node* nearest node to found
   * \return bool true if found
   */
  pair<Node*, bool> _find(const key_type& key, Node* subtree, Node* parent = NULL) const {
    if (subtree == NULL) {
      return pair<Node*, bool>(parent, false);
    }
    
    if (subtree->value_m.first == key) {
      return pair<Node*, bool>(subtree, true);
    }
    else if (subtree->value_m.first > key) {
      return _find(key, subtree->left_m, subtree);
    }
    else {
      return _find(key, subtree->right_m, subtree);
    }
  }

private:

  Node* _leftmost_node() const { return _leftmost_rec(root_m); }

  static Node* _leftmost_rec(Node* subtree) {
    if (subtree != NULL) {
      while (subtree->left_m != NULL) {
	subtree = subtree->left_m;
      }
    }
    return subtree;
  }

  static const Node* _rightmost_node(const Node* subtree) {    
    if (subtree == NULL) {
      while (subtree->right_m != NULL) {
	subtree = subtree->right_m;
      }
    }

    return subtree;
  }

  static Node* _successor(Node* n) {
    if (n->right_m != NULL) {
      return _leftmost_rec(n->right_m);
    }
    else {
      Node* pos = n;
      Node* pos_parent = n->parent_m;
      while (pos_parent != NULL && pos == pos_parent->right_m) {
	pos = pos_parent;
	pos_parent = pos_parent->parent_m;
      }
	
      return pos_parent;
    }
  }

  static void _recursive_delete(Node* n) {
    if (n == NULL) {
      cout << "Logic Error" << endl;
    }

    if (n->left_m != NULL) {
      delete n->left_m;
    }
    
    if (n->right_m != NULL) {
      delete n->right_m;
    } 

    delete n;
  }

  static size_type _size(Node* n) {
    if (n == NULL) {
      return 0;
    }

    return 1 + _size(n->left_m) + _size(n->right_m);
  }

};
