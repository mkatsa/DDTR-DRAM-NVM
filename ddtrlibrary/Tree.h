#ifndef TREE_H
#define TREE_H

#include "DDTIncludes.h"
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {

	template <class treeType> class Tree;

	template <class treeType> class RedBlackTreeExtension;
	template <class elementType> class BinaryTree;

	template <class treeNode> class RedBlackTreeNodeExtension;
	template <class T, class objectTraits = Tracing::no_traits, class ddtTraits = Tracing::no_traits> class BinaryTreeNode;

	///////////////////////////////////////////////////////////////
	// Tree Nodes
	///////////////////////////////////////////////////////////////
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

		typedef typename ddtTraits::template transform<BinaryTreeNode*>::value_type element_pointer;
		typedef typename ddtTraits::template transform<BinaryTreeNode&>::value_type element_reference;

		typedef typename ddtTraits::template transform<T*>::value_type value_pointer;

		typedef ddtTraits BTddtTraits;
		typedef objectTraits BTobjectTraits;
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
		element_pointer _left, _right, _parent;

	};

	///////////////////////////////////////////////////////////////
	//Tree Node Extensions
	///////////////////////////////////////////////////////////////
	template <class treeNode>
	class RedBlackTreeNodeExtension : public treeNode {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = treeNode::type_num };
		typedef typename treeNode::Logger Logger;
		typedef typename treeNode::Allocator Allocator;
		typedef typename treeNode::value_type value_type;
		////////////////////////////////////////////////////////////////////////
		typedef typename treeNode::pointer pointer;
		typedef typename treeNode::const_pointer const_pointer;
		typedef typename treeNode::reference reference;
		typedef typename treeNode::const_reference const_reference;

		typedef typename treeNode::input_value_type input_value_type;
		typedef typename treeNode::const_input_reference const_input_reference;

		typedef typename treeNode::BTobjectTraits::template transform<RedBlackTreeNodeExtension*>::value_type element_pointer_unlogged;
		typedef typename treeNode::BTobjectTraits::template transform<RedBlackTreeNodeExtension&>::value_type element_reference_unlogged;

		typedef typename treeNode::BTddtTraits::template transform<RedBlackTreeNodeExtension*>::value_type element_pointer;
		typedef typename treeNode::BTddtTraits::template transform<RedBlackTreeNodeExtension&>::value_type element_reference;

		typedef typename treeNode::value_pointer value_pointer;

		typedef typename treeNode::size_type size_type;
		typedef typename treeNode::difference_type difference_type;
		
		using treeNode::_subtreeSize;
		using treeNode::_data;

		// ---------------- Construction and destruction ---------------

		RedBlackTreeNodeExtension () {
			_left.setData(this);
			_right.setData(this);
			_parent.setData(this);
			_subtreeSize = 0;
			_data = (pointer) Allocator::malloc(sizeof(value_type));
			_color = BLACK;
			_subtreeSize = 0;
		}

		RedBlackTreeNodeExtension (const_input_reference data, element_pointer_unlogged sentinel, element_pointer_unlogged parent) {
			_left.setData(sentinel);
			_right.setData(sentinel);
			_parent.setData(parent);
			_subtreeSize = 1;
			_data =  new value_type(data);
			_color = RED;
		}
		//RedBlackTreeNodeExtension (const element_reference other) {
		//	_color = other._color;
		//	_key = other._key;
		//	_left = other._left;
		//	_right = other._right;
		//	_parent = other._parent;
		//	_subtreeSize = other._subtreeSize;
		//}

		~RedBlackTreeNodeExtension() {}

		short Color() {
			return _color;
		}
	protected:
		friend BinaryTree<RedBlackTreeNodeExtension>;
		friend RedBlackTreeExtension<BinaryTree<RedBlackTreeNodeExtension> >;
		element_pointer _left, _right, _parent;
		typename treeNode::BTddtTraits::template transform<short>::value_type _color;

	};

	///////////////////////////////////////////////////////////////
	// Tree Bases
	///////////////////////////////////////////////////////////////
	template <class elementType> 
	class BinaryTree :public Tracing::allocated<typename elementType::Allocator> {
	public:
		////////////////////////////////////////////////////////////////////////
		enum { type_num = elementType::type_num };
		typedef typename elementType::Logger Logger;
		typedef typename elementType::Allocator Allocator;
		////////////////////////////////////////////////////////////////////////
		typedef typename elementType::value_type value_type;
		typedef typename elementType::pointer pointer;
		typedef typename elementType::reference reference;
		typedef typename elementType::const_reference const_reference;

		typedef elementType BTelementType;

		typedef typename elementType::input_value_type input_value_type;
		typedef typename elementType::const_input_reference const_input_reference;

		typedef typename elementType::element_pointer element_pointer;
		typedef typename elementType::element_reference element_reference;

		typedef typename elementType::element_pointer_unlogged element_pointer_unlogged;
		typedef typename elementType::element_reference_unlogged element_reference_unlogged;

		typedef typename elementType::size_type size_type;
		typedef typename elementType::difference_type difference_type;


		// ---------------- Construction and destruction ---------------

		BinaryTree () : instance_id_(NewInstance::createId()) {
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

			return y;
		}
		element_pointer_unlogged Search(const_input_reference k) {
			return TreeSearch (_root, k);
		}
		inline element_pointer_unlogged Minimum() const {
			return TreeMinimum (_root);
		}
		inline element_pointer_unlogged Maximum() const{
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
			element_pointer_unlogged tmp = TreeSearch(_root, item);
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
            element_pointer_unlogged tmp;
            if (rank == 0) {
                tmp = _root;
            } else {
                tmp = Select(_root, rank);
            }
            
			if (!IsNil(tmp))
				return tmp->getDataPointer();
			else
				return NULL;
		}
		element_pointer_unlogged Select (element_pointer_unlogged x, size_type i) {
			
			long r = x->_left->_subtreeSize + 1;
			if (i > x->_subtreeSize) {
				return _sentinel;
			}
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
		////////////////////////////////////////////////////////////////////////
		// Glue Layer Integration
		////////////////////////////////////////////////////////////////////////
		inline element_pointer_unlogged getREnd() {
			return _sentinel;
		}
		inline size_type getSize() const {
			return _root->_subtreeSize;
		}
		inline bool empty() const {
			return (_root->_subtreeSize == 0);
		}
		//interface for iterators
		element_pointer_unlogged increment(element_pointer_unlogged data) const {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(data->_right))
				return TreeMinimum(data->_right);
			y = data->_parent;
			while ( (!IsNil(y)) && (data == y->_right) ) {
				data = y;
				y = data->_parent;
			}
			return y;
		}
		element_pointer_unlogged decrement(element_pointer_unlogged data) const {
			element_pointer_unlogged y = _sentinel;
			if (!IsNil(data->_left))
				return TreeMaximum(data->_left);
			y = data->_parent;
			while ( (!IsNil(y)) && (data == y->_left) ) {
				data = y;
				y = data->_parent;
			}
			return y;
		}


		inline pointer getData(element_pointer_unlogged data) const {
			return data->getDataPointer();
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
		inline element_pointer_unlogged TreeMinimum(element_pointer_unlogged x) const {
			while (!IsNil(x->_left))
				x = x->_left;
			return x;
		}
		inline element_pointer_unlogged TreeMaximum(element_pointer_unlogged x) const {
			while (!IsNil(x->_right))
				x = x->_right;
			return x;
		}
		void InorderTreeWalk(element_pointer_unlogged x) {
			if (!IsNil(x)) {
				InorderTreeWalk(x->_left);
				//std::cout << x->getData() << " ";
				InorderTreeWalk(x->_right);
			}
		}

		element_pointer _root;
		element_pointer_unlogged _sentinel;
		const unsigned int instance_id_;

	};

	///////////////////////////////////////////////////////////////
	// Tree Base Extensions
	///////////////////////////////////////////////////////////////
	template <class treeType> 
	class RedBlackTreeExtension : public treeType {
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

		typedef typename treeType::BTelementType elementType;

		typedef typename treeType::input_value_type input_value_type;
		typedef typename treeType::const_input_reference const_input_reference;

		typedef typename treeType::element_pointer_unlogged element_pointer_unlogged;
		typedef typename treeType::element_reference_unlogged element_reference_unlogged;

		typedef typename treeType::element_pointer element_pointer;
		typedef typename treeType::element_reference element_reference;

		typedef typename treeType::size_type size_type;
		typedef typename treeType::difference_type difference_type;


		using treeType::_sentinel;
		using treeType::_root;

		// ---------------- Construction and destruction ---------------

		RedBlackTreeExtension () {}
		//RedBlackTreeExtension (const RedBlackTreeExtension & other) {
		//	_root = other._root;
		//	_sentinel = other._sentinel;
		//}
		virtual element_pointer_unlogged Insert(const_input_reference k){
			element_pointer_unlogged y = _sentinel;
			element_pointer_unlogged x = _root;
			while (!(this->IsNil(x))) {
				y = x;
				x->_subtreeSize++;
				if (k < x->getData())
					x = x->_left;
				else
					x = x->_right;
			}
			element_pointer_unlogged z = new elementType(k, _sentinel, y);
			if (this->IsNil(y))
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
			if(this->IsNil(z->_left) || this->IsNil(z->_right))
				y = z;
			else
				y = this->Successor(z);
			element_pointer_unlogged tmp = y;
			while (!(this->IsNil(tmp))) {
				tmp->_subtreeSize--;
				tmp = tmp->_parent;
			}
			if (!(this->IsNil(y->_left)))
				x = y->_left;
			else
				x = y->_right;
			x->_parent = y->_parent;
			if (this->IsNil(y->_parent))
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

			return y;
		}

	protected:
		void RBInsertFixup(element_pointer_unlogged z) {
			element_pointer_unlogged y;
			while ( (!(this->IsNil(z))) && (!(this->IsNil(z->_parent))) && (z->_parent->_color == RED) ) {
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
							this->LeftRotate(z);
						}
						z->_parent->_color = BLACK;
						z->_parent->_parent->_color = RED;
						this->RightRotate(z->_parent->_parent);
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
							this->RightRotate(z);
						}
						z->_parent->_color = BLACK;
						z->_parent->_parent->_color = RED;
						this->LeftRotate(z->_parent->_parent);
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
						this->LeftRotate(x->_parent);
						w = x->_parent->_right;
					}
					if ( (w->_left->_color == BLACK) && (w->_right->_color == BLACK) ) {
						w->_color = RED;
						x = x->_parent;
					} else {
						if (w->_right->_color == BLACK) {
							w->_left->_color = BLACK;
							w->_color = RED;
							this->RightRotate(w);
							w = x->_parent->_right;
						}
						w->_color = x->_parent->_color;
						x->_parent->_color = BLACK;
						w->_right->_color = BLACK;
						this->LeftRotate(x->_parent);
						x = _root;
					}
				} else {
					w = x->_parent->_left;
					if (w->_color == RED) {
						w->_color = BLACK;
						x->_parent->_color = RED;
						this->RightRotate(x->_parent);
						w = x->_parent->_left;
					}
					if ( (w->_right->_color == BLACK) && (w->_left->_color == BLACK) ) {
						w->_color = RED;
						x = x->_parent;
					} else {
						if (w->_left->_color == BLACK) {
							w->_right->_color = BLACK;
							w->_color = RED;
							this->LeftRotate(w);
							w = x->_parent->_left;
						}
						w->_color = x->_parent->_color;
						x->_parent->_color = BLACK;
						w->_left->_color = BLACK;
						this->RightRotate(x->_parent);
						x = _root;
					}
				}
			}
		}


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
		typedef typename treeType::BTelementType elementType;

		typedef typename treeType::element_pointer_unlogged element_pointer_unlogged;
		typedef typename treeType::element_reference_unlogged element_reference_unlogged;

		typedef typename treeType::input_value_type input_value_type;
		typedef typename treeType::const_input_reference const_input_reference;

		typedef typename treeType::size_type size_type;
		typedef typename treeType::difference_type difference_type;

		typedef Iterator_Base<Tree> iterator;
		typedef ConstIteratorAdapter<iterator> const_iterator;

		typedef ReverseIteratorAdapter<iterator> reverse_iterator;
		typedef ReverseIteratorAdapter<const_iterator> const_reverse_iterator;

#ifndef MORE_LOGGING
		typedef Iterator_Base_Unlogged<Tree> iterator_unlogged;
#else
		typedef iterator iterator_unlogged;
#endif
		
		using treeType::instance_id_;

		Tree() {}

		void clear() {
#ifdef LOGGING
			typename Logger::object_clear S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type));
#endif
			if (!treeType::empty()) {
				iterator_unlogged it1(this, treeType::getFirstElement(), instance_id_);
				iterator_unlogged it2(this, treeType::getEnd(), instance_id_);
				//Destructor2(it1, it2);
                Destructor(it1, it2);
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
			return const_iterator(this, treeType::getFirstElement(), instance_id_);
		}
		iterator begin() {
			return iterator(this, treeType::getFirstElement(), instance_id_);
		}
		const_iterator end() const {
			return const_iterator(this, treeType::getEnd(), instance_id_);
		}
		iterator end() {
			return iterator(this, treeType::getEnd(), instance_id_);
		}
		////////////////////////////////////////////////////////////////////////
		// Reverse iterators of DDT
		////////////////////////////////////////////////////////////////////////
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this, treeType::getLastElement(), instance_id_);
		}
		reverse_iterator rbegin() {
			return reverse_iterator(this, treeType::getLastElement(), instance_id_);
		}
		const_reverse_iterator rend() const {
			return const_reverse_iterator(this, treeType::getEnd(), instance_id_);
		}
		reverse_iterator rend() {
			return reverse_iterator(this, treeType::getEnd(), instance_id_);
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
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), n);
#endif
			return *(treeType::ItemWithRank(n));
		}

		const_reference get(size_type n) const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), n);
#endif
			return *(treeType::ItemWithRank(n));
		}

		const_reference front() const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), 0);
#endif
			return *(treeType::Smallest());
		}
		reference front() {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), 0);
#endif
			return *(treeType::Smallest());
		}
		const_reference back() const {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), treeType::getSize());
#endif
			return *(treeType::Largest());
		}
		reference back() {
#ifdef LOGGING
			typename Logger::object_get S(TYPEID(input_value_type), type_num, instance_id_,
				sizeof(input_value_type), treeType::getSize());
#endif
			return *(treeType::Largest());
		}

		iterator erase(const iterator& pos) {
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
			element_pointer_unlogged curr = pos.getData();
			element_pointer_unlogged next = treeType::Successor(curr);
			element_pointer_unlogged tmp = treeType::Delete(curr);
			if (!(this->IsNil(tmp)))
				Destructor(tmp->getDataPointer());
			return iterator(this, next, instance_id_);
		}

		reverse_iterator erase(const reverse_iterator& pos) {
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
			element_pointer_unlogged curr = pos.getData();
			element_pointer_unlogged prev = treeType::Predecessor(curr);
			element_pointer_unlogged tmp = treeType::Delete(curr);
			if (!(this->IsNil(tmp)))
				Destructor(tmp->getDataPointer());
			return reverse_iterator(this, prev, instance_id_);
		}
        
        /*iterator lower_bound(key) {
            
        }*/

		iterator insert(const_input_reference value) {
#ifdef LOGGING
			typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
			element_pointer_unlogged tmp = treeType::Insert(value);
			return iterator(this, tmp, instance_id_);
		}
		
		void push_back(const_input_reference value) {
#ifdef LOGGING
			typename Logger::object_insert S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));			
#endif
			treeType::Insert(value);
		}
			

		void remove(const_input_reference value) {
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
			treeType::Remove(value);
		}

		pointer ExtractMin () {
#ifdef LOGGING
			typename Logger::object_remove S(TYPEID(input_value_type), type_num, instance_id_, sizeof(input_value_type));
#endif
			element_pointer_unlogged tmp = treeType::ExtractMin();
			return tmp->getDataPointer();
		}

		pointer Find(const_input_reference item) {
			return treeType::Find(item);
		}

		////////////////////////////////////////////////////////////////////////
		// Glue Layer Integration
		////////////////////////////////////////////////////////////////////////
		bool exists(element_pointer_unlogged x) {
			if (!treeType::empty()) {
				iterator_unlogged it1(this, treeType::getFirstElement(), instance_id_);
				iterator_unlogged it2(this, treeType::getEnd(), instance_id_);
				for (iterator_unlogged it = it1; it != it2; it++) {
					if (it.getData() == x)
						return true;
				}
			}
			return false;
		}
	};
}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif
