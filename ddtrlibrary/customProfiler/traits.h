/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_TRAITS_H
#define TRACING_TRAITS_H
#include "allocated.h"
#include "nologger.h"

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Default Traits do nothing 
  //////////////////////////////////////////////////////////////////////////////
  struct no_traits {
    enum { type_num = 0 };
    template <typename T>
      struct transform {
        typedef T value_type;
          typedef T semilogged_value_type;
      };
    typedef no_logger Logger;
    typedef std_allocator Allocator;
  };

template <int N>
    struct std_traits {
      enum { type_num = 0 };
      template <typename T>
        struct transform {
          typedef T value_type;
            typedef T semilogged_value_type;
        };
      typedef no_logger Logger;
        typedef no_logging_allocator<N> Allocator;
    };

}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
