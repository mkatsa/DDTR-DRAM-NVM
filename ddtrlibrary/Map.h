#ifndef MAP_H
#define MAP_H

#include <type_traits>

#include "DDTIncludes.h"
#include "Rb-tree.h"
#include "avltree.h"


namespace DDTLibrary {

    template <class _Pair>
    struct _Select1st : public std::unary_function<_Pair,
                               typename _Pair::first_type>
         {
           typename _Pair::first_type&
           operator()(_Pair& __x) const
           { return __x.first; }
    
           const typename _Pair::first_type&
           operator()(const _Pair& __x) const
           { return __x.first; }
         };
    
    template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
          typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class map {
    public:
        typedef _Key                    key_type;
        typedef _Tp                    mapped_type;
        typedef std::pair<const _Key, _Tp>        value_type;
        typedef _Compare                    key_compare;
        typedef _Alloc                    allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
        
    private:
        typedef typename _Alloc::template
      rebind<value_type>::other _Pair_alloc_type;
        
        typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
                 key_compare, _Pair_alloc_type> _Rep_type;
        
        _Rep_type _M_t;
        
        typedef std::allocator_traits<_Pair_alloc_type> _Alloc_traits;
        
        
        struct Stats {
            unsigned long callsToOperatorDereference;
            unsigned long callsToFind;
            unsigned long callsToInsert;
            
            Stats(): callsToOperatorDereference(0), callsToFind(0), callsToInsert(0) {
                
            }
        } mapStats;
        
    public:
        
        
        typedef typename _Alloc_traits::pointer         pointer;
        typedef typename _Alloc_traits::const_pointer     const_pointer;
        typedef typename _Rep_type::iterator         iterator;
        typedef typename _Rep_type::const_iterator     const_iterator;
        typedef typename _Rep_type::size_type         size_type;
        typedef typename _Rep_type::difference_type     difference_type;
        typedef typename _Rep_type::reverse_iterator     reverse_iterator;
        typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
        
        map() = default;
        
        map(const map&) = default;
        map(map&&) = default;
        
        
        ~map() = default;
        /*~map() {
            std::cout << "stats for map" << std::endl;
            std::cout << "treeSize = " << _M_t.nodeCount() << std::endl;
            std::cout << "callsToOperatorDereference = " << mapStats.callsToOperatorDereference << std::endl;
            std::cout << "callsToFind = " << mapStats.callsToFind << std::endl;
            std::cout << "callsToInsert = " << mapStats.callsToInsert << std::endl;
             
        }*/
        
        iterator
        begin()
        { return _M_t.begin(); }
        
        const_iterator
        begin() const
        { return _M_t.begin(); }
        
        iterator
        end()
        { return _M_t.end(); }
        
        const_iterator
        end() const 
        { return _M_t.end(); }
        
        bool empty() const
        {
            return _M_t.empty();
            
        }
        
        mapped_type&
        operator[](const key_type& __k)
        {
            mapStats.callsToOperatorDereference++;
            
            iterator __i = lower_bound(__k);
            // __i->first is greater than or equivalent to __k.
            if (__i == end() || key_comp()(__k, (*__i).first))
                __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
                          std::tuple<const key_type&>(__k),
                          std::tuple<>());
  
            return (*__i).second;
        }
        
        mapped_type&
        operator[](key_type&& __k) {
            
            mapStats.callsToOperatorDereference++;
            
            iterator __i = lower_bound(__k);
            
            // __i->first is greater than or equivalent to __k.
            if (__i == end() || key_comp()(__k, (*__i).first))
              __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
                            std::forward_as_tuple(std::move(__k)),
                            std::tuple<>());
            return (*__i).second;
        }
        
        iterator
        lower_bound(const key_type& __x) {
            return _M_t.lower_bound(__x);
        }
        
#if __cplusplus > 201103L
        template<typename _Kt>
        auto lower_bound(const _Kt& __x)
    -> decltype(iterator(_M_t._M_lower_bound_tr(__x)))
    { return iterator(_M_t._M_lower_bound_tr(__x)); }
#endif
        
        const_iterator
        lower_bound(const key_type& __x) const
        { return _M_t.lower_bound(__x); }

    #if __cplusplus > 201103L
        template<typename _Kt>
      auto
      lower_bound(const _Kt& __x) const
      -> decltype(const_iterator(_M_t._M_lower_bound_tr(__x)))
      { return const_iterator(_M_t._M_lower_bound_tr(__x)); }
    #endif
        
        
        iterator
        erase(const_iterator __position)
        { return _M_t.erase(__position); }

        iterator
        erase(iterator __position)
        { return _M_t.erase(__position); }
        
        iterator
        erase(const_iterator __first, const_iterator __last)
        { return _M_t.erase(__first, __last); }
        
        key_compare
        key_comp() const
        { return _M_t.key_comp(); }
        
        iterator
        find(const key_type& __x)
        {
            mapStats.callsToFind++;
            
            return _M_t.find(__x);
            
        }
        
#if __cplusplus > 201103L
      template<typename _Kt>
    auto
    find(const _Kt& __x) -> decltype(_M_t._M_find_tr(__x))
    {
        mapStats.callsToFind++;
        
        return _M_t._M_find_tr(__x);
        
    }
#endif
        
        std::pair<iterator, bool>
        insert(const value_type& __x)
        {
            mapStats.callsToInsert++;
            
            return _M_t._M_insert_unique(__x);
            
        }
        
        std::pair<iterator, bool>
        insert(value_type&& __x)
        {
            mapStats.callsToInsert++;
            
            return _M_t._M_insert_unique(std::move(__x));
            
        }
        
        /*
        template<typename _Pair>
      std::enable_if_t<std::is_constructible<value_type, _Pair>::value,
                std::pair<iterator, bool>>
      insert(_Pair&& __x)
      { return _M_t._M_emplace_unique(std::forward<_Pair>(__x)); }
        
        iterator
        find(const key_type& __x)
        { return _M_t.find(__x); }
*/
        
    };
    
    template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
          typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class mapAVL {
    public:
        typedef _Key                    key_type;
        typedef _Tp                    mapped_type;
        typedef std::pair<const _Key, _Tp>        value_type;
        typedef _Compare                    key_compare;
        typedef _Alloc                    allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
        
    private:
        typedef typename _Alloc::template
      rebind<value_type>::other _Pair_alloc_type;
        
        typedef AVLTree<key_type, value_type, _Select1st<value_type>,
                 key_compare, _Pair_alloc_type> _Rep_type;
        
        _Rep_type _M_t;
        
        typedef std::allocator_traits<_Pair_alloc_type> _Alloc_traits;
        
        
        struct Stats {
            unsigned long callsToOperatorDereference;
            unsigned long callsToFind;
            unsigned long callsToInsert;
            
            Stats(): callsToOperatorDereference(0), callsToFind(0), callsToInsert(0) {
                
            }
        } mapStats;
        
    public:
        
        
        typedef typename _Alloc_traits::pointer         pointer;
        typedef typename _Alloc_traits::const_pointer     const_pointer;
        typedef typename _Rep_type::iterator         iterator;
        typedef typename _Rep_type::constIterator     const_iterator;
        typedef typename _Rep_type::sizeType         size_type;
        typedef typename _Rep_type::differenceType     difference_type;
        typedef typename _Rep_type::reverseIterator     reverse_iterator;
        typedef typename _Rep_type::constReverseIterator const_reverse_iterator;
        
        mapAVL() = default;
        
        mapAVL(const mapAVL&) = default;
        mapAVL(mapAVL&&) = default;
        
        
        ~mapAVL() = default;
        //~mapAVL() {
            /*std::cout << "stats for map" << std::endl;
            std::cout << "treeSize = " << _M_t.nodeCount() << std::endl;
            std::cout << "callsToOperatorDereference = " << mapStats.callsToOperatorDereference << std::endl;
            std::cout << "callsToFind = " << mapStats.callsToFind << std::endl;
            std::cout << "callsToInsert = " << mapStats.callsToInsert << std::endl;
             */
        //}
        
        iterator
        begin()
        { return _M_t.begin(); }
        
        const_iterator
        begin() const
        { return _M_t.begin(); }
        
        iterator
        end()
        { return _M_t.end(); }
        
        const_iterator
        end() const
        { return _M_t.end(); }
        
        bool empty() const
        {
            return _M_t.empty();
            
        }
        
        mapped_type&
        operator[](const key_type& __k)
        {
            mapStats.callsToOperatorDereference++;
            
            iterator __i = lower_bound(__k);
            // __i->first is greater than or equivalent to __k.
            if (__i == end() || key_comp()(__k, (*__i).first))
                __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
                          std::tuple<const key_type&>(__k),
                          std::tuple<>());
  
            return (*__i).second;
        }
        
        mapped_type&
        operator[](key_type&& __k) {
            
            mapStats.callsToOperatorDereference++;
            
            iterator __i = lower_bound(__k);
            
            // __i->first is greater than or equivalent to __k.
            if (__i == end() || key_comp()(__k, (*__i).first))
              __i = _M_t.emplaceHintUnique(__i, std::piecewise_construct,
                            std::forward_as_tuple(std::move(__k)),
                            std::tuple<>());
            return (*__i).second;
        }
        
        iterator
        lower_bound(const key_type& __x) {
            return _M_t.lowerBound(__x);
        }
        
#if __cplusplus > 201103L
        template<typename _Kt>
        auto lower_bound(const _Kt& __x)
    -> decltype(iterator(_M_t.lowerBoundTr(__x)))
    { return iterator(_M_t.lowerBoundTr(__x)); }
#endif
        
        const_iterator
        lower_bound(const key_type& __x) const
        { return _M_t.lowerBound(__x); }

    #if __cplusplus > 201103L
        template<typename _Kt>
      auto
      lower_bound(const _Kt& __x) const
      -> decltype(const_iterator(_M_t.lowerBoundTr(__x)))
      { return const_iterator(_M_t.lowerBoundTr(__x)); }
    #endif
        
        
        iterator
        erase(const_iterator __position)
        { return _M_t.erase(__position); }

        iterator
        erase(iterator __position)
        { return _M_t.erase(__position); }
        
        iterator
        erase(const_iterator __first, const_iterator __last)
        { return _M_t.erase(__first, __last); }
        
        key_compare
        key_comp() const
        { return _M_t.keyComp(); }
        
        iterator
        find(const key_type& __x)
        {
            mapStats.callsToFind++;
            
            return _M_t.find(__x);
            
        }
        
#if __cplusplus > 201103L
      template<typename _Kt>
    auto
    find(const _Kt& __x) -> decltype(_M_t.findTr(__x))
    {
        mapStats.callsToFind++;
        
        return _M_t.findTr(__x);
        
    }
#endif
        
        std::pair<iterator, bool>
        insert(const value_type& __x)
        {
            mapStats.callsToInsert++;
            
            return _M_t._M_insert_unique(__x);
            
        }
        
        std::pair<iterator, bool>
        insert(value_type&& __x)
        {
            mapStats.callsToInsert++;
            
            return _M_t._M_insert_unique(std::move(__x));
            
        }
        
        /*
        template<typename _Pair>
      std::enable_if_t<std::is_constructible<value_type, _Pair>::value,
                std::pair<iterator, bool>>
      insert(_Pair&& __x)
      { return _M_t._M_emplace_unique(std::forward<_Pair>(__x)); }
        
        iterator
        find(const key_type& __x)
        { return _M_t.find(__x); }
*/
        
    };
    
    
    
    
}

#endif
