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

  ~bstmap() {
    clear();
  }

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
    pair<Node*, bool> res =_find(x.first, root_m);

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

  void erase(iterator pos) {}
  size_type erase(const Key& x) {}
  
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
    
  }


  value_type& min() {
    return _leftmost_node()->value_m;
  }

  value_type& max() {
    return _rightmost_node()->value_m;
  }
  
private:

  /**
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

  void _recursive_delete(Node* n) {
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

  size_type _size(Node* n) const {
    if (n == NULL) {
      return 0;
    }

    return 1 + _size(n->left_m) + _size(n->right_m);
  }
};
