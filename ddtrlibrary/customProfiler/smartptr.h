/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_SMARTPTR_HPP
#define TRACING_SMARTPTR_HPP
#include "allocated.h"
////////////////////////////////////////////////////////////////////////////////
// begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Class Template smartptr
  //////////////////////////////////////////////////////////////////////////////
  template <class T, class Traits = no_traits>
    class smartptr : public allocated<typename Traits::Allocator>
    {
      public:
        typedef typename Traits::Logger Logger;
        typedef typename Traits::Allocator Allocator;
        
        typedef T Type;
        typedef T* Ptr;

        smartptr(Type * p)
          : p_(p)
          { 
            ref_ = (int *)Allocator::malloc(sizeof(int));
            *ref_ = 1;
          }

        smartptr(const smartptr & rhs)
          : p_(rhs.p_), ref_(rhs.ref_)
          { 
            ++*ref_;
          }

        smartptr & operator=(const smartptr & rhs)
        {
          smartptr copy(rhs);
          Ptr tmp = copy.p_;
          copy.p_ = p_;
          p_ = tmp;
          int * r = copy.ref_;
          copy.ref_ = ref_;
          ref_ = r;
          return *this;
        }   

        ~smartptr() 
        {
          if (--*ref_ == 0) {
            Allocator::free(ref_);
            delete p_;
          }
        }

        Type & operator*() { return *p_; }
        Ptr operator->() { return p_; }
      private:
        Ptr p_;
        mutable int * ref_;
    };
}
////////////////////////////////////////////////////////////////////////////////
// end namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
