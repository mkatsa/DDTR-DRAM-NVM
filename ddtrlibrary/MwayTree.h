#ifndef MWAYTREE_H
#define MWAYTREE_H

#include "DDTIncludes.h"
// This is a test comment
////////////////////////////////////////////////////////////////////////////////
// Begin namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
namespace DDTLibrary {

	template <class treeType> class MwayTreeExtension;

	template <class treeNode> class MwayTreeNodeExtension;

	//Tree Node Adapters
	///////////////////////////////////////////////////////////////
	template <class treeNode>
	class MwayTreeNodeExtension : public treeNode {
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

		typedef typename treeNode::objectTraits::template transform<MwayTreeNodeExtension*>::value_type element_pointer_unlogged;
		typedef typename treeNode::objectTraits::template transform<MwayTreeNodeExtension&>::value_type element_reference_unlogged;

		typedef typename treeNode::ddtTraits::template transform<MwayTreeNodeExtension*>::value_type element_pointer;
		typedef typename treeNode::ddtTraits::template transform<MwayTreeNodeExtension&>::value_type element_reference;

		typedef typename treeNode::value_pointer value_pointer;

		typedef typename treeNode::size_type size_type;
		typedef typename treeNode::difference_type difference_type;

		// ---------------- Construction and destruction ---------------

		MwayTreeNodeExtension () {
			_left.setData(this);
			_right.setData(this);
			_parent.setData(this);
			_subtreeSize = 0;
			_data = (pointer) Allocator::malloc(sizeof(value_type));
			_color = BLACK;
			_subtreeSize = 0;
		}

		MwayTreeNodeExtension (const_input_reference data, element_pointer_unlogged sentinel, element_pointer_unlogged parent) {
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
		friend RedBlackTreeExtension<BinaryTree<RedBlackTreeNodeExtension>>;
		element_pointer _left, _right, _parent;
		typename treeNode::ddtTraits::template transform<short>::value_type _color;

	};

	///////////////////////////////////////////////////////////////
	// Tree Base Extensions
	///////////////////////////////////////////////////////////////
	template <class treeType> 
	class MwayTreeExtension : public treeType {
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

		typedef typename treeType::element_pointer_unlogged element_pointer_unlogged;
		typedef typename treeType::element_reference_unlogged element_reference_unlogged;

		typedef typename treeType::element_pointer element_pointer;
		typedef typename treeType::element_reference element_reference;

		typedef typename treeType::size_type size_type;
		typedef typename treeType::difference_type difference_type;


		// ---------------- Construction and destruction ---------------

		MwayTreeExtension () {}
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

}
////////////////////////////////////////////////////////////////////////////////
// End namespace DDTLibrary
////////////////////////////////////////////////////////////////////////////////
#endif
