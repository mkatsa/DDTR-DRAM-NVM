#ifndef TRACING_MAP_HPP
#define TRACING_MAP_HPP
#include "construct.h"
#include "allocated.h"
#include <utility>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  template <typename T>
  void swap(T& x, T& y) {
    T tmp = x;
    x = y;
    y = tmp;
  }
  //////////////////////////////////////////////////////////////////////////////
  // Default DDT logger does nothing
  //////////////////////////////////////////////////////////////////////////////
  struct std_map_logger {
    struct map_get {
      map_get(const unsigned int, const size_t, const size_t)    {}
      void present()                                             {}
      ~map_get()                                                 {}
    };
    struct map_add {
      map_add(const unsigned int, const size_t, const size_t)    {}
      void present()                                             {}
      ~map_add()                                                 {}
    };
    struct map_remove {
      map_remove(const unsigned int, const size_t, const size_t) {}
      void present()                                             {}
      ~map_remove()                                              {}
    };
    struct map_clear {
      map_clear(const unsigned int, const size_t)                {}
      ~map_clear()                                               {}
    };
  };

  //////////////////////////////////////////////////////////////////////////////
  // Basic value of the tree an index/value pair
  //////////////////////////////////////////////////////////////////////////////
  template<typename T, typename IT>
    struct Rb_value
    {
      typedef size_t                                      key_type;
      typedef T                                           mapped_type;
      typedef mapped_type*                                pointer;
      typedef const mapped_type *                         const_pointer;
      typedef mapped_type&                                reference;
      typedef const mapped_type&                          const_reference;
      typedef IT                                          input_type;
      typedef const input_type&                           input_reference;

      Rb_value(key_type k, input_reference v)
        : key(k), mapped(v)
        {}

      Rb_value(const Rb_value & other)
        : key(other.key), mapped(other.mapped)
        {}

      bool operator<(const Rb_value& other) const
      { return key < other.key; }
      bool operator<=(const Rb_value& other) const
      { return key <= other.key; }
      bool operator>(const Rb_value& other) const
      { return key > other.key; }
      bool operator>=(const Rb_value& other) const
      { return key >= other.key; }
      bool operator==(const Rb_value& other) const
      { return key == other.key; }
      bool operator!=(const Rb_value& other) const
      { return key != other.key; }
  
      const key_type key;
      mapped_type mapped;
    };

  //////////////////////////////////////////////////////////////////////////////
  // Basic node of the red-black tree
  //////////////////////////////////////////////////////////////////////////////
  enum Rb_tree_color { RBRed = false, RBBlack = true};

  template <typename Allocator = std_allocator>
    struct Rb_tree_node_base : public allocated<Allocator> 
    {
      typedef Rb_tree_node_base *                         base_pointer;
      typedef const Rb_tree_node_base *                   const_base_pointer;

      Rb_tree_node_base() {
        color = RBRed;
        parent = 0;
        left = 0;
        right = 0;
      }

      static inline base_pointer minimum(base_pointer x) {
        while (x->left != 0) x = x->left;
        return x;
      } 
      static inline const_base_pointer minimum(const_base_pointer x) {
        while (x->left != 0) x = x->left;
        return x;
      } 
      static inline base_pointer maximum(base_pointer x) {
        while (x->right != 0) x = x->right;
        return x;
      } 
      static inline const_base_pointer maximum(const_base_pointer x) {
        while (x->right != 0) x = x->right;
        return x;
      }

      static inline base_pointer sibling(base_pointer x) {
        if (x->parent->right == x) {
          return x->parent->left;
        } else {
          return x->parent->right;
        }
      }
      
      static inline const_base_pointer sibling(const_base_pointer x) {
        if (x->parent->right == x) {
          return x->parent->left;
        } else {
          return x->parent->right;
        }
      }

      //////////////////////////////////////////////////////////////////////////
      // is_black
      //////////////////////////////////////////////////////////////////////////
      static inline bool is_black(const_base_pointer n) {
        if (n == 0)
          return true;
        else if (n->parent == 0) // header of empty
          return true;
        else if (n->parent->parent == n && n->color == RBRed) // header
          return true;
        else if (n->color == RBBlack)
          return true;
        else
          return false;
      }

      Rb_tree_color color;
      base_pointer parent;
      base_pointer left;
      base_pointer right;
    };
    
  //////////////////////////////////////////////////////////////////////////////
  // Node of the red-black tree with a value
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename Allocator = std_allocator>
    struct Rb_tree_node : public Rb_tree_node_base<Allocator>
    {
      typedef T                                           value_type;
      typedef value_type*                                 pointer;
      typedef const value_type *                          const_pointer;
      typedef value_type&                                 reference;
      typedef const value_type&                           const_reference;
      typedef Rb_tree_node *                              node_pointer;
      typedef const Rb_tree_node *                        const_node_pointer;


      Rb_tree_node(const_reference value)
        : data(value)
        {}

      value_type data;
    };
    
  //////////////////////////////////////////////////////////////////////////////
  // Traversal strategies for nodes
  //////////////////////////////////////////////////////////////////////////////
  template<typename A>
    const Rb_tree_node_base<A> *
    Rb_tree_increment(const Rb_tree_node_base<A> * x) {
      if (x->color == RBRed && x->parent->parent == x) // Header
        return x->left;

      const Rb_tree_node_base<A> * right = x->right;
      if (right != 0) {
        if (right->parent->parent == right) {
          return right;
        } else {
          return Rb_tree_node_base<A>::minimum(right);
        }
      } else {
        while (x->parent && x == x->parent->right) {
          x = x->parent;
        }
        return x->parent;
      }
    }
  
  template<typename A>
    Rb_tree_node_base<A> *
    Rb_tree_increment(Rb_tree_node_base<A> * x)
    {
      if (x->color == RBRed && x->parent->parent == x) // Header
        return x->left;

      Rb_tree_node_base<A> * right = x->right;
      if (right != 0) {
        if (right->parent->parent == right) { // Header
          return right;
        } else {
          return Rb_tree_node_base<A>::minimum(right);
        }
      } else {
        while (x->parent && x == x->parent->right) {
          x = x->parent;
        }
        return x->parent;
      }
    }

  template<typename A>
    const Rb_tree_node_base<A> *
    Rb_tree_decrement(const Rb_tree_node_base<A> * x)
    {
      if (x->color == RBRed && x->parent->parent == x) // Header
        return x->right;

      const Rb_tree_node_base<A> * left = x->left;
      if (left != 0) {
        if (left->parent->parent == left) { // Header
          return left;
        } else {
          return Rb_tree_node_base<A>::maximum(left);
        }
      } else {
        while (x->parent && x == x->parent->left) {
          x = x->parent;
        }
        return x->parent;
      }
    }

  template<typename A>
    Rb_tree_node_base<A> *
    Rb_tree_decrement(Rb_tree_node_base<A> * x)
    {
      if (x->color == RBRed && x->parent->parent == x) // Header
        return x->right;

      Rb_tree_node_base<A> * left = x->left;
      if (left != 0) {
        if (left->parent->parent == left) { // Header
          return left;
        } else {
          return Rb_tree_node_base<A>::maximum(left);
        }
      } else {
        while (x->parent && x == x->parent->left) {
          x = x->parent;
        }
        return x->parent;
      }
    }

  //////////////////////////////////////////////////////////////////////////////
  // Pre-definitions
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename IT, int I, typename L = std_map_logger, typename A=std_allocator>
    struct Rb_tree_iterator;

  template <typename T, typename IT, int I, typename L = std_map_logger, typename A=std_allocator>
    struct Rb_tree_const_iterator;

  template <typename T, typename IT, int I, typename L = std_map_logger, typename Allocator = std_allocator>
    struct Rb_tree;

  template <typename T, typename IT, int I, typename L = std_map_logger, typename Allocator = std_allocator>
    struct map;

  //////////////////////////////////////////////////////////////////////////////
  // Red Black Tree Iterator
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename IT, int I, typename L, typename A>
    struct Rb_tree_iterator
    {
      typedef Rb_value<T, IT>                             value_type;
      typedef typename value_type::key_type               key_type;
      typedef typename value_type::mapped_type            mapped_type;
      typedef value_type*                                 pointer;
      typedef const value_type *                          const_pointer;
      typedef value_type&                                 reference;
      typedef const value_type&                           const_reference;

      typedef Rb_tree_iterator<T,IT,I,L,A>                self_type;
      typedef Rb_tree_node_base<A>                        node_base;
      typedef Rb_tree_node<value_type, A>                 node_type;
      typedef typename node_base::base_pointer            base_pointer;
      typedef typename node_type::node_pointer            link_type;

      Rb_tree_iterator() 
        : node_() {}
      Rb_tree_iterator(link_type node)
        : node_(node) {}

      reference operator*()
      {
        reference x = static_cast<link_type>(node_)->data;
        typename L::map_get S(I, x.key, sizeof(mapped_type));
        S.present(); 
        return x;
      }
      
      pointer operator->()
      {
        pointer x = &static_cast<link_type>(node_)->data;
        typename L::map_get S(I, x->key, sizeof(mapped_type));
        S.present(); 
        return x;
      }

      self_type& operator++()
      {
        node_ = Rb_tree_increment(node_);
        return *this;
      }
      self_type operator++(int)
      {
        self_type tmp = *this;
        node_ = Rb_tree_increment(node_);
        return tmp;
      }
      self_type& operator--()
      {
        node_ = Rb_tree_decrement(node_);
        return *this;
      }
      self_type operator--(int)
      {
        self_type tmp = *this;
        node_ = Rb_tree_decrement(node_);
        return tmp;
      }
      bool operator==(const self_type& x) const
      { return node_ == x.node_; }
      bool operator!=(const self_type& x) const
      { return node_ != x.node_; }
      bool operator==(const Rb_tree_const_iterator<T, IT, I, L, A>& x) const
      { return node_ == x.node_; }
      bool operator!=(const Rb_tree_const_iterator<T, IT, I, L, A>& x) const
      { return node_ == x.node_; }

      friend class Rb_tree_const_iterator<T, IT, I, L, A>;
      friend class Rb_tree<T, IT, I, L, A>;
      friend class map<T, IT, I, L, A>;
      protected:
        base_pointer node_;
    };

  //////////////////////////////////////////////////////////////////////////////
  // Red Black Tree Constant Iterator
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename IT, int I, typename L, typename A>
    struct Rb_tree_const_iterator
    {
      typedef Rb_value<T, IT>                             value_type;
      typedef typename value_type::key_type               key_type;
      typedef typename value_type::mapped_type            mapped_type;
      typedef value_type*                                 pointer;
      typedef const value_type *                          const_pointer;
      typedef value_type&                                 reference;
      typedef const value_type&                           const_reference;

      typedef Rb_tree_iterator<T,IT,I,L,A>                iterator_type;
      typedef Rb_tree_const_iterator<T,IT,I,L,A>          self_type;
      typedef Rb_tree_node_base<A>                        node_base;
      typedef Rb_tree_node<value_type, A>                 node_type;
      typedef typename node_base::const_base_pointer      base_pointer;
      typedef typename node_type::const_node_pointer      link_type;


      Rb_tree_const_iterator() 
        : node_() {}
      Rb_tree_const_iterator(link_type node)
        : node_(node) {}
      Rb_tree_const_iterator(const iterator_type& it)
        : node_(it.node_) {}

      const_reference operator*() const
      {
        const_reference x = static_cast<link_type>(node_)->data;
        typename L::map_get S(I, x.key, sizeof(mapped_type));
        S.present(); 
        return x;
      }
      
      const_pointer operator->() const
      {
        const_pointer x = &static_cast<link_type>(node_)->data;
        typename L::map_get S(I, x->key, sizeof(mapped_type));
        S.present(); 
        return x;
      }

      self_type& operator++()
      {
        node_ = Rb_tree_increment(node_);
        return *this;
      }
      self_type operator++(int)
      {
        self_type tmp = *this;
        node_ = Rb_tree_increment(node_);
        return tmp;
      }
      self_type& operator--()
      {
        node_ = Rb_tree_decrement(node_);
        return *this;
      }
      self_type operator--(int)
      {
        self_type tmp = *this;
        node_ = Rb_tree_decrement(node_);
        return tmp;
      }
      bool operator==(const self_type& x) const
      { return node_ == x.node_; }
      bool operator!=(const self_type& x) const
      { return node_ != x.node_; }

      friend class Rb_tree_iterator<T, IT, I, L, A>;
      friend class Rb_tree<T, IT, I, L, A>;
      friend class map<T, IT, I, L, A>;
      protected:
        base_pointer node_;
    };

  //////////////////////////////////////////////////////////////////////////////
  // Red Black Tree
  //////////////////////////////////////////////////////////////////////////////
  template <typename T, typename IT, int I, typename L, typename A>
    class Rb_tree
    {
      public:
        typedef size_t                                      key_type;
        typedef T                                           mapped_type;
        typedef mapped_type*                                pointer;
        typedef const mapped_type *                         const_pointer;
        typedef mapped_type&                                reference;
        typedef const mapped_type&                          const_reference;
        typedef IT                                          input_type;
        typedef const input_type&                           input_reference;

        typedef Rb_value<mapped_type, input_type>           value_type;
        typedef Rb_tree_node_base<A>                        node_base;
        typedef Rb_tree_node<value_type, A>                 node_type;

        typedef typename node_base::base_pointer            base_pointer;
        typedef typename node_base::const_base_pointer      const_base_pointer;
        typedef typename node_type::node_pointer            node_pointer;
        typedef typename node_type::const_node_pointer      const_node_pointer;

        typedef Rb_tree_iterator<T, IT, I, L, A>            iterator;
        typedef Rb_tree_const_iterator<T, IT, I, L, A>      const_iterator;

        ////////////////////////////////////////////////////////////////////////
        // Constructor, Destructor, ...
        ////////////////////////////////////////////////////////////////////////
        Rb_tree() {
          header_.color = RBRed;
          header_.parent = 0;
          header_.left = &header_;
          header_.right = &header_;
          node_count_ = 0;
        }

        Rb_tree(const Rb_tree& other) {
          header_.color = RBRed;
          if (other.root() == 0) {
            root() = 0;
            leftmost() = &header_;
            rightmost() = &header_;
            node_count_ = 0;
          } else {
            root() = other.root();
            leftmost() = other.leftmost();
            rightmost() = other.rightmost();
            node_count_ = other.node_count_;
          }
        }

        ~Rb_tree() {
          clear();
        }

        Rb_tree& operator=(const Rb_tree& other) {
          if (this != &other) {
            clear();
            if (other.root() == 0) {
              root() = 0;
              leftmost() = &header_;
              rightmost() = &header_;
              node_count_ = 0;
            } else {
              root() = other.root();
              leftmost() = other.leftmost();
              rightmost() = other.rightmost();
              node_count_ = other.node_count_;
            }
          }
          return *this;
        }

        ////////////////////////////////////////////////////////////////////////
        // Basic operations
        ////////////////////////////////////////////////////////////////////////
        bool empty() const
        { return (header_->parent == 0); }

        size_t size() const
        { return node_count_; }

        const_iterator begin() const
        { return static_cast<const_node_pointer>(header_.left); }
        
        iterator begin()
        { return static_cast<node_pointer>(header_.left); }

        const_iterator end() const
        { return static_cast<const_node_pointer>(&header_); }
        
        iterator end()
        { return static_cast<node_pointer>(&header_); }

        const_iterator find(key_type k) const
        {
          const_node_pointer y = static_cast<const_node_pointer>(&header_);
          const_node_pointer x = static_cast<const_node_pointer>(header_.parent);
          while (x != 0) {
            if (x == y) {
              break;
            }
            if (x->data.key > k) {
              x = static_cast<const_node_pointer>(x->left);
            }
            else if (x->data.key < k) {
              x = static_cast<const_node_pointer>(x->right);
            }
            else {
              return const_iterator(x);
            }
          }
          return const_iterator(static_cast<const_node_pointer>(&header_));
        }

        iterator find(key_type k)
        {
          node_pointer y = static_cast<node_pointer>(&header_);
          node_pointer x = static_cast<node_pointer>(header_.parent);
          while (x != 0) {
            if (x == y) {
              break;
            }
            if (x->data.key > k) {
              x = static_cast<node_pointer>(x->left);
            }
            else if (x->data.key < k) {
              x = static_cast<node_pointer>(x->right);
            }
            else {
              return iterator(x);
            }
          }
          return iterator((static_cast<node_pointer>(&header_)));
        }

        std::pair<iterator, bool> add(key_type k, input_reference v)
        {
          if (header_.parent == 0) {
            node_pointer y = new node_type(value_type(k,v));
            header_.parent = y;
            header_.left = y;
            header_.right = y;
            y->parent = &header_;
            y->color = RBBlack;
            y->left = &header_;
            y->right = &header_;
            node_count_++;
            return std::pair<iterator, bool>(iterator(y), true);
          } else {
            node_pointer x = static_cast<node_pointer>(header_.parent);
            while (true) {
              if (k < x->data.key) {
                if (x->left == 0) {
                  node_pointer y = new node_type(value_type(k,v));
                  y->parent = x;
                  y->color = RBRed;
                  x->left = y;
                  rebalance_after_insert(y, root());
                  node_count_++;
                  return std::pair<iterator, bool>(iterator(y), true);
                } else if (x->left == &header_) {
                  node_pointer y = new node_type(value_type(k,v));
                  y->parent = x;
                  y->color = RBRed;
                  y->left = &header_;
                  header_.left = y;
                  x->left = y;
                  rebalance_after_insert(y, root());
                  node_count_++;
                  return std::pair<iterator, bool>(iterator(y), true);
                } else {
                  x = static_cast<node_pointer>(x->left);
                }
              } else if (k > x->data.key) {
                if (x->right == 0) {
                  node_pointer y = new node_type(value_type(k,v));
                  y->parent = x;
                  y->color = RBRed;
                  x->right = y;
                  rebalance_after_insert(y, root());
                  node_count_++;
                  return std::pair<iterator, bool>(iterator(y), true);
                } else if (x->right == &header_) {
                  node_pointer y = new node_type(value_type(k,v));
                  y->parent = x;
                  y->color = RBRed;
                  y->right = &header_;
                  header_.right = y;
                  x->right = y;
                  rebalance_after_insert(y, root());
                  node_count_++;
                  return std::pair<iterator, bool>(iterator(y), true);
                } else {
                  x = static_cast<node_pointer>(x->right);
                }
              } else {
                x->data.mapped = v;
                return std::pair<iterator, bool>(iterator(x), false);
              }
            }
          }
        }

        bool remove(key_type k) {
          return remove(find(k));
        }

        bool remove(iterator i) {
          if (i == end()) {
            return false;
          }
          node_pointer x = rebalance_for_erase(i.node_);
          delete x;
          node_count_--;
          return true;
        }

        void clear() {
          typename L::map_clear S(I, sizeof(mapped_type));
          erase(header_.parent);
          header_.left = &header_;
          header_.right = &header_;
          node_count_ = 0;
        }

        void print_tree() {
          print_consistency(header_.parent);
          std::cout << std::endl;
        }

      protected:

        base_pointer& root()
        { return header_.parent; }
        base_pointer& leftmost()
        { return header_.left; }
        base_pointer& rightmost()
        { return header_.right; }

        const base_pointer& root() const
        { return header_.parent; }
        const base_pointer& leftmost() const
        { return header_.left; }
        const base_pointer& rightmost() const
        { return header_.right; }

        void print_consistency(base_pointer x) {
          if (x == 0) {
            std::cout << "Leaf";
          } else if (x == &header_) {
            std::cout << "Header";
          } else {
            std::cout << "Node {nKey = " << static_cast<node_pointer>(x)->data.key;
            std::cout << ", nColor = ";
            if (x->color == RBRed) {
              std::cout << "Red";
            } else {
              std::cout << "Black";
            }
            std::cout << ", nLeft = ";
            base_pointer left = x->left;
            if (left != 0 && left != &header_ && left->parent != x) {
              std::cout << "Faulty";
            } else {
              print_consistency(left);
            }
            std::cout << ", nRight = ";
            base_pointer right = x->right;
            if (right != 0 && right != &header_ && right->parent != x) {
              std::cout << "Faulty";
            } else {
              print_consistency(right);
            }
            std::cout << "}";
          }
        }

        // Destroys a node as well as it's children,
        //  leaves the tree incomplete w.r.t headers so be careful!
        void erase(base_pointer& n) {
          if (n == 0) {
            return;
          }
          if (n->left != 0 && n->left != &header_) {
            erase(n->left);
          }
          if (n->right != 0 && n->right != &header_) {
            erase(n->right);
          }
          delete n;
          n = 0;
        }

        ////////////////////////////////////////////////////////////////////////
        // rebalance_after_insert
        ////////////////////////////////////////////////////////////////////////
        void rebalance_after_insert(base_pointer x, base_pointer& root) {
          if (x == root) {
            x->color = RBBlack;
            return;
          } else if (x->parent->color == RBBlack) {
            return;
          }
          base_pointer uncle = node_base::sibling(x->parent);
          if (uncle == 0 || uncle == &header_ || uncle->color == RBBlack) {
            if (x->parent == x->parent->parent->left) {
              if (x == x->parent->right) {
                x = x->parent;
                rotate_left(x);
              }
              x->parent->color = RBBlack;
              x->parent->parent->color = RBRed;
              rotate_right(x->parent->parent);
            } else {
              if (x == x->parent->left) {
                x = x->parent;
                rotate_right(x);
              }
              x->parent->color = RBBlack;
              x->parent->parent->color = RBRed;
              rotate_left(x->parent->parent);
            }
          } else {
            x->parent->color = RBBlack;
            uncle->color = RBBlack;
            x->parent->parent->color = RBRed;
            rebalance_after_insert(x->parent->parent, root);
          }
        }

        ////////////////////////////////////////////////////////////////////////
        // rotate_right
        ////////////////////////////////////////////////////////////////////////
        void rotate_right(base_pointer x) {
          base_pointer y = x->left;
          x->left = y->right;
          if (y->right != 0) {
            y->right->parent = x;
          }
          y->parent = x->parent;
          if (x == root()) {
            root() = y;
          } else if (x == x->parent->right) {
            x->parent->right = y;
          } else {
            x->parent->left = y;
          }
          y->right = x;
          x->parent = y;
        }

        ////////////////////////////////////////////////////////////////////////
        // rotate_left
        ////////////////////////////////////////////////////////////////////////
        void rotate_left(base_pointer x) {
          base_pointer y = x->right;
          x->right = y->left;
          if (y->left != 0) {
            y->left->parent = x;
          }
          y->parent = x->parent;
          if (x == root()) {
            root() = y;
          } else if (x == x->parent->left) {
            x->parent->left = y;
          } else {
            x->parent->right = y;
          }
          y->left = x;
          x->parent = y;
        }

        ////////////////////////////////////////////////////////////////////////
        // fix_black
        // Using: http://en.wikipedia.org/wiki/Red-black_tree 
        ////////////////////////////////////////////////////////////////////////
        void fix_black(base_pointer p, bool left) {
          base_pointer n;
          base_pointer s;
          if (left) {
            n = p->left;
            s = p->right;
          } else {
            n = p->right;
            s = p->left;
          }
          key_type key = static_cast<node_pointer>(s)->data.key;
          // Case 2
          if (s->color == RBRed) {
            p->color = RBRed;
            s->color = RBBlack;
            if (left)
              rotate_left(p);
            else
              rotate_right(p);
          }
          if (p->left == s) {
            left = false;
            n = p->right;
            s = p->left;
          } else {
            left = true;
            n = p->left;
            s = p->right;
          }
          // Case 3
          if (p->color == RBBlack &&
              s->color == RBBlack &&
              node_base::is_black(s->left) &&
              node_base::is_black(s->right)) {
            s->color = RBRed;
            if (p != root()) {
              if (p == p->parent->left) {
                fix_black(p->parent, true);
              } else {
                fix_black(p->parent, false);
              }
            }
            return;
          }
          // Case 4
          if (p->color == RBRed &&
              s->color == RBBlack &&
              node_base::is_black(s->left) &&
              node_base::is_black(s->right)) {
            s->color = RBRed;
            p->color = RBBlack;
            return;
          }
          // Case 5
          if (left &&
              s->color == RBBlack &&
              !node_base::is_black(s->left) &&
              node_base::is_black(s->right))
          {
            s->color = RBRed;
            s->left->color = RBBlack;
            base_pointer sl = s->left;
            rotate_right(s);
            s = sl;
          } else if (!left &&
                     s->color == RBBlack &&
                     !node_base::is_black(s->right) &&
                     node_base::is_black(s->left)) {
            s->color = RBRed;
            s->right->color = RBBlack;
            base_pointer sr = s->right;
            rotate_left(s);
            s = sr;
          }
          // Case 6
          s->color = p->color;
          p->color = RBBlack;
          if (left) {
            // sibling(n)->color == RBBlack && sibling(n)->right->color = RBRed
            s->right->color = RBBlack;
            rotate_left(p);
          } else {
            // sibling(n)->color == RBBlack && sibling(n)->left->color = RBRed
            s->left->color = RBBlack;
            rotate_right(p);
          }
        }
        ////////////////////////////////////////////////////////////////////////
        // remove_single_childed
        ////////////////////////////////////////////////////////////////////////
        void remove_single_childed(base_pointer x) {
          base_pointer child;
          base_pointer s = node_base::sibling(x);
          if (x->right == 0 || x == rightmost()) {
            child = x->left;
          } else {
            child = x->right;
          }
          if (x == root()) {
            if (child == 0 || child == &header_) {
              root() = 0;
              leftmost() = &header_;
              rightmost() = &header_;
            } else {
              root() = child;
              child->color = RBBlack;
              if (x == leftmost()) {
                base_pointer y = child;
                while (y->left != 0) {
                  y = y->left;
                }
                leftmost() = y;
                y->left = &header_;
              }
              if (x == rightmost()) {
                base_pointer y = child;
                while (y->right != 0) {
                  y = y->right;
                }
                rightmost() = y;
              }
            }
            return;
          }
          bool left;
          if (child == 0 || child == &header_) {
            // no children
            if (x == x->parent->left) {
              x->parent->left = x->left;
              if (x == leftmost()) {
                leftmost() = x->parent;
              }
              left = true;
            } else {
              x->parent->right = x->right;
              if (x == rightmost()) {
                rightmost() = x->parent;
              }
              left = false;
            }
          } else {
            child->parent = x->parent;
            if (x == x->parent->left) {
              x->parent->left = child;
              left = true;
            } else {
              x->parent->right = child;
              left = false;
            }
            if (x == leftmost()) {
              base_pointer y = child;
              while (y->left != 0) {
                y = y->left;
              }
              leftmost() = y;
              y->left = &header_;
            }
            if (x == rightmost()) {
              base_pointer y = child;
              while (y->right != 0) {
                y = y->right;
              }
              rightmost() = y;
            }
            if (child->color == RBRed) {
              child->color = RBBlack;
              return;
            }
          }
          // Recolor
          if (x->color != RBRed) {
            fix_black(x->parent, left);
          }
        }

        ////////////////////////////////////////////////////////////////////////
        // rebalance_for_erase
        ////////////////////////////////////////////////////////////////////////
        node_pointer rebalance_for_erase(base_pointer x)
        {
          if (x->right != 0 && x != rightmost()
           && x->left != 0 && x != leftmost()) {
            // x has both children
            base_pointer y = x->right;
            while (y->left != 0) {
              y = y->left;
            }
            base_pointer xp = x->parent;
            base_pointer yp = y->parent;
            base_pointer xr = x->right;
            base_pointer yr = y->right;
            base_pointer xl = x->left;
            base_pointer yl = y->left;
            if (x == root()) {
              root() = y;
            } else if (x == xp->left) {
              xp->left = y;
            } else {
              xp->right = y;
            }
            y->parent = xp;
            if (yp == x) {
              y->right = x;
              x->parent = y;  
            } else {
              y->right = xr;
              xr->parent = y;
              yp->left = x;
              x->parent = yp;
            }
            y->left = xl;
            xl->parent = y;
            x->left = yl;
            x->right = yr;
            if (y == rightmost()) {
              rightmost() = x;
            } else if (yr != 0) {
              yr->parent = x;
            }
            swap(x->color, y->color);
          }
          remove_single_childed(x);
          return static_cast<node_pointer>(x);
        }

      private:
        node_base header_;
        size_t node_count_;
    };


  ////////////////////////////////////////////////////////////////////////////
  // Class template map
  // This is a simple map DDT
  ////////////////////////////////////////////////////////////////////////////
  template <typename T, typename IT, int I, typename L, typename Allocator>
    class map
    {
      public:
        ////////////////////////////////////////////////////////////////////////
        // Typing behaviour of DDT
        ////////////////////////////////////////////////////////////////////////
        typedef size_t key_type;

        typedef T mapped_type;
        typedef mapped_type* pointer;
        typedef const mapped_type * const_pointer;
        typedef mapped_type& reference;
        typedef const mapped_type& const_reference;

        typedef IT input_type;
        typedef const input_type& input_reference;
  
        typedef Rb_tree<mapped_type, input_type, I, L, Allocator> tree_type;

        typedef typename tree_type::iterator iterator;
        typedef typename tree_type::const_iterator const_iterator;

        ////////////////////////////////////////////////////////////////////////
        // Basic iterators of DDT
        ////////////////////////////////////////////////////////////////////////
        const_iterator begin() const { return tree_.begin(); }
        iterator begin() { return tree_.begin(); }
        const_iterator end() const { return tree_.end(); }
        iterator end() { return tree_.end(); }

        ////////////////////////////////////////////////////////////////////////
        // Basic operations of DDT
        ////////////////////////////////////////////////////////////////////////
        bool empty() const { return tree_.empty(); }
        key_type size() const { return tree_.size(); }

        const_iterator get(key_type n) {
          typename L::map_get S(I, n, sizeof(mapped_type));
          const_iterator i = tree_.find(n);
          if (i != tree_.end()) {
            S.present();
          }
          return i;
        }

        bool add(key_type n, input_reference value) {
          typename L::map_add S(I, n, sizeof(mapped_type));
          std::pair<iterator, bool> p = tree_.add(n, value);
          if (!p.second) {
            S.present();
          }
          return p.second;
        }

        bool remove(key_type n) {
          typename L::map_remove S(I, n, sizeof(mapped_type));
          bool p = tree_.remove(n);
          if (p) {
            S.present();
          }
          return p;
        }

        bool remove(iterator i) {
          typename L::map_remove S(I, (i.node_.key), sizeof(mapped_type));
          bool p = tree_.remove(i);
          if (p) {
            S.present();
          }
          return p;
        }

        void clear() {
          tree_.clear();
        }

        ////////////////////////////////////////////////////////////////////////
        // Syntactic sugar
        ////////////////////////////////////////////////////////////////////////
        reference operator[](size_t n) { 
          typename L::map_add S(I, n, sizeof(mapped_type));
          std::pair<iterator, bool> p = tree_.add(n, mapped_type());
          if (!p.second) {
            S.present();
          }
          return p.first;
        }
        const_reference operator[](key_type n) const {
          typename L::map_add S(I, n, sizeof(mapped_type));
          std::pair<iterator, bool> p = tree_.add(n, mapped_type());
          if (!p.second) {
            S.present();
          }
          return p.first;
        }

        ////////////////////////////////////////////////////////////////////////
        // map implementation
        ////////////////////////////////////////////////////////////////////////
        map() {
        }

        ~map() {
          clear();
        }
        
        void dump() {
          tree_.print_tree();
        }

    private:
        tree_type tree_;
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
