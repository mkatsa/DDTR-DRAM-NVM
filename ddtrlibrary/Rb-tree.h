#ifndef RB_TREE_H
#define RB_TREE_H

#include <type_traits>

namespace DDTLibrary {
    
    
    enum _Rb_tree_color { _S_red = false, _S_black = true };

    struct _Rb_tree_node_base
    {
      typedef _Rb_tree_node_base* _Base_ptr;
      typedef const _Rb_tree_node_base* _Const_Base_ptr;

      _Rb_tree_color    _M_color;
      _Base_ptr        _M_parent;
      _Base_ptr        _M_left;
      _Base_ptr        _M_right;

      static _Base_ptr
      _S_minimum(_Base_ptr __x)
      {
        while (__x->_M_left != 0) __x = __x->_M_left;
        return __x;
      }

      static _Const_Base_ptr
      _S_minimum(_Const_Base_ptr __x)
      {
        while (__x->_M_left != 0) __x = __x->_M_left;
        return __x;
      }

      static _Base_ptr
      _S_maximum(_Base_ptr __x)
      {
        while (__x->_M_right != 0) __x = __x->_M_right;
        return __x;
      }

      static _Const_Base_ptr
      _S_maximum(_Const_Base_ptr __x)
      {
        while (__x->_M_right != 0) __x = __x->_M_right;
        return __x;
      }
    };
    
    template<typename _Key_compare>
      struct _Rb_tree_key_compare
      {
        _Key_compare        _M_key_compare;

        _Rb_tree_key_compare()
        : _M_key_compare()
        { }

        _Rb_tree_key_compare(const _Key_compare& __comp)
        : _M_key_compare(__comp)
        { }

  #if __cplusplus >= 201103L
        // Copy constructor added for consistency with C++98 mode.
        _Rb_tree_key_compare(const _Rb_tree_key_compare&) = default;

        _Rb_tree_key_compare(_Rb_tree_key_compare&& __x)
        : _M_key_compare(__x._M_key_compare)
        { }
  #endif
      };
    
    // Helper type to manage default initialization of node count and header.
    struct _Rb_tree_header
    {
      _Rb_tree_node_base    _M_header;
      size_t        _M_node_count; // Keeps track of size of tree.

      _Rb_tree_header()
      {
        _M_header._M_color = _S_red;
        _M_reset();
      }

  #if __cplusplus >= 201103L
      _Rb_tree_header(_Rb_tree_header&& __x)
      {
        if (__x._M_header._M_parent != nullptr)
      _M_move_data(__x);
        else
      {
        _M_header._M_color = _S_red;
        _M_reset();
      }
      }
  #endif

      void
      _M_move_data(_Rb_tree_header& __from)
      {
        _M_header._M_color = __from._M_header._M_color;
        _M_header._M_parent = __from._M_header._M_parent;
        _M_header._M_left = __from._M_header._M_left;
        _M_header._M_right = __from._M_header._M_right;
        _M_header._M_parent->_M_parent = &_M_header;
        _M_node_count = __from._M_node_count;

        __from._M_reset();
      }

      void
      _M_reset()
      {
        _M_header._M_parent = 0;
        _M_header._M_left = &_M_header;
        _M_header._M_right = &_M_header;
        _M_node_count = 0;
      }
        
    
    };
    
    template<typename _Val>
      struct _Rb_tree_node : public _Rb_tree_node_base
      {
        typedef _Rb_tree_node<_Val>* _Link_type;

  
        _Val _M_value_field;

        _Val*
        _M_valptr()
          { return std::addressof(_M_value_field); }

        const _Val*
        _M_valptr() const
        { return std::addressof(_M_value_field); }
 
      };
    
    static _Rb_tree_node_base*
    local_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
    {
      if (__x->_M_right != 0)
        {
          __x = __x->_M_right;
          while (__x->_M_left != 0)
            __x = __x->_M_left;
        }
      else
        {
          _Rb_tree_node_base* __y = __x->_M_parent;
          while (__x == __y->_M_right)
            {
              __x = __y;
              __y = __y->_M_parent;
            }
          if (__x->_M_right != __y)
            __x = __y;
        }
      return __x;
    }
    
    
    
    
     _Rb_tree_node_base*
    _Rb_tree_increment(_Rb_tree_node_base* __x) throw () {
        return local_Rb_tree_increment(__x);
    }

     const _Rb_tree_node_base*
    _Rb_tree_increment(const _Rb_tree_node_base* __x) throw () {
        return local_Rb_tree_increment(const_cast<_Rb_tree_node_base*>(__x));
    }
    
    static _Rb_tree_node_base*
    local_Rb_tree_decrement(_Rb_tree_node_base* __x) throw ()
    {
      if (__x->_M_color == _S_red
          && __x->_M_parent->_M_parent == __x)
        __x = __x->_M_right;
      else if (__x->_M_left != 0)
        {
          _Rb_tree_node_base* __y = __x->_M_left;
          while (__y->_M_right != 0)
            __y = __y->_M_right;
          __x = __y;
        }
      else
        {
          _Rb_tree_node_base* __y = __x->_M_parent;
          while (__x == __y->_M_left)
            {
              __x = __y;
              __y = __y->_M_parent;
            }
          __x = __y;
        }
      return __x;
    }

    _Rb_tree_node_base*
    _Rb_tree_decrement(_Rb_tree_node_base* __x) throw () {
        return local_Rb_tree_decrement(__x);
    }

     const _Rb_tree_node_base*
    _Rb_tree_decrement(const _Rb_tree_node_base* __x) throw () {
        return local_Rb_tree_decrement(const_cast<_Rb_tree_node_base*>(__x));
    }
    
    static void
    local_Rb_tree_rotate_left(_Rb_tree_node_base* const __x,
                       _Rb_tree_node_base*& __root)
    {
      _Rb_tree_node_base* const __y = __x->_M_right;

      __x->_M_right = __y->_M_left;
      if (__y->_M_left !=0)
        __y->_M_left->_M_parent = __x;
      __y->_M_parent = __x->_M_parent;

      if (__x == __root)
        __root = __y;
      else if (__x == __x->_M_parent->_M_left)
        __x->_M_parent->_M_left = __y;
      else
        __x->_M_parent->_M_right = __y;
      __y->_M_left = __x;
      __x->_M_parent = __y;
    }
    
    static void
    local_Rb_tree_rotate_right(_Rb_tree_node_base* const __x,
                   _Rb_tree_node_base*& __root)
    {
      _Rb_tree_node_base* const __y = __x->_M_left;

      __x->_M_left = __y->_M_right;
      if (__y->_M_right != 0)
        __y->_M_right->_M_parent = __x;
      __y->_M_parent = __x->_M_parent;

      if (__x == __root)
        __root = __y;
      else if (__x == __x->_M_parent->_M_right)
        __x->_M_parent->_M_right = __y;
      else
        __x->_M_parent->_M_left = __y;
      __y->_M_right = __x;
      __x->_M_parent = __y;
    }
    
    _Rb_tree_node_base*
    _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
                     _Rb_tree_node_base& __header) throw ()
    {
      _Rb_tree_node_base *& __root = __header._M_parent;
      _Rb_tree_node_base *& __leftmost = __header._M_left;
      _Rb_tree_node_base *& __rightmost = __header._M_right;
      _Rb_tree_node_base* __y = __z;
      _Rb_tree_node_base* __x = 0;
      _Rb_tree_node_base* __x_parent = 0;

      if (__y->_M_left == 0)     // __z has at most one non-null child. y == z.
        __x = __y->_M_right;     // __x might be null.
      else
        if (__y->_M_right == 0)  // __z has exactly one non-null child. y == z.
      __x = __y->_M_left;    // __x is not null.
        else
      {
        // __z has two non-null children.  Set __y to
        __y = __y->_M_right;   //   __z's successor.  __x might be null.
        while (__y->_M_left != 0)
          __y = __y->_M_left;
        __x = __y->_M_right;
      }
      if (__y != __z)
        {
          // relink y in place of z.  y is z's successor
          __z->_M_left->_M_parent = __y;
          __y->_M_left = __z->_M_left;
          if (__y != __z->_M_right)
            {
              __x_parent = __y->_M_parent;
              if (__x) __x->_M_parent = __y->_M_parent;
              __y->_M_parent->_M_left = __x;   // __y must be a child of _M_left
              __y->_M_right = __z->_M_right;
              __z->_M_right->_M_parent = __y;
            }
          else
            __x_parent = __y;
          if (__root == __z)
            __root = __y;
          else if (__z->_M_parent->_M_left == __z)
            __z->_M_parent->_M_left = __y;
          else
            __z->_M_parent->_M_right = __y;
          __y->_M_parent = __z->_M_parent;
          std::swap(__y->_M_color, __z->_M_color);
          __y = __z;
          // __y now points to node to be actually deleted
        }
      else
        {                        // __y == __z
          __x_parent = __y->_M_parent;
          if (__x)
            __x->_M_parent = __y->_M_parent;
          if (__root == __z)
            __root = __x;
          else
            if (__z->_M_parent->_M_left == __z)
              __z->_M_parent->_M_left = __x;
            else
              __z->_M_parent->_M_right = __x;
          if (__leftmost == __z)
            {
              if (__z->_M_right == 0)        // __z->_M_left must be null also
                __leftmost = __z->_M_parent;
              // makes __leftmost == _M_header if __z == __root
              else
                __leftmost = _Rb_tree_node_base::_S_minimum(__x);
            }
          if (__rightmost == __z)
            {
              if (__z->_M_left == 0)         // __z->_M_right must be null also
                __rightmost = __z->_M_parent;
              // makes __rightmost == _M_header if __z == __root
              else                      // __x == __z->_M_left
                __rightmost = _Rb_tree_node_base::_S_maximum(__x);
            }
        }
        
      if (__y->_M_color != _S_red)
        {
      while (__x != __root && (__x == 0 || __x->_M_color == _S_black))
        if (__x == __x_parent->_M_left)
          {
            _Rb_tree_node_base* __w = __x_parent->_M_right;
            if (__w->_M_color == _S_red)
            {
                __w->_M_color = _S_black;
                __x_parent->_M_color = _S_red;
                local_Rb_tree_rotate_left(__x_parent, __root);
                __w = __x_parent->_M_right;
            }
            if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_black) &&
                (__w->_M_right == 0 || __w->_M_right->_M_color == _S_black))
            {
                __w->_M_color = _S_red;
                __x = __x_parent;
                __x_parent = __x_parent->_M_parent;
            }
            else
            {
                if (__w->_M_right == 0 || __w->_M_right->_M_color == _S_black)
                {
                    __w->_M_left->_M_color = _S_black;
                    __w->_M_color = _S_red;
                    local_Rb_tree_rotate_right(__w, __root);
                    __w = __x_parent->_M_right;
                }
                __w->_M_color = __x_parent->_M_color;
                __x_parent->_M_color = _S_black;
                if (__w->_M_right)
                  __w->_M_right->_M_color = _S_black;
                local_Rb_tree_rotate_left(__x_parent, __root);
                break;
              }
          }
        else
          {
            // same as above, with _M_right <-> _M_left.
            _Rb_tree_node_base* __w = __x_parent->_M_left;
            if (__w->_M_color == _S_red)
          {
            __w->_M_color = _S_black;
            __x_parent->_M_color = _S_red;
            local_Rb_tree_rotate_right(__x_parent, __root);
            __w = __x_parent->_M_left;
          }
            if ((__w->_M_right == 0 ||
             __w->_M_right->_M_color == _S_black) &&
            (__w->_M_left == 0 ||
             __w->_M_left->_M_color == _S_black))
          {
            __w->_M_color = _S_red;
            __x = __x_parent;
            __x_parent = __x_parent->_M_parent;
          }
            else
          {
            if (__w->_M_left == 0 || __w->_M_left->_M_color == _S_black)
              {
                __w->_M_right->_M_color = _S_black;
                __w->_M_color = _S_red;
                local_Rb_tree_rotate_left(__w, __root);
                __w = __x_parent->_M_left;
              }
            __w->_M_color = __x_parent->_M_color;
            __x_parent->_M_color = _S_black;
            if (__w->_M_left)
              __w->_M_left->_M_color = _S_black;
            local_Rb_tree_rotate_right(__x_parent, __root);
            break;
          }
          }
      if (__x) __x->_M_color = _S_black;
        }
      return __y;
    }

    
    template<typename _Tp>
      struct _Rb_tree_iterator
      {
        typedef _Tp  value_type;
        typedef _Tp& reference;
        typedef _Tp* pointer;

          typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t             difference_type;

        typedef _Rb_tree_iterator<_Tp>        _Self;
        typedef _Rb_tree_node_base::_Base_ptr    _Base_ptr;
        typedef _Rb_tree_node<_Tp>*        _Link_type;

        _Rb_tree_iterator()
        : _M_node() { }

        explicit
        _Rb_tree_iterator(_Base_ptr __x)
        : _M_node(__x) { }

        reference
        operator*() const
        { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

        pointer
        operator->() const
        { return static_cast<_Link_type> (_M_node)->_M_valptr(); }

        _Self&
        operator++()
        {
      _M_node = _Rb_tree_increment(_M_node);
      return *this;
        }

        _Self
        operator++(int)
        {
      _Self __tmp = *this;
      _M_node = _Rb_tree_increment(_M_node);
      return __tmp;
        }

        _Self&
        operator--()
        {
      _M_node = _Rb_tree_decrement(_M_node);
      return *this;
        }

        _Self
        operator--(int)
        {
      _Self __tmp = *this;
      _M_node = _Rb_tree_decrement(_M_node);
      return __tmp;
        }

        friend bool
        operator==(const _Self& __x, const _Self& __y)
        { return __x._M_node == __y._M_node; }

  #if ! __cpp_lib_three_way_comparison
        friend bool
        operator!=(const _Self& __x, const _Self& __y)
        { return __x._M_node != __y._M_node; }
  #endif

        _Base_ptr _M_node;
    };

    template<typename _Tp>
      struct _Rb_tree_const_iterator
      {
        typedef _Tp     value_type;
        typedef const _Tp& reference;
        typedef const _Tp* pointer;

        typedef _Rb_tree_iterator<_Tp> iterator;

          typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t             difference_type;

        typedef _Rb_tree_const_iterator<_Tp>        _Self;
        typedef _Rb_tree_node_base::_Const_Base_ptr    _Base_ptr;
        typedef const _Rb_tree_node<_Tp>*            _Link_type;

        _Rb_tree_const_iterator()
        : _M_node() { }

        explicit
        _Rb_tree_const_iterator(_Base_ptr __x)
        : _M_node(__x) { }

        _Rb_tree_const_iterator(const iterator& __it)
        : _M_node(__it._M_node) { }

        iterator
        _M_const_cast() const
        { return iterator(const_cast<typename iterator::_Base_ptr>(_M_node)); }

        reference
        operator*() const
        { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

        pointer
        operator->() const
        { return static_cast<_Link_type>(_M_node)->_M_valptr(); }

        _Self&
        operator++()
        {
      _M_node = _Rb_tree_increment(_M_node);
      return *this;
        }

        _Self
        operator++(int)
        {
      _Self __tmp = *this;
      _M_node = _Rb_tree_increment(_M_node);
      return __tmp;
        }

        _Self&
        operator--()
        {
      _M_node = _Rb_tree_decrement(_M_node);
      return *this;
        }

        _Self
        operator--(int)
        {
      _Self __tmp = *this;
      _M_node = _Rb_tree_decrement(_M_node);
      return __tmp;
        }

        friend bool
        operator==(const _Self& __x, const _Self& __y)
        { return __x._M_node == __y._M_node; }

  #if ! __cpp_lib_three_way_comparison
        friend bool
        operator!=(const _Self& __x, const _Self& __y)
        { return __x._M_node != __y._M_node; }
  #endif

        _Base_ptr _M_node;
      };
    
    _Rb_tree_node_base*
    _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
                     _Rb_tree_node_base& __header) throw ();
    
    void
    _Rb_tree_insert_and_rebalance(const bool __insert_left,
                  _Rb_tree_node_base* __x,
                  _Rb_tree_node_base* __p,
                                  _Rb_tree_node_base& __header) throw () {
        _Rb_tree_node_base *& __root = __header._M_parent;

        // Initialize fields in new node to insert.
        __x->_M_parent = __p;
        __x->_M_left = 0;
        __x->_M_right = 0;
        __x->_M_color = _S_red;

        // Insert.
        // Make new node child of parent and maintain root, leftmost and
        // rightmost nodes.
        // N.B. First node is always inserted left.
        if (__insert_left)
          {
            __p->_M_left = __x; // also makes leftmost = __x when __p == &__header

            if (__p == &__header)
            {
                __header._M_parent = __x;
                __header._M_right = __x;
            }
            else if (__p == __header._M_left)
              __header._M_left = __x; // maintain leftmost pointing to min node
          }
        else
          {
            __p->_M_right = __x;

            if (__p == __header._M_right)
              __header._M_right = __x; // maintain rightmost pointing to max node
          }
        // Rebalance.
        while (__x != __root
           && __x->_M_parent->_M_color == _S_red)
          {
        _Rb_tree_node_base* const __xpp = __x->_M_parent->_M_parent;

        if (__x->_M_parent == __xpp->_M_left)
          {
            _Rb_tree_node_base* const __y = __xpp->_M_right;
            if (__y && __y->_M_color == _S_red)
              {
            __x->_M_parent->_M_color = _S_black;
            __y->_M_color = _S_black;
            __xpp->_M_color = _S_red;
            __x = __xpp;
              }
            else
              {
            if (__x == __x->_M_parent->_M_right)
              {
                __x = __x->_M_parent;
                local_Rb_tree_rotate_left(__x, __root);
              }
            __x->_M_parent->_M_color = _S_black;
            __xpp->_M_color = _S_red;
            local_Rb_tree_rotate_right(__xpp, __root);
              }
          }
        else
          {
            _Rb_tree_node_base* const __y = __xpp->_M_left;
            if (__y && __y->_M_color == _S_red)
              {
            __x->_M_parent->_M_color = _S_black;
            __y->_M_color = _S_black;
            __xpp->_M_color = _S_red;
            __x = __xpp;
              }
            else
              {
            if (__x == __x->_M_parent->_M_left)
              {
                __x = __x->_M_parent;
                local_Rb_tree_rotate_right(__x, __root);
              }
            __x->_M_parent->_M_color = _S_black;
            __xpp->_M_color = _S_red;
            local_Rb_tree_rotate_left(__xpp, __root);
              }
          }
          }
        __root->_M_color = _S_black;
    }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc = std::allocator<_Val> >
      class _Rb_tree
      {
          typedef typename _Alloc::template
        rebind<_Rb_tree_node<_Val> >::other _Node_allocator;
          typedef std::allocator_traits<_Node_allocator> _Alloc_traits;
          
      protected:
          typedef _Rb_tree_node_base*         _Base_ptr;
          typedef const _Rb_tree_node_base*     _Const_Base_ptr;
          typedef _Rb_tree_node<_Val>*         _Link_type;
          typedef const _Rb_tree_node<_Val>*    _Const_Link_type;
          
          
          struct _Alloc_node
          {
        _Alloc_node(_Rb_tree& __t)
        : _M_t(__t) { }

        template<typename _Arg>
          _Link_type
          operator()(_Arg&& __arg) const
          { return _M_t._M_create_node(__arg); }

          private:
        _Rb_tree& _M_t;
          };
          
      public:
          typedef _Key                 key_type;
          typedef _Val                 value_type;
          typedef value_type*             pointer;
          typedef const value_type*         const_pointer;
          typedef value_type&             reference;
          typedef const value_type&         const_reference;
          typedef size_t                 size_type;
          typedef ptrdiff_t             difference_type;
          typedef _Alloc                 allocator_type;
          
          _Node_allocator&
          _M_get_Node_allocator()
          { return this->_M_impl; }
          
          const _Node_allocator&
          _M_get_Node_allocator() const
          { return this->_M_impl; }
          
      public:
          size_t nodeCount() {
              return _M_impl._M_node_count;
          }
          
      protected:
          
          _Link_type
          _M_get_node()
          { return _Alloc_traits::allocate(_M_get_Node_allocator(), 1); }
          
          
          void
          _M_put_node(_Link_type __p)
          { _Alloc_traits::deallocate(_M_get_Node_allocator(), __p, 1); }
          
          template<typename... _Args>
        void
        _M_construct_node(_Link_type __node, _Args&&... __args)
        {
          try
            {
              ::new(__node) _Rb_tree_node<_Val>;
              _Alloc_traits::construct(_M_get_Node_allocator(),
                           __node->_M_valptr(),
                           std::forward<_Args>(__args)...);
            }
          catch(...)
            {
              __node->~_Rb_tree_node<_Val>();
              _M_put_node(__node);
              throw;
            }
        }
          
          
          template<typename... _Args>
        _Link_type
        _M_create_node(_Args&&... __args)
        {
          _Link_type __tmp = _M_get_node();
          _M_construct_node(__tmp, std::forward<_Args>(__args)...);
          return __tmp;
        }
          
          void
          _M_destroy_node(_Link_type __p)
          {
    #if __cplusplus < 201103L
              get_allocator().destroy(__p->_M_valptr());
    #else
              _Alloc_traits::destroy(_M_get_Node_allocator(), __p->_M_valptr());
              __p->~_Rb_tree_node<_Val>();
    #endif
          }
          
          void
          _M_drop_node(_Link_type __p)
          {
              _M_destroy_node(__p);
              _M_put_node(__p);
          }
          
          template<typename _Key_compare>
          struct _Rb_tree_impl
          : public _Node_allocator
          , public _Rb_tree_key_compare<_Key_compare>
          , public _Rb_tree_header
          {
            typedef _Rb_tree_key_compare<_Key_compare> _Base_key_compare;

            _Rb_tree_impl()
            : _Node_allocator()
            { }

            _Rb_tree_impl(const _Rb_tree_impl& __x)
            : _Node_allocator(_Alloc_traits::_S_select_on_copy(__x))
            , _Base_key_compare(__x._M_key_compare)
            , _Rb_tree_header()
            { }

    
            _Rb_tree_impl(_Rb_tree_impl&&)
            = default;

            explicit
            _Rb_tree_impl(_Node_allocator&& __a)
            : _Node_allocator(std::move(__a))
            { }

            _Rb_tree_impl(_Rb_tree_impl&& __x, _Node_allocator&& __a)
            : _Node_allocator(std::move(__a)),
              _Base_key_compare(std::move(__x)),
              _Rb_tree_header(std::move(__x))
            { }

            _Rb_tree_impl(const _Key_compare& __comp, _Node_allocator&& __a)
            : _Node_allocator(std::move(__a)), _Base_key_compare(__comp)
            { }
      
          };
          
          _Rb_tree_impl<_Compare> _M_impl;
          
      protected:
          
          static const _Key&
          _S_key(_Const_Link_type __x)
          {
              return _KeyOfValue()(*__x->_M_valptr());
          }
          static _Link_type
          _S_left(_Base_ptr __x)
          { return static_cast<_Link_type>(__x->_M_left); }

          static _Const_Link_type
          _S_left(_Const_Base_ptr __x)
          { return static_cast<_Const_Link_type>(__x->_M_left); }
          
          static _Link_type
          _S_right(_Base_ptr __x)
          { return static_cast<_Link_type>(__x->_M_right); }

          static _Const_Link_type
          _S_right(_Const_Base_ptr __x)
          { return static_cast<_Const_Link_type>(__x->_M_right); }
          
          static const _Key&
          _S_key(_Const_Base_ptr __x)
          { return _S_key(static_cast<_Const_Link_type>(__x)); }
          
      protected:
        _Base_ptr&
        _M_root()
        { return this->_M_impl._M_header._M_parent; }

        _Const_Base_ptr
        _M_root() const
        { return this->_M_impl._M_header._M_parent; }
          
          _Base_ptr&
          _M_leftmost()
          { return this->_M_impl._M_header._M_left; }

          _Const_Base_ptr
          _M_leftmost() const
          { return this->_M_impl._M_header._M_left; }
          
          _Base_ptr&
          _M_rightmost()
          { return this->_M_impl._M_header._M_right; }
          
          _Const_Base_ptr
          _M_rightmost() const
          { return this->_M_impl._M_header._M_right; }
          
          _Link_type
          _M_mbegin() const
          { return static_cast<_Link_type>(this->_M_impl._M_header._M_parent); }
          
          _Link_type
          _M_begin()
          { return _M_mbegin(); }

          _Const_Link_type
          _M_begin() const
          {
        return static_cast<_Const_Link_type>
          (this->_M_impl._M_header._M_parent);
          }

          _Base_ptr
          _M_end()
          { return &this->_M_impl._M_header; }

          _Const_Base_ptr
          _M_end() const
          { return &this->_M_impl._M_header; }
          
      public:
        typedef _Rb_tree_iterator<value_type>       iterator;
        typedef _Rb_tree_const_iterator<value_type> const_iterator;
          
          typedef std::reverse_iterator<iterator>       reverse_iterator;
          typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
          
          /*using node_type = _Node_handle<_Key, _Val, _Node_allocator>;
          using insert_return_type = _Node_insert_return<
        conditional_t<is_same_v<_Key, _Val>, const_iterator, iterator>,
        node_type>;*/
          
          std::pair<_Base_ptr, _Base_ptr>
          _M_get_insert_unique_pos(const key_type& __k);
          
          std::pair<_Base_ptr, _Base_ptr>
          _M_get_insert_hint_unique_pos(const_iterator __pos,
                        const key_type& __k);
          
      private:
          
          template<typename _Arg, typename _NodeGen>
        iterator
        _M_insert_(_Base_ptr __x, _Base_ptr __y, _Arg&& __v, _NodeGen&);
          
          iterator
          _M_insert_node(_Base_ptr __x, _Base_ptr __y, _Link_type __z);
          
          
          
          
          
          void
          _M_erase(_Link_type __x);

          iterator
          _M_lower_bound(_Link_type __x, _Base_ptr __y,
                 const _Key& __k);

          const_iterator
          _M_lower_bound(_Const_Link_type __x, _Const_Base_ptr __y,
                 const _Key& __k) const;

          iterator
          _M_upper_bound(_Link_type __x, _Base_ptr __y,
                 const _Key& __k);

          const_iterator
          _M_upper_bound(_Const_Link_type __x, _Const_Base_ptr __y,
                 const _Key& __k) const;
          

      public:
          
          _Rb_tree() = default;


          _Rb_tree(const _Compare& __comp,
               const allocator_type& __a = allocator_type())
          : _M_impl(__comp, _Node_allocator(__a)) { }

          _Rb_tree(const _Rb_tree& __x)
          : _M_impl(__x._M_impl)
          {
        if (__x._M_root() != 0)
          _M_root() = _M_copy(__x);
          }
          
          ~_Rb_tree()
          {
              _M_erase(_M_begin()); }
          
          _Compare
          key_comp() const
          { return _M_impl._M_key_compare; }
          
          iterator
          begin()
          { return iterator(this->_M_impl._M_header._M_left); }

          const_iterator
          begin() const
          { return const_iterator(this->_M_impl._M_header._M_left); }
          
          iterator
          end()
          { return iterator(&this->_M_impl._M_header); }

          const_iterator
          end() const
          { return const_iterator(&this->_M_impl._M_header); }
          
          bool
          empty() const
          { return _M_impl._M_node_count == 0; }
          
          size_type
          size() const
          { return _M_impl._M_node_count; }
          
          
          template<typename _Arg>
          std::pair<iterator, bool>
        _M_insert_unique(_Arg&& __x);
          
          template<typename... _Args>
          iterator
          _M_emplace_hint_unique(const_iterator __pos, _Args&&... __args);
          
          
      private:
        void
        _M_erase_aux(const_iterator __position);

        void
        _M_erase_aux(const_iterator __first, const_iterator __last);
          
      public:
          
          iterator
          erase(const_iterator __position)
          {
              assert(__position != end());
              const_iterator __result = __position;
              ++__result;
              _M_erase_aux(__position);
              return __result._M_const_cast();
          }

          
          iterator
          erase(iterator __position)
          {
              assert(__position != end());
              iterator __result = __position;
              ++__result;
              _M_erase_aux(__position);
              return __result;
          }
          
          
          iterator
          erase(const_iterator __first, const_iterator __last)
          {
              _M_erase_aux(__first, __last);
              return __last._M_const_cast();
          }
          
          void
          clear()
          {
              _M_erase(_M_begin());
              _M_impl._M_reset();
          }
          
          iterator
          find(const key_type& __k);

          const_iterator
          find(const key_type& __k) const;

          iterator
          lower_bound(const key_type& __k)
          { return _M_lower_bound(_M_begin(), _M_end(), __k); }

          const_iterator
          lower_bound(const key_type& __k) const
          { return _M_lower_bound(_M_begin(), _M_end(), __k); }
          
          template<typename _Kt>
          iterator
          _M_find_tr(const _Kt& __k)
          {
            const _Rb_tree* __const_this = this;
            return __const_this->_M_find_tr(__k)._M_const_cast();
          }

          template<typename _Kt>
          const_iterator
          _M_find_tr(const _Kt& __k) const
          {
            auto __j = _M_lower_bound_tr(__k);
            if (__j != end() && _M_impl._M_key_compare(__k, _S_key(__j._M_node)))
              __j = end();
            return __j;
          }
          
          template<typename _Kt>
          iterator
          _M_lower_bound_tr(const _Kt& __k)
          {
            const _Rb_tree* __const_this = this;
            return __const_this->_M_lower_bound_tr(__k)._M_const_cast();
          }

            template<typename _Kt>
          const_iterator
          _M_lower_bound_tr(const _Kt& __k) const
          {
            auto __x = _M_begin();
            auto __y = _M_end();
            while (__x != 0)
              if (!_M_impl._M_key_compare(_S_key(__x), __k))
                {
              __y = __x;
              __x = _S_left(__x);
                }
              else
                __x = _S_right(__x);
            return const_iterator(__y);
          }
          
          

      };
    
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      void
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_erase_aux(const_iterator __position)
      {
        _Link_type __y =
      static_cast<_Link_type>(_Rb_tree_rebalance_for_erase
                  (const_cast<_Base_ptr>(__position._M_node),
                   this->_M_impl._M_header));
        _M_drop_node(__y);
        --_M_impl._M_node_count;
      }

    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      void
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_erase_aux(const_iterator __first, const_iterator __last)
      {
        if (__first == begin() && __last == end())
      clear();
        else
      while (__first != __last)
        _M_erase_aux(__first++);
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      typename _Rb_tree<_Key, _Val, _KeyOfValue,
                _Compare, _Alloc>::iterator
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      find(const _Key& __k)
      {
        iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
        return (__j == end()
            || _M_impl._M_key_compare(__k,
                      _S_key(__j._M_node))) ? end() : __j;
      }

    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      typename _Rb_tree<_Key, _Val, _KeyOfValue,
                _Compare, _Alloc>::const_iterator
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      find(const _Key& __k) const
      {
        const_iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
        return (__j == end()
            || _M_impl._M_key_compare(__k,
                      _S_key(__j._M_node))) ? end() : __j;
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      void
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_erase(_Link_type __x)
      {
        // Erase without rebalancing.
        while (__x != 0)
      {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _M_drop_node(__x);
        __x = __y;
      }
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      typename _Rb_tree<_Key, _Val, _KeyOfValue,
                _Compare, _Alloc>::iterator
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_lower_bound(_Link_type __x, _Base_ptr __y,
             const _Key& __k)
      {
        while (__x != 0)
      if (!_M_impl._M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);
        return iterator(__y);
      }

    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      typename _Rb_tree<_Key, _Val, _KeyOfValue,
                _Compare, _Alloc>::const_iterator
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_lower_bound(_Const_Link_type __x, _Const_Base_ptr __y,
             const _Key& __k) const
      {
        while (__x != 0)
      if (!_M_impl._M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);
        return const_iterator(__y);
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
  #if __cplusplus >= 201103L
      template<typename _Arg, typename _NodeGen>
  #else
      template<typename _NodeGen>
  #endif
        typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
        _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
        _M_insert_(_Base_ptr __x, _Base_ptr __p,
  #if __cplusplus >= 201103L
           _Arg&& __v,
  #else
           const _Val& __v,
  #endif
           _NodeGen& __node_gen)
        {
      bool __insert_left = (__x != 0 || __p == _M_end()
                    || _M_impl._M_key_compare(_KeyOfValue()(__v),
                              _S_key(__p)));

      _Link_type __z = __node_gen(std::forward<_Arg>(__v));

      _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                        this->_M_impl._M_header);
      ++_M_impl._M_node_count;
      return iterator(__z);
        }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_insert_node(_Base_ptr __x, _Base_ptr __p, _Link_type __z)
      {
        bool __insert_left = (__x != 0 || __p == _M_end()
                  || _M_impl._M_key_compare(_S_key(__z),
                                _S_key(__p)));

        _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                      this->_M_impl._M_header);
        ++_M_impl._M_node_count;
        return iterator(__z);
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      template<typename... _Args>
        typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
        _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
        _M_emplace_hint_unique(const_iterator __pos, _Args&&... __args)
        {
      _Link_type __z = _M_create_node(std::forward<_Args>(__args)...);

      try
        {
          auto __res = _M_get_insert_hint_unique_pos(__pos, _S_key(__z));

          if (__res.second)
            return _M_insert_node(__res.first, __res.second, __z);

          _M_drop_node(__z);
          return iterator(__res.first);
        }
      catch(...)
        {
          _M_drop_node(__z);
          throw;
        }
        }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
    std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
                 _Compare, _Alloc>::_Base_ptr,
       typename _Rb_tree<_Key, _Val, _KeyOfValue,
                 _Compare, _Alloc>::_Base_ptr>
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_get_insert_unique_pos(const key_type& __k)
      {
          typedef std::pair<_Base_ptr, _Base_ptr> _Res;
        _Link_type __x = _M_begin();
        _Base_ptr __y = _M_end();
        bool __comp = true;
        while (__x != 0)
      {
        __y = __x;
        __comp = _M_impl._M_key_compare(__k, _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
      }
        iterator __j = iterator(__y);
        if (__comp)
      {
        if (__j == begin())
          return _Res(__x, __y);
        else
          --__j;
      }
        if (_M_impl._M_key_compare(_S_key(__j._M_node), __k))
      return _Res(__x, __y);
        return _Res(__j._M_node, 0);
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
    std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
                 _Compare, _Alloc>::_Base_ptr,
       typename _Rb_tree<_Key, _Val, _KeyOfValue,
                 _Compare, _Alloc>::_Base_ptr>
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_get_insert_hint_unique_pos(const_iterator __position,
                    const key_type& __k)
      {
        iterator __pos = __position._M_const_cast();
          typedef std::pair<_Base_ptr, _Base_ptr> _Res;

        // end()
        if (__pos._M_node == _M_end())
      {
        if (size() > 0
            && _M_impl._M_key_compare(_S_key(_M_rightmost()), __k))
          return _Res(0, _M_rightmost());
        else
          return _M_get_insert_unique_pos(__k);
      }
        else if (_M_impl._M_key_compare(__k, _S_key(__pos._M_node)))
      {
        // First, try before...
        iterator __before = __pos;
        if (__pos._M_node == _M_leftmost()) // begin()
          return _Res(_M_leftmost(), _M_leftmost());
        else if (_M_impl._M_key_compare(_S_key((--__before)._M_node), __k))
          {
            if (_S_right(__before._M_node) == 0)
          return _Res(0, __before._M_node);
            else
          return _Res(__pos._M_node, __pos._M_node);
          }
        else
          return _M_get_insert_unique_pos(__k);
      }
        else if (_M_impl._M_key_compare(_S_key(__pos._M_node), __k))
      {
        // ... then try after.
        iterator __after = __pos;
        if (__pos._M_node == _M_rightmost())
          return _Res(0, _M_rightmost());
        else if (_M_impl._M_key_compare(__k, _S_key((++__after)._M_node)))
          {
            if (_S_right(__pos._M_node) == 0)
          return _Res(0, __pos._M_node);
            else
          return _Res(__after._M_node, __after._M_node);
          }
        else
          return _M_get_insert_unique_pos(__k);
      }
        else
      // Equivalent keys.
      return _Res(__pos._M_node, 0);
      }
    
    template<typename _Key, typename _Val, typename _KeyOfValue,
         typename _Compare, typename _Alloc>
      template<typename _Arg>
    std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
                 _Compare, _Alloc>::iterator, bool>
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
      _M_insert_unique(_Arg&& __v)
      {
          typedef std::pair<iterator, bool> _Res;
          std::pair<_Base_ptr, _Base_ptr> __res
      = _M_get_insert_unique_pos(_KeyOfValue()(__v));

        if (__res.second)
      {
        _Alloc_node __an(*this);
        return _Res(_M_insert_(__res.first, __res.second,
                               std::forward<_Arg>(__v), __an),
                true);
      }

        return _Res(iterator(__res.first), false);
      }

}

#endif


