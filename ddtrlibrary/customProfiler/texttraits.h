/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_TEXTTRAITS_HPP
#define TRACING_TEXTTRAITS_HPP
#include "allocated.h"
#include "nologger.h"
#include "textlogger.h"

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Default Traits do nothing 
  //////////////////////////////////////////////////////////////////////////////
  template <int I>
  struct text_traits {
    enum { type_num = I };
    template <typename T>
      struct transform {
        typedef T value_type;
      };
    typedef text_logger Logger;
    typedef logged_allocator<I, text_logger> Allocator;
  };

  template <int I>
  struct text_semi_traits {
    enum { type_num = I };
    template <typename T>
      struct transform {
        typedef T value_type;
      };
    typedef text_logger Logger;
    typedef std_allocator Allocator;
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
