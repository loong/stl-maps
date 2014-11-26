# STL Maps

This are implementations of STL container compatible maps. These containers can have the same interface as STL's 
map implementation, hence can be used as a free interchangable blackbox.

# Implementations
Two implementations of a map container are in this repo:

 * BST Map - values are stored as nodes in a binary search tree, can be hence accessed in ~log(n) time
 * Hashtable Map - values are stored in buckets, to find the buckets a constant hash time is needed. Special 
   remarks: A binary tree search is done for the case that several values fall into the same bucket

# Other remarks
This project is strictly following STL idioms.

# License
This project is license under WTFPL \< http://www.wtfpl.net/ >
