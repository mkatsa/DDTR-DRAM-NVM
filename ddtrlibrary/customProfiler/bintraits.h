/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_BINTRAITS_HPP
#define TRACING_BINTRAITS_HPP
#include "allocated.h"
#include "dmmlogger.h"
#include "ddtrlogger.h"
#include "nologger.h"

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Default Traits do nothing 
  //////////////////////////////////////////////////////////////////////////////
  template <int I>
  struct bin_traits {
    enum { type_num = I };
    template <typename T>
      struct transform {
        typedef T value_type;
      };
    
      typedef no_logger Logger;
    typedef logged_allocator<I, DMMLogger> Allocator;
  };

    template <int I>
    struct ddtr_traits {
      enum { type_num = I };
      template <typename T>
        struct transform {
          typedef T value_type;
        };
      
        typedef DDTRLogger Logger;
      typedef logged_allocator<I, DDTRLogger> Allocator;
    };



  template <int I>
  struct bin_semi_traits {
    enum { type_num = I };
    template <typename T>
      struct transform {
        typedef T value_type;
      };
    typedef DMMLogger Logger;
    typedef std_allocator Allocator;
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
