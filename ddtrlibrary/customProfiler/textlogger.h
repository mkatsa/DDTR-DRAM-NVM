/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_TEXTLOGGER_HPP_
#define TRACING_TEXTLOGGER_HPP_
#include <string>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // text_logger
  //   This is a simple logger that will log to stdout in textual format
  //////////////////////////////////////////////////////////////////////////////
  class text_logger {
    public:
		
		////////////////////////////////////////////////////////////////////////
		// DDT Library
		////////////////////////////////////////////////////////////////////////
		struct object_construct {
			object_construct(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				std::cout
					<< "LOG_OBJECT_CONSTRUCT_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << std::endl;
			};
			~object_construct() {
				std::cout 
					<< "LOG_OBJECT_CONSTRUCT_END" << std::endl;
			}
		};

		struct object_copy {
			object_copy(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const unsigned int old_type_num,
				const unsigned int old_instance_id)
			{
				std::cout
					<< "LOG_OBJECT_COPY_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << " "
					<< old_type_num << " "
					<< old_instance_id << std::endl;
			};
			~object_copy() {
				std::cout 
					<< "LOG_OBJECT_COPY_END" << std::endl;
			}
		};

		struct object_destruct {
			object_destruct(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				std::cout
					<< "LOG_OBJECT_DESTRUCT_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << std::endl;
			};
			~object_destruct() {
				std::cout 
					<< "LOG_OBJECT_DESTRUCT_END" << std::endl;
			}
		};

		struct object_swap {
			object_swap(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz, 
				const unsigned int old_type_num,
				const unsigned int old_instance_id)
			{
			  std::cout
				<< "LOG_OBJECT_SWAP_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< old_type_num << " "
				<< old_instance_id << std::endl;
			}
			~object_swap() {
				std::cout 
					<< "LOG_OBJECT_SWAP_END" << std::endl;
			}
		};

		struct object_resize {
			object_resize(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
			  std::cout
				<< "LOG_OBJECT_RESIZE_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << std::endl;
			}
			~object_resize() {
			  std::cout 
				<< "LOG_OBJECT_RESIZE_END" << std::endl;
			}
		};		
		struct object_get {
			object_get(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const size_t index)
			{
			  std::cout
				<< "LOG_OBJECT_GET_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< index << std::endl;
			}
			~object_get() {
			  std::cout 
				<< "LOG_OBJECT_GET_END" << std::endl;
			}
		};

		struct object_insert {
			object_insert(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				std::cout
					<< "LOG_OBJECT_INSERT_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << std::endl;
			};
			~object_insert() {
				std::cout 
					<< "LOG_OBJECT_INSERT_END" << std::endl;
			}
		};

		struct object_remove {
			object_remove(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				std::cout
					<< "LOG_OBJECT_REMOVE_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << std::endl;
			};
			~object_remove() {
				std::cout 
					<< "LOG_OBJECT_REMOVE_END" << std::endl;
			}
		};

		struct object_clear {
			object_clear(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				std::cout
					<< "LOG_OBJECT_CLEAR_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< sz << std::endl;
			};
			~object_clear() {
				std::cout 
					<< "LOG_OBJECT_CLEAR_END" << std::endl;
			}
		};

		struct object_operator {
			object_operator(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id)
			{
				std::cout
					<< "LOG_OBJECT_OPERATOR_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << std::endl;
			};
			~object_operator() {
				std::cout 
					<< "LOG_OBJECT_OPERATOR_END" << std::endl;
			}
		};

		struct iterator_next {
			iterator_next(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr)
			{
			  std::cout
				<< "LOG_ITERATOR_NEXT_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< addr << std::endl;
			}
			~iterator_next() {
			  std::cout 
				<< "LOG_ITERATOR_NEXT_END" << std::endl;
			}
		};

		struct iterator_previous {
			iterator_previous(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr)
			{
			  std::cout
				<< "LOG_ITERATOR_PREVIOUS_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< addr << std::endl;
			}
			~iterator_previous() {
			  std::cout 
				<< "LOG_ITERATOR_PREVIOUS_END" << std::endl;
			}
		};

		struct iterator_add {
			iterator_add(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr,
				const size_t offset)
			{
			  std::cout
				<< "LOG_ITERATOR_ADD_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< addr << " "
				<< offset << std::endl;
			}
			~iterator_add() {
			  std::cout 
				<< "LOG_ITERATOR_ADD_END" << std::endl;
			}
		};

		struct iterator_sub {
			iterator_sub(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr,
				const size_t offset)
			{
			  std::cout
				<< "LOG_ITERATOR_SUB_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< addr << " "
				<< offset << std::endl;
			}
			~iterator_sub() {
			  std::cout 
				<< "LOG_ITERATOR_SUB_END" << std::endl;
			}
		};

		struct iterator_get {
			iterator_get(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz,
				const void * addr)
			{
			  std::cout
				<< "LOG_ITERATOR_GET_BEGIN "
				<< type_id << " "
				<< type_num << " "
				<< instance_id << " "
				<< sz << " "
				<< addr << std::endl;
			}
			~iterator_get() {
			  std::cout 
				<< "LOG_ITERATOR_GET_END" << std::endl;
			}
		};
		struct iterator_reset {
			iterator_reset(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const unsigned int index)
			{
				std::cout
					<< "LOG_ITERATOR_RESET_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << " "
					<< index << std::endl;
			};
			~iterator_reset() {
				std::cout 
					<< "LOG_ITERATOR_RESET_END" << std::endl;
			}
		};

		struct iterator_isdone {
			iterator_isdone(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id)
			{
				std::cout
					<< "LOG_ITERATOR_ISDONE_BEGIN "
					<< type_id << " "
					<< type_num << " "
					<< instance_id << std::endl;
			};
			~iterator_isdone() {
				std::cout 
					<< "LOG_ITERATOR_ISDONE_END" << std::endl;
			}
		};


      //////////////////////////////////////////////////////////////////////////
      // map ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct map_get {
        map_get(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          std::cout << "LOG_MAP_START" << std::endl;
        }
        void present() { present_ = true; }
        ~map_get() {
          std::cout << "LOG_MAP_GET " << id_ << " " << n_ << " " << size_ << " " << present_ << std::endl;
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_add {
        map_add(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          std::cout << "LOG_MAP_START" << std::endl;
        }
        void present() { present_ = true; }
        ~map_add() {
          std::cout << "LOG_MAP_ADD " << id_ << " " << n_ << " " << size_ << " " << present_ << std::endl;
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_remove {
        map_remove(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          std::cout << "LOG_MAP_START" << std::endl;
        }
        void present() { present_ = true; }
        ~map_remove() {
          std::cout << "LOG_MAP_REMOVE " << id_ << " " << n_ << " " << size_ << " " << present_ << std::endl;
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_clear {
        map_clear(const unsigned int id, const size_t sz)
        : id_(id), size_(sz) {
          std::cout << "LOG_MAP_START" << std::endl;
        }
        ~map_clear() {
           std::cout << "LOG_MAP_CLEAR " << id_ << " " << size_ << std::endl;
        }
        private: const unsigned int id_; const size_t size_;
      };

      //////////////////////////////////////////////////////////////////////////
      // sequence ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct sequence_get {
        sequence_get(const unsigned int id, const size_t n, const size_t sz) {
          std::cout << "LOG_SEQUENCE_GET " << id << " " << n << " " << sz << std::endl;
        }
        ~sequence_get() {
          std::cout << "LOG_SEQUENCE_END" << std::endl;
        }
      };
      struct sequence_add {
        sequence_add(const unsigned int id, const size_t n, const size_t sz) {
          std::cout << "LOG_SEQUENCE_ADD " << id << " "<< n << " " << sz << std::endl;
        }
        ~sequence_add() {
          std::cout << "LOG_SEQUENCE_END" << std::endl;
        }
      };
      struct sequence_remove {
        sequence_remove(const unsigned int id, const size_t n, const size_t sz) {
          std::cout << "LOG_SEQUENCE_REMOVE " << id << " "<< n << " " << sz << std::endl;
        }
        ~sequence_remove() {
          std::cout << "LOG_SEQUENCE_END" << std::endl;
        }
      };
      struct sequence_clear {
        sequence_clear(const unsigned int id, const size_t sz) {
          std::cout << "LOG_SEQUENCE_CLEAR " << id << " " << sz << std::endl;
        }
        ~sequence_clear() {
          std::cout << "LOG_SEQUENCE_END" << std::endl;
        }
      };
      
      //////////////////////////////////////////////////////////////////////////
      // var related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_read(const void * addr, const unsigned int id, const size_t sz) {
        std::cout << "LOG_VAR_READ " << id << " "<< addr << " "<< sz << std::endl;
      }

      inline static void log_write(const void * addr, const unsigned int id, const size_t sz) {
        std::cout << "LOG_VAR_WRITE " << id << " "<< addr << " "<< sz << std::endl;
      }

      //////////////////////////////////////////////////////////////////////////
      // allocated related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
        std::cout << "LOG_MALLOC_BEGIN " << id << " "<< sz << std::endl;
      }

      inline static void log_malloc_end(const unsigned int id, const size_t sz, const void * addr) {
        std::cout << "LOG_MALLOC_END " << id << " "<< addr << " "<< sz << std::endl;
      }

      inline static void log_free_begin(const unsigned int id, const void * addr) {
        std::cout << "LOG_FREE_BEGIN " << id << " "<< addr << std::endl;
      }

      inline static void log_free_end(const unsigned int id, const void * addr) {
        std::cout << "LOG_FREE_END " << id << " "<< addr << std::endl;
      }

      //////////////////////////////////////////////////////////////////////////
      // pool related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_address_range(const unsigned int id, const void * addr, const size_t sz) {
        std::cout << "LOG_ADDRESS_RANGE " << id << " "<< addr << " "<< sz << std::endl;
      }

      inline static void log_used_memory(const unsigned int id, const void * addr, const size_t sz) {
        std::cout << "LOG_USED_MEMORY " << id << " "<< addr << " "<< sz << std::endl;
      }

      //////////////////////////////////////////////////////////////////////////
      // scope related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_scope_begin(const std::string & scopeName) {
        std::cout << "LOG_SCOPE_BEGIN " << scopeName << std::endl;
      }

      inline static void log_scope_end(const std::string & scopeName) {
        std::cout << "LOG_SCOPE_END " << scopeName << std::endl;
      }
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
