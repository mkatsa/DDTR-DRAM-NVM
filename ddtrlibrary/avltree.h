//
//  avltree.h
//  ddtrLibrary
//
//  Created by Christos Baloukas on 2/6/21.
//

#ifndef avltree_h
#define avltree_h

namespace DDTLibrary {
    
    int max(int a, int b)
    {
        return (a > b)? a : b;
    }
    
    struct avlTreeNodeBase
    {
        typedef avlTreeNodeBase* basePtr;
        typedef const avlTreeNodeBase* constBasePtr;
        
        int height;
        basePtr         parent;
        basePtr         left;
        basePtr         right;

        static basePtr
        minimum(basePtr _x)
        {
            while (_x->left != 0)
                _x = _x->left;
            return _x;
        }

        static constBasePtr
        minimum(constBasePtr _x)
        {
            while (_x->left != 0)
                _x = _x->left;
            return _x;
        }

        static basePtr
        maximum(basePtr _x)
        {
            while (_x->right != 0)
                _x = _x->right;
            return _x;
        }

        static constBasePtr
        maximum(constBasePtr _x)
        {
            while (_x->right != 0)
                _x = _x->right;
            return _x;
        }
        
        int getHeight() {
            return height;
        }
    };
    
    int getBalance(avlTreeNodeBase *N)
    {
        if (N == NULL)
            return 0;
        int leftHeight = 0;
        int rightHeight = 0;
        if (N->left) {
            leftHeight = N->left->height;
        }
        if (N->right) {
            rightHeight = N->right->height;
        }
        return  leftHeight -
            rightHeight;
    }
    
    template<typename _KeyCompare>
      struct avlTreeKeyCompare
      {
          _KeyCompare        keyCompare;

          avlTreeKeyCompare() : keyCompare()
          { }

          avlTreeKeyCompare(const _KeyCompare& _comp): keyCompare(_comp)
          { }

  
          // Copy constructor added for consistency with C++98 mode.
          avlTreeKeyCompare(const avlTreeKeyCompare&) = default;

          avlTreeKeyCompare(avlTreeKeyCompare&& _x): keyCompare(_x.keyCompare)
          { }
  
      };
    
    struct avlTreeHeader {
        
        avlTreeNodeBase     header;
        size_t              nodeCount; //keeps track of size of tree
        
        avlTreeHeader() {
            reset();
        }
        
        avlTreeHeader(avlTreeHeader&& _x) {
            if (_x.header.parent != nullptr)
                moveData(_x);
            else {
                reset();
            }
        }
        
        void moveData(avlTreeHeader& _from) {
            header.parent = _from.header.parent;
            header.left = _from.header.left;
            header.right = _from.header.right;
            header.parent = _from.header.parent;
            nodeCount = _from.nodeCount;
            
            _from.reset();
        }
        
        void reset() {
            header.parent = 0;
            header.height = 0;
            header.left = &header;
            header.right = &header;
            nodeCount = 0;
        }
    };
    
    template<typename _Val>
    struct avlTreeNode : public avlTreeNodeBase {
        
        typedef avlTreeNode<_Val>* linkType;
        
        _Val valueField;
        
        _Val*
        valPtr() {
            return std::addressof(valueField);
        }
        
        const _Val*
        valPtr() const {
            return std::addressof(valueField);
        }
        
    };
    
    avlTreeNodeBase*
    localAvlTreeIncrement(avlTreeNodeBase* _x) {
        if (_x->right != 0) {
            _x = _x->right;
            while (_x->left != 0)
                _x = _x->left;
        }
        else {
            avlTreeNodeBase* _y = _x->parent;
            while (_x == _y->right) {
                _x = _y;
                _y = _y->parent;
            }
            if (_x->right != _y)
                _x = _y;
        }
        
        return _x;
    }
    
    avlTreeNodeBase*
    avlTreeIncrement(avlTreeNodeBase* _x) {
        return localAvlTreeIncrement(_x);
    }
    
    const avlTreeNodeBase*
    avlTreeIncrement(const avlTreeNodeBase* _x) {
        return localAvlTreeIncrement(const_cast<avlTreeNodeBase*>(_x));
    }
    
    avlTreeNodeBase*
    localAvlTreeDecrement(avlTreeNodeBase* _x) {
        if (_x->left != 0) {
            avlTreeNodeBase* _y = _x->left;
            while (_y->right != 0)
                _y = _y->right;
            _x = _y;
        }
        else {
            avlTreeNodeBase* _y = _x->parent;
            while (_x == _y->left) {
                _x = _y;
                _y = _y->parent;
            }
            _x = _y;
        }
        return _x;
    }
    
    avlTreeNodeBase*
    avlTreeDecrement(avlTreeNodeBase* _x) {
        return localAvlTreeDecrement(_x);
    }
    
    const avlTreeNodeBase*
    avlTreeDecrement(const avlTreeNodeBase* _x) {
        return localAvlTreeDecrement(const_cast<avlTreeNodeBase*>(_x));
    }
    
    void
    localAvlTreeRotateLeft(avlTreeNodeBase* const _x, avlTreeNodeBase*& _root) {
        avlTreeNodeBase* const _y = _x->right;
        
        // Perform rotation
        _x->right = _y->left;
        if (_y->left != 0)
            _y->left->parent = _x;
        _y->parent = _x->parent;
        
        if (_x == _root)
            _root = _y;
        else if (_x == _x->parent->left)
            _x->parent->left = _y;
        else
            _x->parent->right = _y;
        _y->left = _x;
        _x->parent = _y;
        
        // Update heights
        _x->height = max(_x->left->height, _x->right->height) + 1;
        _y->height = max(_y->left->height, _y->right->height) + 1;
    }
    
    void
    localAvlTreeRotateRight(avlTreeNodeBase* const _x, avlTreeNodeBase*& _root) {
        avlTreeNodeBase* const _y = _x->left;
        
        _x->left = _y->right;
        if (_y->right != 0)
            _y->right->parent = _x;
        _y->parent = _x->parent;
        
        if (_x == _root)
            _root = _y;
        else if (_x == _x->parent->right)
            _x->parent->right = _y;
        else
            _x->parent->left = _y;
        _y->right = _x;
        _x->parent = _y;
    }
    
    avlTreeNodeBase*
    avlTreeRebalanceForErase(avlTreeNodeBase* const _z, avlTreeNodeBase& _header) {
        
        avlTreeNodeBase *& _root = _header.parent;
        avlTreeNodeBase *& _leftmost = _header.left;
        avlTreeNodeBase *& _rightmost = _header.right;
        avlTreeNodeBase* _y = _z;
        avlTreeNodeBase* _x = 0;
        avlTreeNodeBase* _xParent = 0;
        
        if (_y->left == 0)
            _x = _y->right;
        else {
            if (_y->right == 0)
                _x = _y->left;
            else {
                _y = _y->right;
                while (_y->left != 0)
                    _y = _y->left;
                _x = _y->right;
            }
        }
        if (_y != _z) {
            _z->left->parent = _y;
            _y->left = _z->left;
            if (_y != _z->right) {
                _xParent = _y->parent;
                if (_x)
                    _x->parent = _y->parent;
                _y->parent->left = _x;
                _y->right  = _z->right;
                _z->right->parent = _y;
            }
            else
                _xParent = _y;
            if (_root == _z)
                _root = _y;
            else if (_z->parent->left == _z)
                _z->parent->left = _y;
            else
                _z->parent->right = _y;
            _y->parent = _z->parent;
            
            _y = _z; //_y now points to the node to be deleted
        } else {
            // _y == _z
            _xParent = _y->parent;
            if (_x)
                _x->parent = _y->parent;
            if (_root == _z)
                _root = _x;
            else
                if (_z->parent->left == _z)
                    _z->parent->left = _x;
                else
                    _z->parent->right = _x;
            if (_leftmost == _z) {
                if (_z->right == 0)
                    _leftmost = _z->parent; //make _leftmost == header if _z == _root
                else
                    _leftmost = avlTreeNodeBase::minimum(_x);
            }
            if (_rightmost == _z) {
                if (_z->left == 0)
                    _rightmost = _z->parent;
                else
                    _rightmost = avlTreeNodeBase::maximum(_x);
            }
                
        }
        
        //-----------------------------
        
        
        
        while (_x->parent != _root) {
            
            // get the height of the current node
            _x->height = 1 + max(_x->left->height, _x->right->height);
            
            //GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
            int balance = getBalance(_x);
            
            
            // If this node becomes unbalanced,
            // then there are 4 cases

            // Left Left Case
            if (balance > 1 && getBalance(_x->left) >= 0)
                localAvlTreeRotateRight(_x, _root);
            
            // Left Right Case
            if (balance > 1 && getBalance(_x->left) < 0) {
                localAvlTreeRotateLeft(_x->left, _root);
                localAvlTreeRotateRight(_x, _root);
                
            }
            
            // Right Right Case
            if (balance < -1 && getBalance(_x->right) <= 0)
                localAvlTreeRotateLeft(_x, _root);
                
            
            // Right Left Case
            if (balance < -1 && getBalance(_x->right) > 0) {
                localAvlTreeRotateRight(_x->right, _root);
                localAvlTreeRotateLeft(_x, _root);
            }
            
            _x = _x->parent;
        }
        
        if (_x->parent == _root) {
            // get the height of the current node
            _x->height = 1 + max(_x->left->height, _x->right->height);
            
            //GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
            int balance = getBalance(_x);
            
            
            // If this node becomes unbalanced,
            // then there are 4 cases

            // Left Left Case
            if (balance > 1 && getBalance(_x->left) >= 0)
                localAvlTreeRotateRight(_x, _root);
            
            // Left Right Case
            if (balance > 1 && getBalance(_x->left) < 0) {
                localAvlTreeRotateLeft(_x->left, _root);
                localAvlTreeRotateRight(_x, _root);
                
            }
            
            // Right Right Case
            if (balance < -1 && getBalance(_x->right) <= 0)
                localAvlTreeRotateLeft(_x, _root);
                
            
            // Right Left Case
            if (balance < -1 && getBalance(_x->right) > 0) {
                localAvlTreeRotateRight(_x->right, _root);
                localAvlTreeRotateLeft(_x, _root);
            }
        }
        
        return _x;
        
        //-----------------------------
    
    }
    
    
    
    
    
    
    
    
    
    template<typename _Tp>
    struct avlTreeIterator {
        typedef _Tp valueType;
        typedef _Tp& reference;
        typedef _Tp* pointer;
        
        typedef std::bidirectional_iterator_tag     iteratorCategory;
        typedef ptrdiff_t                           differenceType;
        
        typedef avlTreeIterator<_Tp>                _self;
        typedef avlTreeNodeBase::basePtr            basePtr;
        typedef avlTreeNode<_Tp>*                   linkType;
        
        avlTreeIterator(): node() {
        }
        
        explicit
        avlTreeIterator(basePtr _x): node(_x) {
        }
        
        reference
        operator* () const {
            return *static_cast<linkType>(node)->valPtr();
        }
        
        pointer
        operator->() const {
            return static_cast<linkType>(node)->valPtr();
        }
        
        _self& operator++() {
            node = avlTreeIncrement(node);
            return *this;
        }
        _self
        operator++(int) {
            _self _tmp = *this;
            node = avlTreeIncrement(node);
            return _tmp;
        }
        _self&
        operator--() {
            node = avlTreeDecrement(node);
            return *this;
        }
        _self
        operator--(int) {
            _self _tmp = *this;
            node = avlTreeDecrement(node);
            return _tmp;
        }
        friend bool
        operator==(const _self& _x, const _self& _y) {
            return _x.node == _y.node;
        }
        friend bool
        operator!=(const _self& _x, const _self& _y) {
            return _x.node != _y.node;
        }
        
        basePtr node;
        
    };
    
    template<typename _Tp>
    struct avlTreeConstIterator {
        
        typedef _Tp valueType;
        typedef const _Tp& reference;
        typedef const _Tp* pointer;
        
        typedef avlTreeIterator<_Tp>                iterator;
        
        typedef std::bidirectional_iterator_tag     iteratorCategory;
        typedef ptrdiff_t                           differenceType;
        
        typedef avlTreeConstIterator<_Tp>           _self;
        typedef avlTreeNodeBase::constBasePtr       basePtr;
        typedef const avlTreeNode<_Tp>*             linkType;
        
        avlTreeConstIterator(): node() {
        }
        
        explicit
        avlTreeConstIterator(basePtr _x): node(_x) {
        }
        
        
        avlTreeConstIterator(const iterator& _it): node(_it.node) {
        }
        
        iterator
        constCast() const {
            return iterator(const_cast<typename iterator::basePtr>(node));
        }
        
        reference
        operator* () const {
            return *static_cast<linkType>(node)->valPtr();
        }
        
        pointer
        operator->() const {
            return static_cast<linkType>(node)->valPtr();
        }
        
        _self& operator++() {
            node = avlTreeIncrement(node);
            return *this;
        }
        _self
        operator++(int) {
            _self _tmp = *this;
            node = avlTreeIncrement(node);
            return _tmp;
        }
        _self&
        operator--() {
            node = avlTreeDecrement(node);
            return *this;
        }
        _self
        operator--(int) {
            _self _tmp = *this;
            node = avlTreeDecrement(node);
            return _tmp;
        }
        friend bool
        operator==(const _self& _x, const _self& _y) {
            return _x.node == _y.node;
        }
        friend bool
        operator!=(const _self& _x, const _self& _y) {
            return _x.node != _y.node;
        }
        
        basePtr node;
        
    };
    
    template<typename _key, typename _val, typename _keyOfValue, typename _compare, typename _alloc = std::allocator<_val> >
    class AVLTree {
        
        typedef typename _alloc::template
      rebind<avlTreeNode<_val> >::other nodeAllocator;
        typedef std::allocator_traits<nodeAllocator> allocTraits;
        
    protected:
        typedef avlTreeNodeBase*                basePtr;
        typedef const avlTreeNodeBase*          constBasePtr;
        typedef avlTreeNode<_val>*              linkType;
        typedef const avlTreeNode<_val>*        constLinkType;
        
    public:
        typedef _key                    keyType;
        typedef _val                    valueType;
        typedef valueType*              pointer;
        typedef const valueType*        constPointer;
        typedef valueType&              reference;
        typedef const valueType&        constReference;
        typedef size_t                  sizeType;
        typedef ptrdiff_t               differenceType;
        typedef _alloc                  allocatorType;
        
        size_t
        nodeCount() {
            return avlImpl.nodeCount;
        }
        
        nodeAllocator&
        getNodeAllocator() {
            return this->avlImpl;
        }
        const nodeAllocator&
        getNodeAllocator() const {
            return this->avlImpl;
        }
        
    protected:
        
        linkType
        getNode() {
            return allocTraits::allocate(getNodeAllocator(), 1);
        }
        
        void
        putNode(linkType _p) {
            allocTraits::deallocate(getNodeAllocator(), _p, 1);
        }
        
        template<typename... _Args>
        void
        constructNode(linkType _node, _Args&&... _args) {
            try {
                ::new(_node) avlTreeNode<_val>;
                allocTraits::construct(getNodeAllocator(), _node->valPtr(), std::forward<_Args>(_args)...);
            } catch (...) {
                _node->~avlTreeNode<_val>();
                putNode(_node);
                throw;
            }
        }
        
        template<typename... _Args>
        linkType
        createNode(_Args&&... _args) {
            linkType _tmp = getNode();
            constructNode(_tmp, std::forward<_Args>(_args)...);
            return _tmp;
        }
        
        void
        destroyNode(linkType _p) {
            allocTraits::destroy(getNodeAllocator(), _p->valPtr());
            _p->~avlTreeNode<_val>();
        }
        
        void
        dropNode(linkType _p) {
            destroyNode(_p);
            putNode(_p);
        }
        
        template<typename _keyCompare>
        struct avlTreeImpl
        : public nodeAllocator
        , public avlTreeKeyCompare<_keyCompare>
        , public avlTreeHeader {
            
            typedef avlTreeKeyCompare<_keyCompare> baseKeyCompare;
            
            avlTreeImpl(): nodeAllocator()
            {}
            
            avlTreeImpl(const avlTreeImpl& _x)
            : nodeAllocator(allocTraits::selectOnCopy(_x))
            , baseKeyCompare(_x.keyCompare)
            , avlTreeHeader()
            {}
            
            avlTreeImpl(avlTreeImpl&&) = default;
            
            explicit
            avlTreeImpl(nodeAllocator&& _a)
            : nodeAllocator(std::move(_a)) {}
            
            avlTreeImpl(avlTreeImpl&& _x, nodeAllocator&& _a)
            : nodeAllocator(std::move(_a))
            , baseKeyCompare(std::move(_x))
            , avlTreeHeader(std::move(_x))
            {}
            
            avlTreeImpl(const _keyCompare& _comp, nodeAllocator&& _a)
            : nodeAllocator(std::move(_a))
            , baseKeyCompare(_comp)
            {}
            
        };
        
        avlTreeImpl<_compare> avlImpl;
        
    protected:
        
        static const _key&
        key(constLinkType _x) {
            return _keyOfValue()(*_x->valPtr());
        }
        
        static linkType
        left(basePtr _x) {
            return static_cast<linkType>(_x->left);
        }
        
        static constLinkType
        left(constBasePtr _x) {
            return static_cast<constLinkType>(_x->left);
        }
        
        static linkType
        right(basePtr _x) {
            return static_cast<linkType>(_x->right);
        }
        
        static constLinkType
        right(constBasePtr _x) {
            return static_cast<constLinkType>(_x->right);
        }
        
        static const _key&
        key(constBasePtr _x) {
            return key(static_cast<constLinkType>(_x));
        }
        
        basePtr&
        root() {
            return this->avlImpl.header.parent;
        }
        
        constBasePtr
        root() const {
            return this->avlImpl.header.parent;
        }
        
        basePtr&
        leftmost() {
            return this->avlImpl.header.left;
            
        }

        constBasePtr
        leftmost() const {
            return this->avl.header.left;
        }
        
        basePtr&
        rightmost() {
            return this->avlImpl.header.right;
        }
        
        constBasePtr
        rightmost() const {
            return this->avlImpl.header.right;
        }
        
        linkType
        mbegin() const {
            return static_cast<linkType>(this->avlImpl.header.parent);
        }
        
        linkType
        _M_begin() {
            return mbegin();
        }

        constLinkType
        _M_begin() const {
            return static_cast<constLinkType>(this->avlImpl.header.parent);
        }

        basePtr
        _M_end() {
            return &this->avlImpl.header;
        }

        constBasePtr
        _M_end() const {
            return &this->avlImpl.header;
        }
        
    public:
        typedef avlTreeIterator<valueType>              iterator;
        typedef avlTreeConstIterator<valueType>         constIterator;
        
        typedef std::reverse_iterator<iterator>         reverseIterator;
        typedef std::reverse_iterator<constIterator>    constReverseIterator;
        
        std::pair<basePtr, basePtr>
        getInsertUniquePos(const keyType& _k) {
            
            typedef std::pair<basePtr, basePtr> _Res;
            linkType _x = _M_begin();
            basePtr _y = _M_end();
            bool _comp = true;
            
            while (_x != 0) {
                _y = _x;
                _comp = avlImpl.keyCompare(_k, key(_x));
                _x = _comp ? left(_x) : right(_x);
            }
            iterator _j = iterator(_y);
            if (_comp) {
                if (_j == begin())
                    return _Res(_x, _y);
                else
                    --_j;
            }
            if (avlImpl.keyCompare(key(_j.node), _k))
                return _Res(_x, _y);
            
            return _Res(_j.node, 0);
        }
        
        std::pair<basePtr, basePtr>
        getInsertHintUniquePos(constIterator _position, const keyType& _k) {
            iterator _pos = _position.constCast();
            typedef std::pair<basePtr, basePtr> _Res;
            
            // end()
            if (_pos.node == _M_end()) {
                if (size() > 0 && avlImpl.keyCompare(key(rightmost()), _k))
                    return _Res(0, rightmost());
                else
                    return getInsertUniquePos(_k);
            }
            else if (avlImpl.keyCompare(_k, key(_pos.node))) {
                // First, try before..
                iterator _before = _pos;
                if (_pos.node == leftmost()) // begin()
                    return _Res(leftmost(), leftmost());
                else if (avlImpl.keyCompare(key((--_before).node), _k)) {
                    if (right(_before.node) == 0)
                        return _Res(0, _before.node);
                    else
                        return _Res(_pos.node, _pos.node);
                }
                else
                    return getInsertUniquePos(_k);
            }
            else if (avlImpl.keyCompare(key(_pos.node), _k)) {
                
                //... then try after
                iterator _after = _pos;
                if (_pos.node == rightmost())
                    return _Res(0, rightmost());
                else if (avlImpl.keyCompare(_k, key((++_after).node))) {
                    if (right(_pos.node) == 0)
                        return _Res(0, _pos.node);
                    else
                        return _Res(_after.node, _after.node);
                } else
                    return getInsertUniquePos(_k);
            } else
                //equivalent keys.
                return _Res(_pos.node, 0);
        }
        
    private:
        
        void
        avlTreeInsertAndRebalance(const bool _insertLeft, avlTreeNodeBase* _x, avlTreeNodeBase* _p, avlTreeNodeBase& _header) {
            
            avlTreeNodeBase *& _root = _header.parent;
            
            // Initialize fields in new node to insert.
            _x->parent = _p;
            _x->left = 0;
            _x->right = 0;
            _x->height = 1;
            
            // Insert.
            // Make new node child of parent and maintain root, leftmost and
            // rightmost nodes.
            // N.B. First node is always inserted left.
            
            if (_insertLeft) {
                _p->left = _x; // also makes leftmost = _x when _p = &_header
                
                if (_p == &_header) {
                    _header.parent = _x;
                    _header.right = _x;
                } else if (_p == _header.left) {
                    _header.left = _x; //maintain leftmost pointing to min node
                }
            }
            else {
                _p->right = _x;
                
                if (_p == _header.right)
                    _header.right = _x; //maintain rightmost pointing to max node
            }
            
            // Rebalance
            while (_x != _root) {
                int leftHeight = 0;
                int rightHeight = 0;
                if(_x->left) {
                    leftHeight = _x->left->height;
                }
                if(_x->right) {
                    rightHeight = _x->right->height;
                }
                _x->height = 1 + max(leftHeight, rightHeight);
                
                //Get the balance factor of this ancestor node to check whether this node became unbalanced
                int _balance = getBalance(_x);
                
                //If this node became unbalanced then there are 4 cases
                
                //Left Left Case
                if (_balance > 1 && key(_x) < key(_x->left))
                    localAvlTreeRotateRight(_x, _root);
                
                // Right Right Case
                if (_balance < -1 && key(_x) > key(_x->right))
                    localAvlTreeRotateLeft(_x, _root);
                
                // Left Right Case
                if (_balance > 1 && key(_x) > key(_x->left)) {
                    localAvlTreeRotateLeft(_x->left, _root);
                    localAvlTreeRotateRight(_x, _root);
                }
                
                // Right Left Case
                if (_balance < -1 && key(_x) < key(_x->right))
                {
                    localAvlTreeRotateRight(_x->right, _root);
                    localAvlTreeRotateLeft(_x, _root);
                }
                
                _x = _x->parent;
            }
            if (_x == _root) {
                
                int leftHeight = 0;
                int rightHeight = 0;
                if(_x->left) {
                    leftHeight = _x->left->height;
                }
                if(_x->right) {
                    rightHeight = _x->right->height;
                }
                _x->height = 1 + max(leftHeight, rightHeight);
                
                //Get the balance factor of this ancestor node to check whether this node became unbalanced
                int _balance = getBalance(_x);
                
                //If this node became unbalanced then there are 4 cases
                
                //Left Left Case
                if (_balance > 1 && key(_x) < key(_x->left))
                    localAvlTreeRotateRight(_x, _root);
                
                // Right Right Case
                if (_balance < -1 && key(_x) > key(_x->right))
                    localAvlTreeRotateLeft(_x, _root);
                
                // Left Right Case
                if (_balance > 1 && key(_x) > key(_x->left)) {
                    localAvlTreeRotateLeft(_x->left, _root);
                    localAvlTreeRotateRight(_x, _root);
                }
                
                // Right Left Case
                if (_balance < -1 && key(_x) < key(_x->right))
                {
                    localAvlTreeRotateRight(_x->right, _root);
                    localAvlTreeRotateLeft(_x, _root);
                }
            }
            
        }
        
        template<typename _Arg, typename _nodeGen>
        iterator
        insert(basePtr _x, basePtr _p, _Arg&& _v, _nodeGen& __nodeGen) {
            
            bool _insertLeft = (_x != 0 || _p = end() || avlImpl.keyCompare(_keyOfValue()(_v), key(_p)));
            linkType _z = __nodeGen(std::forward<_Arg>(_v));
            
            avlTreeInsertAndRebalance(_insertLeft, _z, _p, this->avlImpl.header);
            ++avlImpl.nodeCount;
            
            return iterator(_z);
            
        }
        
        iterator
        insertNode(basePtr _x, basePtr _p, linkType _z) {
            
            bool insertLeft = (_x != 0 || _p == _M_end() || avlImpl.keyCompare(key(_z), key(_p)));

            avlTreeInsertAndRebalance(insertLeft, _z, _p, this->avlImpl.header);
            ++avlImpl.nodeCount;
            return iterator(_z);
        }
        
        void
        erase(linkType _x) {
            // Erase without rebalancing
            while (_x != 0) {
                erase(right(_x));
                linkType _y = left(_x);
                dropNode(_x);
                _x = _y;
            }
        }

        iterator
        _M_lowerBound(linkType _x, basePtr _y, const _key& _k) {
            while (_x != 0) {
                if (!avlImpl.keyCompare(key(_x), _k))
                    _y = _x, _x = left(_x);
                else
                    _x = right(_x);
            }
            return iterator(_y);
        }

        constIterator
        _M_lowerBound(constLinkType _x, constBasePtr _y, const _key& _k) const {
            while (_x != 0) {
                if (!avlImpl.keyCompare(key(_x), _k))
                    _y = _x, _x = left(_x);
                else
                    _x = right(_x);
            }
            return constIterator(_y);
        }

        iterator
        upperBound(linkType _x, basePtr _y,
               const _key& _k);

        constIterator
        upperBound(constLinkType _x, constBasePtr _y,
               const _key& _k) const;
        
    public:
        
        AVLTree() = default;
        
        AVLTree(const _compare& _comp, const allocatorType& _a = allocatorType())
        : avlImpl(_comp, nodeAllocator(_a)) {}
        
        AVLTree(const AVLTree& _x)
        : avlImpl(_x.avlImpl) {
            if (_x.root() != 0)
                root() = copy(_x);
        }
        
        ~AVLTree() {
            erase(_M_begin());
        }
        
        _compare
        keyComp() const {
            return avlImpl.keyCompare;
        }
        
        iterator
        begin() {
            return iterator(this->avlImpl.header.left);
        }
        
        constIterator
        begin() const {
            return constIterator(this->avlImpl.header.left);
        }
        
        iterator
        end() {
            return iterator(&this->avlImpl.header);
        }
        
        constIterator
        end() const {
            return constIterator(&this->avlImpl.header);
        }
        
        bool
        empty() const {
            return avlImpl.nodeCount == 0;
        }
        
        sizeType
        size() const {
            return avlImpl.nodeCount;
        }
        
        template<typename _Arg>
        std::pair<iterator, bool>
        insertUnique(_Arg&& _x);
        
        template<typename... _Args>
        iterator
        emplaceHintUnique(constIterator _pos, _Args&&... _args) {
            linkType _z = createNode(std::forward<_Args>(_args)...);
            
            try {
                auto _res = getInsertHintUniquePos(_pos, key(_z));
                
                if (_res.second)
                    return insertNode(_res.first, _res.second, _z);
                
                dropNode(_z);
                return iterator(_res.first);
            }
            catch(...)
            {
                dropNode(_z);
                throw;
            }
        }
        
    private:
        
        void
        eraseAux(constIterator _position) {
            linkType _y = static_cast<linkType>(avlTreeRebalanceForErase(const_cast<basePtr>(_position.node), this->avlImpl.header));
            dropNode(_y);
            --avlImpl.nodeCount;
        }
        
        void
        eraseAux(constIterator _first, constIterator _last) {
            if (_first == begin() && _last == end())
                clear();
            else
                while (_first != _last)
                    eraseAux(_first++);
        }
        
    public:
        
        iterator
        erase(constIterator _position) {
            assert(_position != end());
            constIterator _result = _position;
            ++_result;
            eraseAux(_position);
            
            return _result.constCast();
        }
        
        iterator
        erase(iterator _position) {
            assert(_position != end());
            iterator _result = _position;
            ++_result;
            eraseAux(_position);
            
            return _result;
        }
        
        iterator
        erase(constIterator _first, constIterator _last) {
            eraseAux(_first, _last);
            return _last.constCast();
        }
        
        void
        clear() {
            erase(begin());
            avlImpl.reset();
        }
        
        iterator
        find(const keyType& _k) {
            iterator _j = lowerBound(begin(), end(), _k);
            return (_j == end() || avlImpl.keyCompare(_k, key(_j.node))) ? end() : _j;
        }
        
        constIterator
        find(const keyType& _k) const {
            constIterator _j = lowerBound(begin(), end(), _k);
            return (_j == end() || avlImpl.keyCompare(_k, key(_j.node))) ? end() : _j;
        }
        
        
        
        iterator
        lowerBound(const keyType& _k) {
            return _M_lowerBound(_M_begin(), _M_end(), _k);
        }
        
        constIterator
        lowerBound(const keyType& _k) const {
            return _M_lowerBound(_M_begin(), _M_end(), _k);
        }
        
        template<typename _kt>
        iterator
        findTr(const _kt& _k) {
            const AVLTree& _constThis = this;
            return _constThis->findTr(_k).constCast();
        }
        
        template<typename _kt>
        constIterator
        findTr(const _kt& _k) const {
            auto _j = lowerBoundTr(_k);
            if (_j != end() && avlImpl.keyCompare(_k, key(_j.node)))
                _j = end();
            return _j;
        }
        
        template<typename _kt>
        iterator
        lowerBoundTr(const _kt& _k) {
            const AVLTree* _constThis = this;
            return _constThis->lowerBoundTr(_k).constCast();
        }
        
        template<typename _kt>
        constIterator
        lowerBoundTr(const _kt& _k) const {
            auto _x = begin();
            auto _y = end();
            while (_x != 0) {
                if (!avlImpl.keyCompare(key(_x), _k)) {
                    _y = _x;
                    _x = left(_x);
                }
                else {
                    _x = right(_x);
                }
            }
            return constIterator(_y);
        }
        
    };
    
}
#endif /* avltree_h */
