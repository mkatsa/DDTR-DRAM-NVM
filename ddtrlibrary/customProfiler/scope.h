/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 * This is a function-call tracer.  It is a template that will accept a logger
 * that it will then use to log scope entries and exits.
 * Like var, it uses the DMMLogger unless
 * Using this scope--tracer is quite simple.  Just make a variable
 *  scope func("name-of-function");
 * as first variable inside a scope and the constructor and destructor
 * will automatically take care of it.  Of course this could be used to trace
 * any sort of scope and hence is called 'scope'.
 *
 */

#ifndef TRACING_SCOPE_HPP
#define TRACING_SCOPE_HPP
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Standard logger:
  //   This is the template that a logger must follow
  //   The standard logger does nothing
  //////////////////////////////////////////////////////////////////////////////
  class std_scope_logger {
    public:
      inline static void log_scope_begin(const std::string & s) {
      }
      inline static void log_scope_end(const std::string & s) {
      }
  };

  //////////////////////////////////////////////////////////////////////////////
  // Class template scope:
  //   Traces scope entries and exits
  //////////////////////////////////////////////////////////////////////////////
  template <class L = std_scope_logger>
  class scope
  {
    public:
      scope(std::string name) 
        : name_(name)
      {
        L::log_scope_begin(name_);
      };
      ~scope() {
        L::log_scope_end(name_);
      };
    private:
      std::string name_;
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
