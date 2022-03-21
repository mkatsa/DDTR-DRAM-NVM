/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_NOLOGGER_HPP_
#define TRACING_NOLOGGER_HPP_
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // no_logger
  //   This is a simple logger that will log nothing
  //////////////////////////////////////////////////////////////////////////////
  class no_logger {
    public:
		struct object_construct {
			object_construct(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz) {}
			~object_construct() {}
		};

		struct object_copy {
			object_copy(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const unsigned int old_type_num,
				const unsigned int old_instance_id) {}
			~object_copy() {}
		};

		struct object_destruct {
			object_destruct(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz) {}
			~object_destruct() {}
		};

		struct object_swap {
			object_swap(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz, 
				const unsigned int old_type_num,
				const unsigned int old_instance_id) {}
			~object_swap() {}
		};

		struct object_resize {
			object_resize(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz) {}
			~object_resize() {}
		};
		struct object_get {
			object_get(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const size_t index) {}
			~object_get() {}
		};

		struct object_insert {
			object_insert(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr) {}
			~object_insert() {}
		};

		struct object_remove {
			object_remove(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr) {}
			~object_remove() {}
		};

		struct object_clear {
			object_clear(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz) {}
			~object_clear() {}
		};

		struct object_operator {
			object_operator(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id) {}
			~object_operator() {}
		};

		struct iterator_next {
			iterator_next(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr) {}
			~iterator_next() {}
		};

		struct iterator_previous {
			iterator_previous(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr) {}
			~iterator_previous() {}
		};

		struct iterator_add {
			iterator_add(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr,
				const size_t offset) {}
			~iterator_add() {}
		};

		struct iterator_sub {
			iterator_sub(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr,
				const size_t offset) {}
			~iterator_sub() {}
		};

		struct iterator_get {
			iterator_get(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr){}
			~iterator_get() {}
		};

		struct iterator_reset {
			iterator_reset(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const unsigned int index) {}
			~iterator_reset() {}
		};

		struct iterator_isdone {
			iterator_isdone(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id) {}
			~iterator_isdone() {}
		};


      //////////////////////////////////////////////////////////////////////////
      // map ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct map_get {
        map_get(const unsigned int id, const size_t n, const size_t sz) {
        }
        void present() { }
        ~map_get() {
        }
      };
      struct map_add {
        map_add(const unsigned int id, const size_t n, const size_t sz) {
        }
        void present() { }
        ~map_add() {
        }
      };
      struct map_remove {
        map_remove(const unsigned int id, const size_t n, const size_t sz) {
        }
        void present() { }
      };
      struct map_clear {
        map_clear(const unsigned int id, const size_t sz) {
        }
        ~map_clear() {
        }
      };

      //////////////////////////////////////////////////////////////////////////
      // var related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_read(const void * addr, const unsigned int id, const size_t sz) {
      }

      inline static void log_write(const void * addr, const unsigned int id, const size_t sz) {
      }

      //////////////////////////////////////////////////////////////////////////
      // allocated related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
      }

      inline static void log_malloc_end(const unsigned int id, const size_t sz, const void * addr) {
      }

      inline static void log_free_begin(const unsigned int id, const void * addr) {
      }

      inline static void log_free_end(const unsigned int id, const void * addr) {
      }

      //////////////////////////////////////////////////////////////////////////
      // pool related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_address_range(const unsigned int id, const void * addr, const size_t sz) {
      }

      inline static void log_used_memory(const unsigned int id, const void * addr, const size_t sz) {
      }

      //////////////////////////////////////////////////////////////////////////
      // scope related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_scope_begin(const std::string & scopeName) {
      }

      inline static void log_scope_end(const std::string & scopeName) {
      }
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
