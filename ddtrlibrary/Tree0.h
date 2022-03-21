#ifndef TREE_H
#define TREE_H

#include "DDTIncludes.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {

	template <class treeType> class Tree;

	template <class iteratorExtension> class BTreeIterator_Extension;

	template <class treeType> class RedBlackTreeAdapter;
	template <class elementType> class BinaryTree;

	template <class elementType> class BTree;

	template <class treeNode> class RedBlackTreeNodeAdapter;
	template <class T, class objectTraits = Tracing::std_traits, class ddtTraits = Tracing::std_traits> class BinaryTreeNode;

	template <class T, class Traits = Tracing::std_traits> class BTreeNode;

	const short MAX_TREE_HEIGHT = 30;

	///////////////////////////////////////////////////////////////
	// Tree Nodes
	///////////////////////////////////////////////////////////////
	template <class T, class Traits>
	class BTreeNode {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = Traits::type_num };
		typedef typename Traits::Logger Logger;
		typedef typename Traits::Allocator Allocator;
		typedef typename Traits::template transform<T>::value_type value_type;
		////////////////////////////////////////////////////////////////////////
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef T input_value_type;
		typedef const input_value_type& const_input_reference;

		typedef BTreeNode* element_pointer;
		typedef BTreeNode& element_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;


		// ------------------ Access and Manipulation -----------------

		// Return the number of items in this node.
		long Size() const {
			return _keyCount;
		}

		// Return the $i$-th item.  The value $i$ must be such that
		// $0 \le i < {\tt Size()}$.
		value_type Item (short i) const {
			return _item[i];
		}

		// Return the handle of the $i$-th subtree. The value $i$ must be such that
		// $0 \le i \le {\tt Size()}$.
		element_pointer Subtree (short i) const {
			return _subtree[i];
		}

		// Return the number of keys in the subtree rooted at this node. This
		// method consults an instance variable, and therefore takes constant
		// time; it does not need to  traverse the subtree.
		long SubtreeSize() const {
			return _subtreeSize;
		}

		// Search the node for the given key; return greatest $i$ such that
		// ${\tt key[i]} \le {\tt key}$. Return true if {\tt key[i] $=$ key}, false
		// otherwise.
		bool Search (value_type itm, short& index) const {
			if (!_item)
				return false;

			short i;
			if (_keyCount <= 7) { // Small number of keys, do a linear search
				if (_keyCount == 0) {
					index = -1;
					return false;
				}
				for (i = 0; i < _keyCount; i++) {
					if (_item[i] >= itm) {
						break;
					}
				}
				if (_item[i] == itm) {
					index = i;
					return true;
				}
				else  {
					index = i-1;
					return false;
				}
			}

			// Do a binary search
			short lo = 0, hi = _keyCount-1, mid=0;
			while (lo <= hi) {
				mid = (lo + hi)/2;
				if (_item[mid] == itm) {
					index = mid;
					return true;
				}
				if (_item[mid] < itm) {
					lo = mid+1;
				} else {
					hi = mid-1;
				}
			}
			index = (_item[mid] <= itm) ? (mid) :  mid-1;
			return false;
		}


		short getKeyCount() const {
			return _keyCount;
		}

		bool isLeaf() const {
			return _isLeaf;
		}

		long getSubTreeSize() const {
			return _subtreeSize;
		}

		short getOrder() const {
			return _order;
		}

		pointer getItem() const {
			return _item;
		}

		element_pointer getSubTree(short i) const {
			return _subtree[i];
		}

		// --------------------- End public protocol -----------------------


		// ---------------- Construction and destruction ---------------

		// Constructor: create a node of the B-tree with given order.
		// The constructor is protected, because only BTrees may
		// create new nodes.
		BTreeNode (short order)
			: _keyCount(0),
			_isLeaf(true),
			_subtreeSize(0),
			_item(new value_type[2 * order - 1]),
			_subtree(new element_pointer[2 * order]),
			_order(order)
		{
			memset(_subtree, 0, 2 * order * sizeof(element_pointer));
			memset(_subtree, 0, (2 * order - 1)* sizeof(value_type));
		}

		// Destructor.
		~BTreeNode() {
			delete [] _item;
			delete [] _subtree;
		}


		static element_pointer * subtreeAlloc(int size) {
			return new element_pointer[size];	// this will get the right (ie: local) new[] operator
		}


		void * operator new(const size_t sz)   { return Allocator::malloc(sz); }
		void operator delete(void * p)         { return Allocator::free(p); }
		void * operator new[](const size_t sz) { return Allocator::malloc(sz); }
		void operator delete[](void * p)       { return Allocator::free(p); }
	protected:

		typename Traits::template transform<short>::value_type _keyCount;	// # keys in node
		typename Traits::template transform<bool>::value_type  _isLeaf;		// Is this node a leaf?
		typename Traits::template transform<long>::value_type  _subtreeSize;// # keys in subtree rooted at this node
		typename Traits::template transform<short>::value_type _order;

		typename Traits::template transform<pointer>::value_type _item;		 // Vector of items of type T
		typename Traits::template transform<element_pointer *>::value_type _subtree; // Vector of subtrees

		friend BTree<BTreeNode>;
		//friend BTreeIterator<value_type>;
		// perform housekeeping when node is modified
		void modified() {}

		// Shift all the keys and subtrees, beginning at position {\tt pos}
		// right by the given amount. Note that the subtree to the left of
		// {\tt key[pos]} is {\it also\/} moved.
		void ShiftRightAt (short pos, short amount = 1) {
			short i;
			for (i = _keyCount-1; i >= pos; i--) {
				_item[i+amount] = _item[i];
				_subtree[i+amount+1] = _subtree[i+1];
			}
			_subtree [pos+amount] = _subtree[pos];
			for (i = pos; i < pos+amount; i++) {
				_item[i] = 0;
			}
		}


		void ShiftLeftAt (short pos, short amount = 1) {
			short i;
			for (i = pos; i < _keyCount; i++) {
				_item[i-amount] = _item[i];
				_subtree[i-amount] = _subtree[i];
			}
			// Now move the rightmost subtree
			_subtree [_keyCount-amount] = _subtree[_keyCount];
			for (i = _keyCount-amount+1; i <= _keyCount; i++)
				_subtree[i] = 0;
			for (i = _keyCount-amount; i < _keyCount; i++)
				_item[i] = 0;
			_keyCount -= amount;
		}


		// MoveSubNode: Move {\tt nkeys\/} keys, and their left and right
		// subtrees, beginning from position {\tt pos} in node $x$ into this node
		// beginning at position {\tt ourPos}.
		void MoveSubNode (const element_reference x, short pos, short ourPos, short nkeys) {
			short i, j;
			for (i = ourPos, j = pos; i < ourPos + nkeys; i++, j++) {
				_item[i] = x._item[j];
				_subtree[i] = x._subtree[j];
				x._item[j] = 0;
				x._subtree[j] = 0;
			}
			_subtree[ourPos+nkeys] = x._subtree[pos + nkeys];
		}

	};

	template <class T, class objectTraits, class ddtTraits>
	class BinaryTreeNode {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = ddtTraits::type_num };
		typedef typename ddtTraits::Logger Logger;
		typedef typename ddtTraits::Allocator Allocator;
		typedef typename objectTraits::template transform<T>::value_type value_type;

		typedef typename objectTraits::template transform<BinaryTreeNode*>::value_type element_pointer_unlogged;
		typedef typename objectTraits::template transform<BinaryTreeNode&>::value_type element_reference_unlogged;

		typedef typename ddtTraits::transform<BinaryTreeNode*>::value_type element_pointer;
		typedef typename ddtTraits::transform<BinaryTreeNode&>::value_type element_reference;

		typedef typename ddtTraits::template transform<T*>::value_type value_pointer;

		typedef ddtTraits ddtTraits;
		typedef objectTraits objectTraits;
		////////////////////////////////////////////////////////////////////////
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef T input_value_type;
		typedef const input_value_type& const_input_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		// ---------------- Construction and destruction ---------------

		BinaryTreeNode ()  {
			_left.setData(this);
			_right.setData(this);
			_parent.setData(this);
			_subtreeSize = 0;
			_data = (pointer) Allocator::malloc(sizeof(value_type));
		}
		BinaryTreeNode (const_input_reference data, element_pointer_unlogged sentinel, element_pointer_unlogged parent) {
			_left.setData(sentinel);
			_right.setData(sentinel);
			_parent.setData(parent);
			_subtreeSize = 1;
			_data =  new value_type(data);
		}
		//		BinaryTreeNode (const element_reference_unlogged other) {
		//			_data = other._data;
		//			_left = other._left;
		//			_right = other._right;
		//			_parent = other._parent;
		//			_subtreeSize = other._subtreeSize;
		//		}
		~BinaryTreeNode() {}

		inline pointer getDataPointer() {
			return _data;
		}
		inline const_reference getData() {
			return *_data;
		}
		inline element_pointer_unlogged Left() {
			return _left;
		}
		inline element_pointer_unlogged Right() {
			return _right;
		}
		inline element_pointer_unlogged Parent() {
			return _parent;
		}
		inline size_t SubtreeSize() {
			return _subtreeSize;
		}

		void * operator new(const size_t sz)   { return Allocator::malloc(sz); }
		void operator delete(void * p)         { return Allocator::free(p); }
		void * operator new[](const size_t sz) { return Allocator::malloc(sz); }
		void operator delete[](void * p)       { return Allocator::free(p); }
	protected:
		friend BinaryTree<BinaryTreeNode>;
		pointer _data;
		size_t _subtreeSize;
		element_pointer _left, _right, _parent, _dummy;

	};

	///////////////////////////////////////////////////////////////
	//Tree Node Adapters
	///////////////////////////////////////////////////////////////
	template <class treeNode>
	class RedBlackTreeNodeAdapter : public treeNode {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = treeNode::type_num };
		typedef typename treeNode::Logger Logger;
		typedef typename treeNode::Allocator Allocator;
		typedef typename treeNode::value_type value_type;
		typedef ddtTraits ddtTraits;
		typedef objectTraits objectTraits;
		////////////////////////////////////////////////////////////////////////
		typedef typename treeNode::pointer pointer;
		typedef typename treeNode::const_pointer const_pointer;
		typedef typename treeNode::reference reference;
		typedef typename treeNode::const_reference const_reference;

		typedef typename treeNode::input_value_type input_value_type;
		typedef typename treeNode::const_input_reference const_input_reference;

		typedef typename objectTraits::template transform<RedBlackTreeNodeAdapter*>::value_type element_pointer_unlogged;
		typedef typename objectTraits::template transform<RedBlackTreeNodeAdapter&>::value_type element_reference_unlogged;

		typedef typename ddtTraits::transform<RedBlackTreeNodeAdapter*>::value_type element_pointer;
		typedef typename ddtTraits::transform<RedBlackTreeNodeAdapter&>::value_type element_reference;

		typedef typename treeNode::size_type size_type;
		typedef typename treeNode::difference_type difference_type;

		// ---------------- Construction and destruction ---------------

		RedBlackTreeNodeAdapter () {
			_left.setData(this);
			_right.setData(this);
			_parent.setData(this);
			_subtreeSize = 0;
			_data = (pointer) Allocator::malloc(sizeof(value_type));
			_color = BLACK;
			_subtreeSize = 0;
		}

		RedBlackTreeNodeAdapter (const_input_reference data, element_pointer_unlogged sentinel, element_pointer_unlogged parent) {
			_left.setData(sentinel);
			_right.setData(sentinel);
			_parent.setData(parent);
			_subtreeSize = 1;
			_data =  new value_type(data);
			_color = RED;
		}
		//RedBlackTreeNodeAdapter (const element_reference other) {
		//	_color = other._color;
		//	_key = other._key;
		//	_left = other._left;
		//	_right = other._right;
		//	_parent = other._parent;
		//	_subtreeSize = other._subtreeSize;
		//}

		~RedBlackTreeNodeAdapter() {}

		short Color() {
			return _color;
		}
	protected:
		friend BinaryTree<RedBlackTreeNodeAdapter>;
		friend RedBlackTreeAdapter<BinaryTree<RedBlackTreeNodeAdapter>>;
		element_pointer _left, _right, _parent;
		short _color;

	};

	///////////////////////////////////////////////////////////////
	// Tree Bases
	///////////////////////////////////////////////////////////////
	template <class elementType>
	class BTree {	
		// The Generic templated B-tree

		//There are three classes related to the generic B-tree:
		//@li BTree, a class that encapsulates B-tree algorithms;
		//@li BTreeNode, which defines the structure of a node in a B-tree;
		//@li BTreeIterator, an object that  allows  inspection of the items
		//in a B-tree in ascending order;
		//The  algorithms implemented  here assume that items are stored in the
		//internal nodes  as  well as in  the leaves;  the definitions of the
		//B-tree concepts are based on the ones in Cormen, Leiserson and Rivest's
		//book Introduction to Algorithms.
		//The BTreeNode class encapsulates a single node of the Generic
		//B-tree. It is intended primarily for use by the NodeSpace, not by the
		//user of the GenericBTree class.
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = elementType::type_num };
		typedef typename elementType::Logger Logger;
		typedef typename elementType::Allocator Allocator;
		typedef typename elementType::value_type value_type;
		typedef typename elementType::Traits Traits;
		////////////////////////////////////////////////////////////////////////
		typedef typename elementType::pointer pointer;
		typedef typename elementType::reference reference;
		typedef const typename elementType::const_reference const_reference;

		typedef elementType elementType;

		typedef typename elementType::input_value_type input_value_type;
		typedef typename elementType::const_input_reference const_input_reference;

		typedef typename elementType::element_pointer element_pointer;
		typedef typename elementType::element_reference element_reference;

		typedef typename elementType::size_type size_type;
		typedef typename elementType::difference_type difference_type;

		// --------------------- Construction and destruction ------------------

		// Create a new B-tree of given order. Duplicate items are not
		// allowed. The first parameter specifies the Comparator to be used when
		// comparing two cells. The {\tt order} parameter must be at least 2;
		// anything less than 2 is taken to be 2.
		// The NodeSpace {\tt space} may by created by the derived class and
		// passed to this constructor; if it is NULL, a default in-memory node
		// space is created. If the derived class passes a non-null NodeSpace,
		// it is the responsibility of the derived class to destroy the
		// NodeSpace object.
		BTree (short order = 2) : instance_id_(NewInstance::createId()), _order(order >= 2 ? order : 2),
			_root(NULL) {}
		//BTree (const BTree& other)
		//	: _order(other._order), _root(other._root) {}

		// Destructor: tells the NodeSpace to destroy all the nodes.
		~BTree () {
			//Allocator::delete _root;
		}
		void initialize() {
			_root = NULL;
		}

		// ----------------------- Search and related methods ------------------

		// Search the tree for the given item.
		// Return a pointer to the found item in situ if the search was successful.
		// If the search fails, the return value is NULL.
		// The algorithm used is a standard B-tree search algorithm that takes
		// log_d n time in an n-item B-tree of order d.
		pointer Find(const_input_reference item) const {
			short pos;
			bool found = false;
			pointer ret_val = NULL;

			element_pointer current = _root;
			while (current) {
				found = current->Search (item, pos);
				if (found || current->_isLeaf) break;
				current = current->_subtree [pos+1];
			};

			if (found)
				ret_val = &(current->_item [pos]);
			return ret_val;
		}




		// Find and return the minimum item. If the tree is empty,
		// the null pointer is returned. The implementation simply returns the
		// value {\tt ItemWithRank (0)}. 
		pointer Smallest() const {
			return ItemWithRank (0);
		}

		// Find and return the maximum item. If the tree is empty,
		// the null pointer is returned. The implementation simply returns the
		// value {\tt ItemWithRank (Size()$-$1)}.
		pointer Largest() const {
			return ItemWithRank (Size()-1);
		};

		// Given an index $i$ between 0 and {\tt Size()}$-1$, return the element
		// of rank $i$, i.e., the element that has $i$ elements less than it in the tree.
		// If $i \le 0$, this returns the smallest element, and if $i \ge {\tt
		// Size()}$, this returns the largest element. If the tree is empty,
		// the null value of the base type is returned. The implementation
		// examines only the nodes on the path from the root to the one
		// containing the key sought, and therefore takes no more than $\log
		// n$ time steps with $n$ keys in the tree.
		// Note that it is possible to iterate through the elements of the tree
		// via calls to this method, varying the index from 0 to ${\tt Size()}-1$;
		// however, this is much less efficient than using the BTreeIterator.
		pointer ItemWithRank (long rank) const {
			element_pointer tmp_ptr, p1;
			tmp_ptr = _root;
			if (!tmp_ptr || tmp_ptr->_keyCount <= 0)
				return NULL;

			rank = (rank >? 0) <? (tmp_ptr->_subtreeSize-1);
			do {
				if (tmp_ptr->_isLeaf) {
					assert ((0 <= rank) && (rank <= tmp_ptr->_keyCount-1));
					// ("Internal error: CL_GenericBTree::ItemWithRank:"
					//	"bad key count %d rank %ld", tmp_ptr->_keyCount, rank));
					return &(tmp_ptr->_item[rank]);
				}

				// We're in a non-leaf, so find the subtree to descend into
				// (if any)
				short i;
				for (i = 0; i < tmp_ptr->_keyCount; i++) {
					p1 = tmp_ptr->_subtree[i];
					if (p1->_subtreeSize > rank)
						break;
					rank -= p1->_subtreeSize; // Account for i-th subtree

					if (rank == 0) {
						// We've got the item we wanted
						return &(tmp_ptr->_item[i]);
					}
					rank--;					 // Account for i-th key in node
				}

				if (i >= tmp_ptr->_keyCount) {
					// Descend into rightmost subtree
					p1 = tmp_ptr->_subtree[i];
				}
				tmp_ptr = p1;
			} while (1);
		}


		// Return the number of elements in the tree that are less than the
		// parameter.
		long RankOf (value_type item) const {
			short pos;
			bool found;
			long count = 0;

			element_pointer tmp_ptr, p1;
			tmp_ptr = _root;
			if (!tmp_ptr || tmp_ptr->getKeyCount() <= 0)
				return 0;

			do {
				found = tmp_ptr->Search (item, pos);
				if (tmp_ptr->isLeaf()) {
					count += found ? pos : pos+1;
					return count;
				}

				// We're in a non-leaf, so find the subtree to descend into
				short i;
				for (i = 0; i <= pos; i++) {
					p1 = tmp_ptr->getSubTree(i);
					count += p1->getSubTreeSize(); // Account for i-th subtree
				}

				if (found)  {
					return count + pos;
				}

				count += pos+1; // Account for the keys we compared
				p1 = tmp_ptr->getSubTree(i);
				tmp_ptr = p1;
			} while (1);
		}



		// Return the size of the tree (number of items currently present).
		// The implementation needs constant time regardless of tree size.
		long Size () const {
			return _root ? _root->_subtreeSize : 0;
		}




		// ------------------------ Modification ------------------------------

		// Insert the item to the tree. Return true if successfully added,
		// false if the item was already in the tree.
		element_pointer Insert(value_type item) {
			bool ans;
			element_pointer aNode;
			if (!_root) {
				_root = new elementType(_order);
			}
			if (_root->_keyCount < (2*_order - 1)) {
				ans = _InsertNonFull (_root, item);
				return _root;
			}

			// Root is full; create a new empty root
			aNode = new elementType(_order); // aNode  will be the new root
			aNode->_subtree [0] = _root;
			aNode->_isLeaf = false;
			aNode->_subtreeSize = _root->_subtreeSize;
			aNode->_keyCount = 0;
			_SplitChild (aNode, 0, _root);

			_root = aNode;

			// Now add the key 
			_InsertNonFull (aNode, item);
			return aNode;
		}


		// Remove the specified item from the tree.
		// Return NULL if the item was not found in the tree, and the found item otherwise.
		// The implementation needs (in the worst case) two passes over the path
		// to the key, and so takes $2\log n$ time steps with $n$ keys in the
		// tree.
		// It immediately coalesces any underflowing nodes along the path
		// from the root to the deleted key.
		int Remove (value_type key) {
			element_pointer root = _root;
			element_pointer node = root;
			value_type retVal;
			short sp, index;
			bool found;

			if (!node || node->_keyCount == 0) // Empty root
				return NULL;
			if (node->_keyCount == 1 && node->_isLeaf) {
				// Tree has only one key
				if (key == node->_item[0]) {
					node->_keyCount = node->_subtreeSize = 0;
					/*return node->_item;*/
					return 1;
				}
				return 0;
			}

			element_pointer stack[DDTLibrary::MAX_TREE_HEIGHT];

			// We need a stack for updating the subtree sizes
			sp = 0;
			index = 0;
			found = false;

			element_pointer q;
			// stack[sp++] = node;
			enum {SEARCHING, DESCENDING} state = SEARCHING;
			DeleteActionEnum action;
			while (1) {
				if (state == SEARCHING) {
					found = node->Search (key, index);
					if (found)
						retVal = node->_item[index];
				}
				q = _DescendInto (node, index+1, action);
				if (node == root &&  node->_keyCount == 0) {
					delete node;
				}
				else {
					// We should add the root to the stack only if it wasn't
					// already destroyed
					stack [sp++] = node;
				}
				if (!q) break;

				// _DescendInto may have caused our key to be copied into q.
				// If so, it would be copied into either q->_item[0] or
				// q->_item[_order-1]  (because of a right or left rotation,
				// respectively) or into q->_item[_order-1] (because of a merge).
				if (found) {
					state = DESCENDING;
					if (action == RotateRight) {
						index = 0;
					}
					else if (action == RotateLeft || action == Merge) {
						index = _order-1;
					}
					else // No rotation or merge was done
						break;
				}
				node = q;
			}
			if (!found) {
				// The key is not in the tree
				return 0;
			}
			if (node->_isLeaf) {
				// Key found in leaf
				node->ShiftLeftAt (index+1);
			}
			else {
				// The key is in an internal node, so we'll replace it by its
				// inorder successor:
				element_pointer p = q;
				while (1) {
					stack[sp++] = p;
					if (p->_isLeaf) break;
					p = _DescendInto (p, 0, action);
				}
				node->_item[index] = p->_item[0];
				p->ShiftLeftAt(1);
			}

			updSubtree(stack, sp);

			return 1;
		}


		//int Delete (element_pointer node, short index) {
		// element_pointer root = _root;
		// element_pointer node = root;
		// value_type retVal;
		// short sp, index;
		// bool found;

		// if (!node || node->_keyCount == 0) // Empty root
		//  return NULL;
		// if (node->_keyCount == 1 && node->_isLeaf) {
		//  // Tree has only one key
		//  if (key == node->_item[0]) {
		//   node->_keyCount = node->_subtreeSize = 0;
		//   return node->_item;
		//  }
		//  return NULL;
		// }
		// 
		// element_pointer stack[DDTLibrary::MAX_TREE_HEIGHT];

		// // We need a stack for updating the subtree sizes
		// sp = 0;
		// index = 0;
		// found = false;
		// 
		// element_pointer q;
		// // stack[sp++] = node;
		// enum {SEARCHING, DESCENDING} state = SEARCHING;
		// DeleteActionEnum action;
		// while (1) {
		//  if (state == SEARCHING) {
		//   found = node->Search (key, index);
		//   if (found)
		//	   retVal = node->_item[index];
		//  }
		//  q = _DescendInto (node, index+1, action);
		//  if (node == root &&  node->_keyCount == 0) {
		//   delete node;
		//  }
		//  else {
		//   // We should add the root to the stack only if it wasn't
		//   // already destroyed
		//   stack [sp++] = node;
		//  }
		//  if (!q) break;
		//   
		//  // _DescendInto may have caused our key to be copied into q.
		//  // If so, it would be copied into either q->_item[0] or
		//  // q->_item[_order-1]  (because of a right or left rotation,
		//  // respectively) or into q->_item[_order-1] (because of a merge).
		//  if (found) {
		//   state = DESCENDING;
		//   if (action == RotateRight) {
		//	   index = 0;
		//   }
		//   else if (action == RotateLeft || action == Merge) {
		//	   index = _order-1;
		//   }
		//   else // No rotation or merge was done
		//	   break;
		//  }
		//  node = q;
		// }
		// if (!found) {
		//  // The key is not in the tree
		//  return 0;
		// }
		// if (node->_isLeaf) {
		//  // Key found in leaf
		//  node->ShiftLeftAt (index+1);
		// }
		// else {
		//  // The key is in an internal node, so we'll replace it by its
		//  // inorder successor:
		//  element_pointer p = q;
		//  while (1) {
		//   stack[sp++] = p;
		//   if (p->_isLeaf) break;
		//   p = _DescendInto (p, 0, action);
		//  }
		//  node->_item[index] = p->_item[0];
		//  p->ShiftLeftAt(1);
		// }

		// updSubtree(stack, sp);

		// return 1;
		//}

		// Remove and return the smallest item in the tree. Return NULL if
		// if the tree is empty.
		pointer ExtractMin () {
			element_pointer stack[DDTLibrary::MAX_TREE_HEIGHT];
			// We need a stack for updating the subtree sizes
			short sp = 0;
			element_pointer node = _root;
			if (node->_keyCount == 0)
				return NULL;

			stack[sp++] = node;
			DeleteActionEnum action;
			while (!node->_isLeaf) {
				node = _DescendInto (node, 0, action);
				stack[sp++] = node;
			}
			value_type item = node->_item[0];
			node->ShiftLeftAt(1);
			for (short i = 0; i < sp; i++) {
				stack[i]->_subtreeSize--;
			}
			return &item;
		}

		element_pointer Root () const {
			return _root;
		}

		// --------------------- End public protocol -----------------------

		void * operator new(const size_t sz)   { return Allocator::malloc(sz); }
		void operator delete(void * p)         { return Allocator::free(p); }
		void * operator new[](const size_t sz) { return Allocator::malloc(sz); }
		void operator delete[](void * p)       { return Allocator::free(p); }

	protected:

		//------------------- Protected helper methods ---------------------

		// update subtree sizes along a search path
		void updSubtree(element_pointer * stack, int sp) {
			short i = 0;
			if (stack[0]->_keyCount == 0) {
				i = 1;
			}
			for (; i < sp; i++) {
				stack[i]->_subtreeSize--;
			}
		}



		enum DeleteActionEnum {NoAction, RotateLeft, RotateRight, Merge};

		// Ensure that the node {\tt n1} is non-full, and recurse into it while
		// inserting.
		bool _InsertNonFull (element_pointer x, value_type item) {
			short pos;
			element_pointer y, z = x;
			element_pointer stack[DDTLibrary::MAX_TREE_HEIGHT];
			// We need a stack for updating the subtree sizes

			short sp = 0;
			bool found = false;

			while (z && !(found = z->Search (item, pos))) {
				stack[sp++] = z;
				if (z->_isLeaf) break;
				pos++;
				y =  z->_subtree[pos];
				if (y->_keyCount == 2*_order-1) {
					_SplitChild (z, pos, y);
					if (item >= z->_item[pos]) {
						pos++;
						y = z->_subtree[pos];
					}
				}
				z = y;
			}

			if (!found) {
				short n = z->_keyCount;
				if (n > 0) {
					z->ShiftRightAt (pos+1);
					z->_item[pos+1] = item;
				}
				else 
					z->_item[0] = item;
				z->_keyCount++;
				for (short i = 0; i < sp; i++) {
					stack[i]->_subtreeSize++;
				}
			}
			return !found;
		}


		void _SplitChild (element_pointer x, short i, element_pointer y) {
			element_pointer z = new elementType(_order);
			z->MoveSubNode (*y, _order, 0, _order-1);
			z->_isLeaf = y->_isLeaf;
			z->_keyCount = y->_keyCount = _order-1;
			x->ShiftRightAt (i); 
			// We shouldn't shift subtree[i], but it shouldn't matter

			x->_subtree[i+1] = z;
			x->_item [i] = y->_item [_order-1];
			x->_keyCount++;

			// Recompute _subtreeSize for y and z
			long size = 0;
			if (!z->_isLeaf) {
				for (short j = 0; j <= z->_keyCount; j++) {
					element_pointer p = z->_subtree[j];
					size += p->_subtreeSize;
				}
			}
			size += z->_keyCount;
			z->_subtreeSize = size;
			y->_subtreeSize -= size+1;
			y->modified ();
			x->modified ();
		}



		element_pointer _DescendInto (element_pointer node, short subtreeIndex,DeleteActionEnum& action) {
			element_pointer child, sibling, p;
			child = node->_subtree[subtreeIndex];
			if (!child || child->_keyCount >= _order) {
				action = NoAction;
				return child;
			}
			if (subtreeIndex == 0) {
				sibling = node->_subtree[1];
				p = _Adjust (node, 0, child, sibling, action);
			}
			else {
				sibling = node->_subtree[subtreeIndex-1];
				p = _Adjust (node, subtreeIndex-1, sibling, child, action);
			}
			return p;
		}


		element_pointer _Adjust (element_pointer node, short index,
			element_pointer c0, element_pointer c1, DeleteActionEnum& action) {
				assert ((c0 != NULL) && (c1 != NULL));
				// ("BTree::Adjust: assertion failed: line %d\n", __LINE__));
				assert ((c0->_keyCount == _order-1) || (c1->_keyCount == _order-1));
				// ("BTree::Adjust: assertion failed: line %d\n", __LINE__));

				if (c0->_keyCount == _order-1 && c1->_keyCount == _order-1) {
					// Merge the two nodes
					c0->_item[_order-1] = node->_item[index];
					c0->MoveSubNode (*c1, 0, _order, _order-1);
					c0->_keyCount = 2*_order-1;
					c0->_subtreeSize += c1->_subtreeSize+1;

					delete c1;
					if (node->_keyCount > 1) {
						node->ShiftLeftAt (index+1);
						node->_subtree[index] = c0;
					} else {
						_root = c0;
						node->_keyCount--;
					}
					action = Merge;
					return c0;
				}
				if (c0->_keyCount >= _order) {
					// Rotate right
					c1->ShiftRightAt (0);
					c1->_item[0] = node->_item[index];
					c1->_subtree[0] = c0->_subtree[c0->_keyCount];
					node->_item[index] = c0->_item[c0->_keyCount-1];
					c0->_keyCount--;
					c1->_keyCount++;
					if (!c1->_isLeaf) {
						element_pointer p = c1->_subtree[0];
						long xfr = (p) ? p->_subtreeSize+1 : 1;
						c1->_subtreeSize += xfr;
						c0->_subtreeSize -= xfr;
						c0->modified ();
					}
					action = RotateRight;
					return c1;
				} else {
					// c1->keyCount >= order, so rotate left
					c0->_item[_order-1] = node->_item[index];
					c0->_subtree[_order] = c1->_subtree[0];
					c0->_keyCount++;
					node->_item[index] = c1->_item[0];
					if (!c0->_isLeaf) {
						element_pointer p = c0->_subtree[_order];
						long xfr = (p) ? p->_subtreeSize+1 : 1;
						c1->_subtreeSize -= xfr;
						c0->_subtreeSize += xfr;
						c1->ShiftLeftAt(1);
						c1->modified ();
					}
					action = RotateLeft;
					return c0;
				}
		}




		//------------ Instance data -----------------------------

		typename Traits::template transform<short> _order;
		typename Traits::template transform<element_pointer> _root;
		const unsigned int instance_id_;

		//friend elementType;
		//friend BTreeIterator<input_value_type>;

		//  void        NewRoot (BTreeNode<T> *h);
		// Make the node the root of the tree.

		// BTreeNode<T>   *CreateNode ();
		// Create a new node and return its address.
	};

	template <class elementType> 
	class BinaryTree :public Tracing::allocated<typename elementType::Allocator> {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = elementType::type_num };
		typedef typename elementType::Logger Logger;
		typedef typename elementType::Allocator Allocator;
		typedef typename elementType::Traits Traits;
		////////////////////////////////////////////////////////////////////////
		typedef typename elementType::value_type value_type;
		typedef typename elementType::pointer pointer;
		typedef typename elementType::reference reference;
		typedef typename elementType::const_reference const_reference;

		typedef elementType elementType;

		typedef typename elementType::input_value_type input_value_type;
		typedef typename elementType::const_input_reference const_input_reference;

		typedef typename elementType::element_pointer element_pointer;
		typedef typename elementType::element_reference element_reference;

		typedef typename elementType::element_pointer element_pointer_unlogged;
		typedef typename elementType::element_reference element_reference_unlogged;

		typedef typename elementType::size_type size_type;
		typedef typename elementType::difference_type difference_type;


		// ---------------- Construction and destruction ---------------

		BinaryTree (short dummy = 0 /*dummy parameter*/) : instance_id_(NewInstance::createId()) {
#ifdef LOGGING
			typename Logger::object_construct S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type));
#endif
			_sentinel = new elementType();
			_root = _sentinel;
		}
		//BinaryTree (const BinaryTree & other)
		//	: _root(other._root), _sentinel(other._sentinel) {
		//}		
		~BinaryTree() {
			//delete _sentinel;
		}
		void initialize() {
			_root = _sentinel;
		}		
		inline bool IsNil(element_pointer_unlogged x) const {
			return x == _sentinel;
		}
		void LeftRotate(element_pointer_unlogged x) {
			element_pointer_unlogged y = x->_right;
			long a = x->_left->_subtreeSize;
			long b = y->_left->_subtreeSize;
			long c = y->_right->_subtreeSize;
			x->_right = y->_left;
			if (!IsNil(y->_left))
				y->_left->_parent = x;
			y->_parent = x->_parent;
			if (IsNil(x->_parent)) {
				_root = y;
			} else {
				if (x == x->_parent->_left) {
					x->_parent->_left = y;
				} else {
					x->_parent->_right = y;
				}
			}
			y->_left = x;
			x->_parent = y;
			x->_subtreeSize = a + b + 1;
			y->_subtreeSize = x->_subtreeSize + c + 1;
		}
		void RightRotate(element_pointer_unlogged x) {
			element_pointer_unlogged y = x->_left;
			long c = x->_right->_subtreeSize;
			long a = y->_left->_subtreeSize;
			long b = y->_right->_subtreeSize;
			x->_left = y->_right;
			if (!IsNil(y->_right))
				y->_right->_parent = x;
			y->_parent = x->_parent;
			if (IsNil(x->_parent)) {
				_root = y;
			} else {
				if (x == x->_parent->_right) {
					x->_parent->_right = y;
				} else {
					x->_parent->_left = y;
				}
			}
			y->_right = x;
			x->_parent = y;
			x->_subtreeSize = b + c + 1;
			y->_subtreeSize = x->_subtreeSize + a + 1;
		}
		virtual element_pointer_unlogged Insert(const_input_reference k){
			element_pointer_unlogged y = _sentinel;
			element_pointer_unlogged x = _root;
			while (!IsNil(x)) {
				y = x;
				x->_subtreeSize++;
				if (k < x->getData())
					x = x->_left;
				else
					x = x->_right;
			}
			element_pointer_unlogged z = new elementType(k, _sentinel, y);
			if (IsNil(y))
				_root = z;
			else {
				if (k < y->getData())
					y->_left = z;
				else
					y->_right = z;
			}
			return z;
		}		
		virtual element_pointer_unlogged Delete(element_pointer_unlogged z) {
			element_pointer_unlogged x, y;
			if(IsNil(z->_left) || IsNil(z->_right))
				y = z;
			else
				y = Successor(z);
			element_pointer tmp = y;
			while (!IsNil(tmp)) {
				tmp->_subtreeSize--;
				tmp = tmp->_parent;
			}
			if (!IsNil(y->_left))
				x = y->_left;
			else
				x = y->_right;
			x->_parent = y->_parent;
			if (IsNil(y->_parent))
				_root = x;
			else {
				if (y == y->_parent->_left)
					y->_parent->_left = x;
				else 
					y->_parent->_right = x;
			}
			if (y != z)
				z->_data = y->_data;

			return y;
		}
		element_pointer_unlogged Search(const_input_reference k) {
			return TreeSearch (_root, k);
		}
		inline element_pointer_unlogged Minimum() {
			return TreeMinimum (_root);
		}
		inline element_pointer_unlogged Maximum() {
			return TreeMaximum (_root);
		}
		pointer Smallest() const {
			if (!IsNil(_root))
				return Minimum()->getDataPointer();
			else
				return NULL;
		}
		pointer Largest() const {
			if (!IsNil(_root))
				return Maximum()->getDataPointer();
			else
				return NULL;
		};
		inline element_pointer_unlogged ExtractMin() {
			return Delete(TreeMinimum(_root));

		}
		pointer Find(const_input_reference item) {
			element_pointer_unlogged tmp = TreeSearch (_root, k);
			if (!IsNil(tmp))
				return tmp->getDataPointer();
			else
				return NULL;
		}
		element_pointer_unlogged Remove(const_input_reference item) {
			element_pointer_unlogged tmp = TreeSearch(_root, item);
			if (!IsNil(tmp)) {
				return Delete(tmp);
			}
			return tmp;
		}
		element_pointer_unlogged Successor(element_pointer_unlogged x) {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(x->_right))
				return TreeMinimum(x->_right);
			y = x->_parent;
			while ( (!IsNil(y)) && (x == y->_right) ) {
				x = y;
				y = x->_parent;
			}
			return y;
		}
		element_pointer_unlogged Predecessor(element_pointer_unlogged x) {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(x->_left))
				return TreeMaximum(x->_left);
			y = x->_parent;
			while ( (!IsNil(y)) && (x == y->_left) ) {
				x = y;
				y = x->_parent;
			}
			return y;
		}
		pointer ItemWithRank(size_type rank) {
			element_pointer_unlogged tmp = Select(_root, rank);
			if (!IsNil(tmp))
				return tmp->getDataPointer();
			else
				return NULL;
		}
		element_pointer_unlogged Select (element_pointer_unlogged x, size_type i) {
			if (i > x->_subtreeSize)
				return _sentinel;

			long r = x->_left->_subtreeSize + 1;

			else if (i == r)
				return x;
			else if (i < r)
				return Select(x->_left, i);
			else
				return Select(x->_right, i-r);
		}
		void Print() {
			std::cout << "Printing Tree Nodes :" << std::endl;
			InorderTreeWalk(_root);
			std::cout << std::endl;
		}
		inline element_pointer_unlogged Root() {
			return _root;
		}
		inline element_pointer_unlogged Sentinel() {
			return _sentinel;
		}
		inline element_pointer_unlogged getFirstElement() {
			return TreeMinimum(_root);
		}
		inline element_pointer_unlogged getLastElement() {
			return TreeMaximum(_root);
		}
		inline element_pointer_unlogged getEnd() {
			return _sentinel;
		}
		inline size_type getSize() {
			return _root->_subtreeSize;
		}
		inline bool empty() {
			return (_root->subtreeSize == 0);
		}
		//interface for iterators
		element_pointer_unlogged increment(element_pointer_unlogged data) {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(x->_right))
				return TreeMinimum(x->_right);
			y = x->_parent;
			while ( (!IsNil(y)) && (x == y->_right) ) {
				x = y;
				y = x->_parent;
			}
			return y;
		}
		element_pointer_unlogged decrement(element_pointer_unlogged data) {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(x->_left))
				return TreeMaximum(x->_left);
			y = x->_parent;
			while ( (!IsNil(y)) && (x == y->_left) ) {
				x = y;
				y = x->_parent;
			}
			return y;
		}


	protected:
		element_pointer_unlogged TreeSearch(element_pointer_unlogged x, const_input_reference k) {
			while ( (!IsNil(x)) && (k != x->getData()) ) {
				if (k < x->getData())
					x = x->_left;
				else
					x = x->_right;
			}
			return x;
		}
		inline element_pointer_unlogged TreeMinimum(element_pointer_unlogged x) {
			while (!IsNil(x->_left))
				x = x->_left;
			return x;
		}
		inline element_pointer_unlogged TreeMaximum(element_pointer_unlogged x) {
			while (!IsNil(x->_right))
				x = x->_right;
			return x;
		}
		void InorderTreeWalk(element_pointer_unlogged x) {
			if (!IsNil(x)) {
				InorderTreeWalk(x->_left);
				std::cout << x->getData() << " ";
				InorderTreeWalk(x->_right);
			}
		}

		element_pointer _root;
		element_pointer_unlogged _sentinel;
		const unsigned int instance_id_;

	};

	///////////////////////////////////////////////////////////////
	// Tree Base Adapters
	///////////////////////////////////////////////////////////////
	template <class treeType> 
	class RedBlackTreeAdapter : public treeType {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = treeType::type_num };
		typedef typename treeType::Logger Logger;
		typedef typename treeType::Allocator Allocator;
		typedef typename treeType::value_type value_type;
		////////////////////////////////////////////////////////////////////////
		typedef typename treeType::pointer pointer;
		typedef typename treeType::reference reference;
		typedef typename treeType::const_reference const_reference;

		typedef typename treeType::elementType elementType;

		typedef typename treeType::input_value_type input_value_type;
		typedef typename treeType::const_input_reference const_input_reference;

		typedef typename elementType::element_pointer element_pointer_unlogged;
		typedef typename elementType::element_reference element_reference_unlogged;

		typedef typename treeType::element_pointer element_pointer;
		typedef typename treeType::element_reference element_reference;

		typedef typename treeType::size_type size_type;
		typedef typename treeType::difference_type difference_type;


		// ---------------- Construction and destruction ---------------

		RedBlackTreeAdapter (short dummy = 0 /*dummy parameter*/) {}
		//RedBlackTreeAdapter (const RedBlackTreeAdapter & other) {
		//	_root = other._root;
		//	_sentinel = other._sentinel;
		//}
		virtual element_pointer_unlogged Insert(const_input_reference k){
			element_pointer_unlogged y = _sentinel;
			element_pointer_unlogged x = _root;
			while (!IsNil(x)) {
				y = x;
				x->_subtreeSize++;
				if (k < x->getData())
					x = x->_left;
				else
					x = x->_right;
			}
			element_pointer_unlogged z = new elementType(k, _sentinel, y);
			if (IsNil(y))
				_root = z;
			else {
				if (k < y->getData())
					y->_left = z;
				else
					y->_right = z;
			}
			RBInsertFixup(z);
			return z;
		}
		virtual element_pointer_unlogged Delete(element_pointer_unlogged z) {
			element_pointer_unlogged x, y;
			if(IsNil(z->_left) || IsNil(z->_right))
				y = z;
			else
				y = Successor(z);
			element_pointer_unlogged tmp = y;
			while (!IsNil(tmp)) {
				tmp->_subtreeSize--;
				tmp = tmp->_parent;
			}
			if (!IsNil(y->_left))
				x = y->_left;
			else
				x = y->_right;
			x->_parent = y->_parent;
			if (IsNil(y->_parent))
				_root = x;
			else {
				if (y == y->_parent->_left)
					y->_parent->_left = x;
				else 
					y->_parent->_right = x;
			}
			if (y != z)
				z->_data = y->_data;
			if (y->_color == BLACK)
				RBDeleteFixup(x);

			delete y;

			return y;
		}

	protected:
		void RBInsertFixup(element_pointer_unlogged z) {
			element_pointer_unlogged y;
			while ( (!IsNil(z)) && (!IsNil(z->_parent)) && (z->_parent->_color == RED) ) {
				if (z->_parent == z->_parent->_parent->_left) {
					y = z->_parent->_parent->_right;
					if (y->_color == RED) {
						z->_parent->_color = BLACK;
						y->_color = BLACK;
						z->_parent->_parent->_color = RED;
						z = z->_parent->_parent;
					} else {
						if (z == z->_parent->_right) {
							z = z->_parent;
							LeftRotate(z);
						}
						z->_parent->_color = BLACK;
						z->_parent->_parent->_color = RED;
						RightRotate(z->_parent->_parent);
					}
				} else {
					y = z->_parent->_parent->_left;
					if (y->_color == RED) {
						z->_parent->_color = BLACK;
						y->_color = BLACK;
						z->_parent->_parent->_color = RED;
						z = z->_parent->_parent;
					} else {
						if (z == z->_parent->_left) {
							z = z->_parent;
							RightRotate(z);
						}
						z->_parent->_color = BLACK;
						z->_parent->_parent->_color = RED;
						LeftRotate(z->_parent->_parent);
					}
				}
			}
			_root->_color = BLACK;
		}
		void RBDeleteFixup(element_pointer_unlogged x) {
			element_pointer_unlogged w;
			while ( (x != _root) && (x->_color == BLACK) ) {
				if (x == x->_parent->_left) {
					w = x->_parent->_right;
					if (w->_color == RED) {
						w->_color = BLACK;
						x->_parent->_color = RED;
						LeftRotate(x->_parent);
						w = x->_parent->_right;
					}
					if ( (w->_left->_color == BLACK) && (w->_right->_color == BLACK) ) {
						w->_color = RED;
						x = x->_parent;
					} else {
						if (w->_right->_color == BLACK) {
							w->_left->_color = BLACK;
							w->_color = RED;
							RightRotate(w);
							w = x->_parent->_right;
						}
						w->_color = x->_parent->_color;
						x->_parent->_color = BLACK;
						w->_right->_color = BLACK;
						LeftRotate(x->_parent);
						x = _root;
					}
				} else {
					w = x->_parent->_left;
					if (w->_color == RED) {
						w->_color = BLACK;
						x->_parent->_color = RED;
						RightRotate(x->_parent);
						w = x->_parent->_left;
					}
					if ( (w->_right->_color == BLACK) && (w->_left->_color == BLACK) ) {
						w->_color = RED;
						x = x->_parent;
					} else {
						if (w->_left->_color == BLACK) {
							w->_right->_color = BLACK;
							w->_color = RED;
							LeftRotate(w);
							w = x->_parent->_left;
						}
						w->_color = x->_parent->_color;
						x->_parent->_color = BLACK;
						w->_left->_color = BLACK;
						RightRotate(x->_parent);
						x = _root;
					}
				}
			}
		}


	};

	///////////////////////////////////////////////////////////////
	// A search path is a sequence of pairs of the form 
	// <node#, subtree#>, with the  first pair <root,  subtree#>
	// and the   last pair being  of the form <node#,  key#>. It
	// completely   specifies the path   from the root  to a
	// particular key in the tree.
	// (needed for BTree)
	///////////////////////////////////////////////////////////////
	template <class element_pointer>
	class PathStruct
	{
	public:
		element_pointer _handle;
		short _indexInNode;
	};

	//to be removed
	template <class iteratorExtension>
	class BTreeIterator_Extension : public iteratorExtension {    
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = iteratorExtension::type_num };
		typedef typename iteratorExtension::Logger Logger;
		////////////////////////////////////////////////////////////////////////
		typedef typename iteratorExtension::dataStructureType dataStructureType;
		typedef typename iteratorExtension::value_type value_type;
		typedef typename iteratorExtension::pointer pointer;
		typedef typename iteratorExtension::reference reference;
		typedef typename iteratorExtension::elementType elementType;
		typedef typename iteratorExtension::element_pointer element_pointer;
		typedef typename iteratorExtension::size_type size_type;
		typedef typename iteratorExtension::difference_type difference_type;

		// Constructor: create a BTreeIterator for the given tree {\tt t}.
		BTreeIterator_Extension ()
			: _path(NULL), _length(-1) {
		}

		BTreeIterator_Extension (const dataStructureType * dataStructure) {
			//cout << "BTreeIterator_Extension : creating iterator with size = " << tree_size << endl;
			_dataStructure = new dataStructureType(*dataStructure);
			_path = new PathStruct<element_pointer> [MAX_TREE_HEIGHT];
			_length = 0;
		}
		// Copy constructor. The copy inspects the same B-tree as {\tt itr}, and
		// (unless reset) begins  its iteration at the item at which {\tt itr}
		// is currently positioned.
		BTreeIterator_Extension (const BTreeIterator_Extension& itr) {
			_dataStructure = new dataStructureType(*(itr.dataStructure));
			_path = new PathStruct<element_pointer> [MAX_TREE_HEIGHT];
			if (_path) {
				_length = itr._length;
				for (register short i = 0; i < _length; i++)
					_path[i] = itr._path[i];
			}
			else
				_length = 0;
			_index = itr._index;
		}


		// Destructor.
		~BTreeIterator_Extension() {
			if (_path)
				delete [] _path;
		}

		// Reset the iterator to the leftmost (smallest) item.
		void ResetBegin() {
			if (!_path) // Memory allocation failed?
				return;
			_length = 1;
			PathStruct<element_pointer>* path = _path;
			path[0]._handle = _dataStructure->Root();
			path[0]._indexInNode = -1;
			_index = -1;
			increment();
		}

		// Reset the iterator to the rightmost (largest) item.
		void ResetEnd() {
			_length = 1;
			PathStruct<element_pointer>* path = _path;
			path[0]._handle = _dataStructure->Root();
			path[0]._indexInNode = path[0]._handle->getKeyCount();
			_index = _dataStructure->Size();
			decrement();
		}

		// Reset the iterator to the leftmost (smallest) item.
		void ResetRBegin() {
			ResetEnd();
		}

		// Reset the iterator to the rightmost (largest) item.
		void ResetREnd() {
			ResetRBegin();
		}

		// Begin the iteration from the given item. The next call to {\tt Next}
		// will return the given item (or the one immediately
		// larger, if the given item isn't in the tree).
		void BeginFrom (value_type item) {
			short pos;
			bool found;

			if (!_path) // Memory allocation failed?
				return;

			_length = 0;
			_index  = -1;
			if (_dataStructure->Size() <= 0)
				return;

			PathStruct<element_pointer>* path = _path;

			element_pointer tmp_ptr, p;
			tmp_ptr = _dataStructure->Root();
			do {
				found = tmp_ptr->Search (item, pos);
				path[_length]._handle = tmp_ptr;
				_index += path[_length]._indexInNode = found ? pos : pos+1;
				_length++;
				if (tmp_ptr->_isLeaf) break;
				for (register long i = 0; i <= pos; i++) {
					element_pointer p = tmp_ptr->_subtree[i];
					_index += p->_subtreeSize;
				}
				if (found) break;
				tmp_ptr = tmp_ptr->_subtree [pos+1];
			} while (1);

			if (!tmp_ptr->_isLeaf) {
				// We're in an internal node; so move down to the leaf
				p = tmp_ptr->_subtree[pos];
				do {
					path[_length]._handle = p;
					path[_length]._indexInNode = p->_keyCount;
					_length++;
					p = p->_subtree[p->_keyCount]; // Rightmost subtree
				} while (p);
			}
			path[_length-1]._indexInNode--;
			// So that the first call to Next gives
			// the nearest key >= the given key
		}


		// Tell whether there are more items in the iteration.
		bool More() const {
			return _index < _dataStructure->Size()-1;
		}
		bool Exist() const {
			return _index < _dataStructure->Size();
		}
		bool Exist2() const {
			return _index > 0;
		}

		//void Update(element_pointer root, long size) {
		//	_dataStructure_root = root;
		//	_dataStructure_size = size;
		//}

		// Return the next item in the iteration sequence. Return the NULL
		// pointer if no more items.
		pointer increment() {
			//if (_index >= _dataStructure->Size()) {
			//	std::cout << "no more elements to access!" << std::endl;
			//	return NULL;
			//}

			if (!_path || _length == 0)
				return NULL;

			PathStruct<element_pointer>* path = _path;
			element_pointer node = path[_length-1]._handle;
			short  ndx = path[_length-1]._indexInNode;

			_index++;
			if (! node->isLeaf()) {
				// Move to the next right subtree
				path[_length-1]._indexInNode++;
				element_pointer handle = node->getSubTree(ndx+1);
				while (!node->isLeaf()) {
					path[_length]._handle = handle;
					path[_length]._indexInNode = 0;
					_length++;
					node = handle;
					handle = node->getSubTree(0);
				}
				return node->getItem();
			}

			// We're in a leaf
			if (ndx >= node->getKeyCount()-1) {
				// We're at far right of the leaf, so move up
				do {
					_length--;
					if (_length <= 0) break;
					node = path[_length-1]._handle;
					ndx = path[_length-1]._indexInNode;
				} while (ndx >= node->getKeyCount());

				pointer retVal;
				if (_length) {
					retVal = node->getItem() + ndx;
				}
				else
					retVal = NULL;
				return retVal;
			}

			// We're in the middle or at left end of a leaf
			path[_length-1]._indexInNode++;
			return node->getItem() + path[_length-1]._indexInNode;
		}

		pointer decrement() {
			//if (_index < 1) {
			//	std::cout << "no more elements to access!" << std::endl;
			//	return NULL;
			//}

			if (!_path || _length == 0)
				return NULL;

			PathStruct<element_pointer>* path = _path;
			element_pointer node = path[_length-1]._handle;
			short  ndx = path[_length-1]._indexInNode;
			/*std::cout << std::endl << "mpainw 1" << std::endl;*/
			_index--;
			if (!node->isLeaf()) {
				// Move to the next left subtree
				path[_length-1]._indexInNode--;
				element_pointer handle = node->getSubTree(ndx);
				while (!node->isLeaf()) {
					path[_length]._handle = handle;
					path[_length]._indexInNode = handle->getKeyCount();
					_length++;
					node = handle;
					handle = node->getSubTree(node->getKeyCount());
				}
				path[_length-1]._indexInNode--;
				return node->getItem() + path[_length-1]._indexInNode;
			}

			// We're in a leaf
			if (ndx <= 0) {
				// We're at far left of the leaf, so move up
				do {
					_length--;
					if (_length <= 0) break;
					node = path[_length-1]._handle;
					ndx = path[_length-1]._indexInNode;
				} while (ndx < 0);
				pointer retVal;
				if (_length) {
					//tricky point..
					if (path[_length-1]._handle->getKeyCount() == path[_length-1]._indexInNode)
						ndx = --path[_length-1]._indexInNode;
					retVal = node->getItem() + ndx;
				}
				else
					retVal = NULL;
				return retVal;
			}

			// We're in the middle or at right end of a leaf
			path[_length-1]._indexInNode--;
			return node->getItem() + path[_length-1]._indexInNode;
		}

		// Return the rank of the element that was returned by the most recent
		// call to {\tt increment()}.
		long CurrentRank () const {
			return _index;
		}

		void copyConstruction(const BTreeIterator_Extension& other) {
			_length = other._length;
			_index = other._index;
			_dataStructure = other._dataStructure;
			_path = new PathStruct<element_pointer> [MAX_TREE_HEIGHT];
			if (_path) {
				for (register short i = 0; i < _length; i++)
					_path[i] = other._path[i];
			}
		}

		virtual bool operatorLower(const Iterator_Extension<dataStructureType>& other) const {
			return iteratorExtension::operatorLower(other);
		}
		virtual bool operatorGreater(const Iterator_Extension<dataStructureType>& other) const {
			return iteratorExtension::operatorGreater(other);
		}
		void operatorAssignment(const BTreeIterator_Extension& other) {
			_length = other._length;
			_index = other._index;
			_dataStructure = other._dataStructure;
			_path = new PathStruct<element_pointer> [MAX_TREE_HEIGHT];
			if (_path) {
				for (register short i = 0; i < _length; i++)
					_path[i] = other._path[i];
			}
		}
		pointer getData() {
			return _path[_length-1]._handle->getItem() + _path[_length-1]._indexInNode;
		}
		element_pointer getHandle() const {
			return _path[_length-1]._handle;
		}


		// --------------------- End public protocol -----------------------


	protected:
		PathStruct<element_pointer> * _path;  // Stack containing path to current element
		short _length;			// Length of stack
	};

	///////////////////////////////////////////////////////////////
	// This is the main handler for the tree abstract data type
	///////////////////////////////////////////////////////////////
	template <class treeType>
	class Tree : public treeType{
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = treeType::type_num };
		typedef typename treeType::Logger Logger;
		typedef typename treeType::Allocator Allocator;
		typedef typename treeType::value_type value_type;
		////////////////////////////////////////////////////////////////////////
		typedef typename treeType::pointer pointer;
		typedef typename treeType::reference reference;
		typedef typename treeType::const_reference const_reference;

		typedef typename treeType::element_pointer element_pointer;
		typedef typename treeType::element_reference element_reference;
		typedef typename treeType::elementType elementType;

		typedef typename treeType::input_value_type input_value_type;
		typedef typename treeType::const_input_reference const_input_reference;

		typedef typename treeType::size_type size_type;
		typedef typename treeType::difference_type difference_type;

		typedef typename treeType::iterator iterator;
		typedef typename treeType::const_iterator const_iterator;

		typedef typename treeType::reverse_iterator reverse_iterator;
		typedef typename treeType::const_reverse_iterator const_reverse_iterator;

		typedef Iterator_Base_Unlogged<Tree> iterator_unlogged;

		Tree(short order = 2)
			: treeType(order >= 2 ? order : 2) {
		}

		void clear() {
#ifdef LOGGING
			typename Logger::object_clear S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type));
#endif
			if (!treeType::empty()) {
				iterator_unlogged it1(this, treeType::getFirstElement());
				iterator_unlogged it2(this, treeType::getEnd());
				Destructor2(it1, it2);
			}

			treeType::initialize();

		}

		~Tree() {
#ifdef LOGGING
			typename Logger::object_destruct S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type));
#endif
			clear();
		}

		////////////////////////////////////////////////////////////////////////
		// Basic iterators of DDT
		////////////////////////////////////////////////////////////////////////
		const_iterator begin() const {
			return treeType::createConstIterator(this, treeType::getFirstElement(), instance_id_);
		}
		iterator begin() {
			//cout << "BTree : creating iterator with size = " << Size() << endl;
			return treeType::createIterator(this, treeType::getFirstElement(), instance_id_);
		}
		const_iterator end() const {
			const_iterator it = treeType::createConstIterator(this, treeType::getEnd(), instance_id_);
			it.ResetEnd();
			return it;
		}
		iterator end() {
			iterator it = treeType::createIterator(this, treeType::getEnd(), instance_id_);
			it.ResetEnd();
			return it;
		}
		////////////////////////////////////////////////////////////////////////
		// Reverse iterators of DDT
		////////////////////////////////////////////////////////////////////////
		const_reverse_iterator rbegin() const {
			return treeType::createConstReverseIterator(this, treeType::getLastElement(), instance_id_);
		}
		reverse_iterator rbegin() {
			return treeType::createReverseIterator(this, treeType::getLastElement(), instance_id_);
		}
		const_reverse_iterator rend() const {
			reverse_iterator it = treeType::createConstReverseIterator(this, treeType::getEnd(), instance_id_);
			it.ResetEnd();
			return it;
		}
		reverse_iterator rend() {
			reverse_iterator it = treeType::createReverseIterator(this, treeType::getEnd(), instance_id_);
			it.ResetEnd();
			return it;
		}

		////////////////////////////////////////////////////////////////////////
		// Size information
		////////////////////////////////////////////////////////////////////////
		inline bool empty() const {
			return treeType::empty();
		}
		inline size_type size() const {
			return treeType::getSize();
		}
		inline size_type capacity() const {
			return treeType::getSize();
		}

		void reserve(size_type new_capacity) {}
		//		void resize(size_type new_size) {
		//			#ifdef LOGGING
		//          		typename Logger::object_resize S(TYPEID(input_value_type), type_num, instance_id_, new_size);
		//			#endif
		//                        if (new_size > size())
		//				treeType::resize(new_size);
		//		}

		reference operator[](size_type n) { return get(n);}

		const_reference operator[](size_type n) const { return get(n); }

		reference get(size_type n) {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), n);
#endif
			return *(treeType::ItemWithRank(n));
		}

		const_reference get(size_type n) const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), n);
#endif
			return *(treeType::ItemWithRank(n));
		}

		const_reference front() const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), 0);
#endif
			return *(treeType::Smallest());
		}
		reference front() {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), 0);
#endif
			return *(treeType::Smallest());
		}
		const_reference back() const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), numberofObjects);
#endif
			return *(treeType::Largest());
		}
		reference back() {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id,
				sizeof(input_value_type), numberofObjects);
#endif
			return *(treeType::Largest());
		}

		iterator erase(const iterator& pos) {
			element_pointer_unlogged curr = pos.getData();
			element_pointer_unlogged next = treeType::Successor(curr);
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type), curr);
#endif
			element_pointer_unlogged tmp = treeType::Delete(curr);
			if (!IsNil(tmp))
				delete tmp;
			return treeType::createIterator(this, next, instance_id_);
		}

		reverse_iterator erase(const reverse_iterator& pos) {
			element_pointer_unlogged curr = pos.getData();
			element_pointer_unlogged prev = treeType::Predecessor(curr);
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type), curr);
#endif
			element_pointer_unlogged tmp = treeType::Delete(curr);
			if (!IsNil(tmp))
				delete tmp;
			return treeType::createIterator(this, prev, instance_id_);
		}

		void Insert(const_input_reference value) {
			element_pointer_unlogged tmp = treeType::Insert(value);
#ifdef LOGGING
			typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), tmp);
#endif
		}

		void Remove(const_input_reference value) {
			element_pointer_unlogged tmp = treeType::Remove(value);
			if (tmp) {
#ifdef LOGGING
				typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type), tmp);
#endif
			}
		}

		pointer ExtractMin () {
			if (numberofObjects > 0) {
				element_pointer tmp = treeType::ExtractMin();
#ifdef LOGGING
				typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type), tmp);
#endif
				value_type k = tmp->Key();
				return &(k);
			} else {
				return NULL;
			}
		}

		pointer Find(const_input_reference item) {
			return treeType::Find(item);
		}


	};
}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif
