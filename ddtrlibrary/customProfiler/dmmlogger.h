/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_DMMLOGGER_HPP_
#define TRACING_DMMLOGGER_HPP_

#include <string>
#include <map>
#include <cstdio>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // DMMLogger
  //   This is the centralized logger that ensures that the logging format is 
  //   compatible with the O'Caml analysis toolset.
  //////////////////////////////////////////////////////////////////////////////
  class DMMLogger {
      enum LogType {
        ////////////////////////////////////////////////////////////////////////
        // var
        ////////////////////////////////////////////////////////////////////////
        LOG_VAR_READ = 0,
        LOG_VAR_WRITE = 1,
        ////////////////////////////////////////////////////////////////////////
        // scope
        ////////////////////////////////////////////////////////////////////////
        LOG_SCOPE_BEGIN = 2,
        LOG_SCOPE_END = 3,
        ////////////////////////////////////////////////////////////////////////
        // allocated
        ////////////////////////////////////////////////////////////////////////
        LOG_MALLOC_BEGIN = 4,
        LOG_MALLOC_END = 5,
        LOG_FREE_BEGIN = 6, 
        LOG_FREE_END = 7,
        ////////////////////////////////////////////////////////////////////////
        // sequence ddt
        ////////////////////////////////////////////////////////////////////////
        LOG_SEQUENCE_GET = 11,
        LOG_SEQUENCE_ADD = 12,
        LOG_SEQUENCE_REMOVE = 13,
        LOG_SEQUENCE_CLEAR = 14,
        LOG_SEQUENCE_END = 19,
        ////////////////////////////////////////////////////////////////////////
        // map ddt
        ////////////////////////////////////////////////////////////////////////
        LOG_MAP_START = 20,
        LOG_MAP_GET = 21,
        LOG_MAP_ADD = 22,
        LOG_MAP_REMOVE = 23,
        LOG_MAP_CLEAR = 24,
        ////////////////////////////////////////////////////////////////////////
        // DDT Library
        ////////////////////////////////////////////////////////////////////////
        LOG_OBJECT_CONSTRUCT   = 30,
        LOG_OBJECT_COPY        = 31,
        LOG_OBJECT_DESTRUCT    = 32,
        LOG_OBJECT_SWAP        = 33,
        LOG_OBJECT_RESIZE      = 34,
        LOG_OBJECT_GET         = 35,
        LOG_OBJECT_INSERT      = 36,
        LOG_OBJECT_REMOVE      = 37,
        LOG_OBJECT_CLEAR       = 38,
		LOG_OBJECT_OPERATOR    = 39,
		LOG_OBJECT_END         = 40,
        LOG_ITERATOR_NEXT      = 41,
        LOG_ITERATOR_PREVIOUS  = 42,
        LOG_ITERATOR_ADD       = 43,
        LOG_ITERATOR_SUB       = 44,
        LOG_ITERATOR_GET       = 45,
		LOG_ITERATOR_RESET     = 46,
		LOG_ITERATOR_ISDONE    = 47,
        ////////////////////////////////////////////////////////////////////////
        // pool
        ////////////////////////////////////////////////////////////////////////
        LOG_USED_MEMORY = 254,
        LOG_ADDRESS_RANGE = 255
      };

    public:
      /*struct object_stats {
          unsigned long long numReads;
          unsigned long long numWrites;
          unsigned long long maxMemory;
          unsigned long long currMemory;
      };*/
      
		static unsigned long long numReads, numWrites, maxMemory, currMemory;
      //static std::map<int, object_stats> statsMap;
		static std::map<const void*, size_t> allocationMap;
		static std::map<const void*, size_t>::iterator m_Iter;
		typedef std::pair<const void*, size_t> allocationPair;
        
		static void addAllocation(const void* address, size_t sz) 
		{
			allocationMap.insert(allocationPair(address, sz));
		}
		static size_t allocationSize(const void* address) 
		{
			m_Iter = allocationMap.find(address);
			if (m_Iter == allocationMap.end())
				std::cout << "problem" << std::endl;
			return m_Iter->second;
		}
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
				getInstance()->write_header(LOG_OBJECT_CONSTRUCT, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz;
			}
			~object_construct() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
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
				getInstance()->write_header(LOG_OBJECT_COPY, 6*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz
					<< old_type_num
					<< old_instance_id;
			};
			~object_copy() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_destruct {
			object_destruct(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				getInstance()->write_header(LOG_OBJECT_DESTRUCT, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz;
			};
			~object_destruct() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_OBJECT_SWAP, 6*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< old_type_num
				<< old_instance_id;
			}
			~object_swap() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_resize {
			object_resize(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
			  getInstance()->write_header(LOG_OBJECT_RESIZE, 4*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz;
			}
			~object_resize() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_OBJECT_GET, 5*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< index;
			}
			~object_get() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_insert {
			object_insert(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				getInstance()->write_header(LOG_OBJECT_INSERT, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz;
			};
			~object_insert() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_remove {
			object_remove(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				getInstance()->write_header(LOG_OBJECT_REMOVE, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz;
			};
			~object_remove() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_clear {
			object_clear(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const size_t sz)
			{
				getInstance()->write_header(LOG_OBJECT_CLEAR, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< sz;
			};
			~object_clear() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct object_operator {
			object_operator(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id)
			{
				getInstance()->write_header(LOG_OBJECT_OPERATOR, 3*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id;
			};
			~object_operator() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_ITERATOR_NEXT, 5*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< addr;
			}
			~iterator_next() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_ITERATOR_PREVIOUS, 5*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< addr;
			}
			~iterator_previous() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_ITERATOR_ADD, 6*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< addr
				<< offset;
			}
			~iterator_add() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_ITERATOR_SUB, 6*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< addr
				<< offset;
			}
			~iterator_sub() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
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
			  getInstance()->write_header(LOG_ITERATOR_GET, 5*sizeof(unsigned int))
				<< getInstance()->getTypeNum(type_id)
				<< type_num
				<< instance_id
				<< sz
				<< addr;
			}
			~iterator_get() {
			  getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct iterator_reset {
			iterator_reset(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id,
				const unsigned int index)
			{
				getInstance()->write_header(LOG_ITERATOR_RESET, 4*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id
					<< index;
			};
			~iterator_reset() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};

		struct iterator_isdone {
			iterator_isdone(
				const char * type_id,
				const unsigned int type_num,
				const unsigned int instance_id)
			{
				getInstance()->write_header(LOG_ITERATOR_ISDONE, 3*sizeof(unsigned int))
					<< getInstance()->getTypeNum(type_id)
					<< type_num
					<< instance_id;
			};
			~iterator_isdone() {
				getInstance()->write_header(LOG_OBJECT_END, 0);
			}
		};


      //////////////////////////////////////////////////////////////////////////
      // sequence ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct sequence_get {
        sequence_get(const unsigned int id, const size_t n, const size_t sz) {
          getInstance()->write_header(LOG_SEQUENCE_GET, 3*sizeof(unsigned int)) << id << n << sz;
        }
        ~sequence_get() {
          getInstance()->write_header(LOG_SEQUENCE_END, 0);
        }
      };
      struct sequence_add {
        sequence_add(const unsigned int id, const size_t n, const size_t sz) {
          getInstance()->write_header(LOG_SEQUENCE_ADD, 3*sizeof(unsigned int)) << id << n << sz;
        }
        ~sequence_add() {
          getInstance()->write_header(LOG_SEQUENCE_END, 0);
        }
      };
      struct sequence_remove {
        sequence_remove(const unsigned int id, const size_t n, const size_t sz) {
          getInstance()->write_header(LOG_SEQUENCE_REMOVE, 3*sizeof(unsigned int)) << id << n << sz;
        }
        ~sequence_remove() {
          getInstance()->write_header(LOG_SEQUENCE_END, 0);
        }
      };
      struct sequence_clear {
        sequence_clear(const unsigned int id, const size_t sz) {
          getInstance()->write_header(LOG_SEQUENCE_CLEAR, 2*sizeof(unsigned int)) << id << sz;
        }
        ~sequence_clear() {
          getInstance()->write_header(LOG_SEQUENCE_END, 0);
        }
      };

      //////////////////////////////////////////////////////////////////////////
      // map ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct map_get {
        map_get(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          getInstance()->write_header(LOG_MAP_START, 0);

        }
        void present() { present_ = true; }
        ~map_get() {
          getInstance()->write_header(LOG_MAP_GET, 3*sizeof(unsigned int) + sizeof(unsigned char))
            << id_ << n_ << size_ << (unsigned char)(present_ ? '\01' : '\00');
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_add {
        map_add(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          getInstance()->write_header(LOG_MAP_START, 0);
        }
        void present() { present_ = true; }
        ~map_add() {
          getInstance()->write_header(LOG_MAP_ADD, 3*sizeof(unsigned int) + sizeof(unsigned char))
            << id_ << n_ << size_ << (unsigned char)(present_ ? '\01' : '\00');
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_remove {
        map_remove(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {
          getInstance()->write_header(LOG_MAP_START, 0);
        }
        void present() { present_ = true; }
        ~map_remove() {
          getInstance()->write_header(LOG_MAP_REMOVE, 3*sizeof(unsigned int) + sizeof(unsigned char))
            << id_ << n_ << size_ << (unsigned char)(present_ ? '\01' : '\00');
        }
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_clear {
        map_clear(const unsigned int id, const size_t sz)
        : id_(id), size_(sz) {
          getInstance()->write_header(LOG_MAP_START, 0);
        }
        ~map_clear() {
          getInstance()->write_header(LOG_MAP_CLEAR, 2*sizeof(unsigned int)) << id_ << size_;
        }
        private: const unsigned int id_; const size_t size_;
      };

      //////////////////////////////////////////////////////////////////////////
      // var related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_read(const void * addr, const unsigned int id, const size_t sz) {
        getInstance()->write_header(LOG_VAR_READ, 3*sizeof(unsigned int)) << id << addr << sz;
		++numReads;
          //statsMap[id].numReads++;
      }

      inline static void log_write(const void * addr, const unsigned int id, const size_t sz) {
        getInstance()->write_header(LOG_VAR_WRITE, 3*sizeof(unsigned int)) << id << addr << sz;
		++numWrites;
          //statsMap[id].numWrites++;
      }

      //////////////////////////////////////////////////////////////////////////
      // allocated related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
        getInstance()->write_header(LOG_MALLOC_BEGIN, 2*sizeof(unsigned int)) << id << sz;
		currMemory += sz;
		if (maxMemory < currMemory)
			maxMemory = currMemory;
      }

      inline static void log_malloc_end(const unsigned int id, const size_t sz, const void * addr) {
        getInstance()->write_header(LOG_MALLOC_END, 3*sizeof(unsigned int)) << id << addr << sz;
		addAllocation(addr, sz);
      }

      inline static void log_free_begin(const unsigned int id, const void * addr) {
        getInstance()->write_header(LOG_FREE_BEGIN, 2*sizeof(unsigned int)) << id << addr;
		currMemory -= allocationSize(addr);
      }

      inline static void log_free_end(const unsigned int id, const void * addr) {
        getInstance()->write_header(LOG_FREE_END, 2*sizeof(unsigned int)) << id << addr;
      }

      //////////////////////////////////////////////////////////////////////////
      // pool related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_address_range(const unsigned int id, const void * addr, const size_t sz) {
        getInstance()->write_header(LOG_ADDRESS_RANGE, 3*sizeof(unsigned int)) << id << addr << sz;
      }

      inline static void log_used_memory(const unsigned int id, const void * addr, const size_t sz) {
        getInstance()->write_header(LOG_USED_MEMORY, 3*sizeof(unsigned int)) << id << addr << sz;
      }

      //////////////////////////////////////////////////////////////////////////
      // scope related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_scope_begin(const std::string & scopeName) {
        getInstance()->logScope(LOG_SCOPE_BEGIN, scopeName);
      }

      inline static void log_scope_end(const std::string & scopeName) {
        getInstance()->logScope(LOG_SCOPE_END, scopeName);
      }

      void logScope(LogType logType, const std::string & funcName) {
        std::pair<functionmap::iterator, bool> x = functionMap_.insert(functionmap::value_type(funcName, functionCtr_++));
        if (!x.second) {
          functionCtr_--;
        }
        write_header(logType, 1*sizeof(unsigned int)) << (*(x.first)).second;
      }

      //////////////////////////////////////////////////////////////////////////
      // type related logging
      //////////////////////////////////////////////////////////////////////////
      
      unsigned int getTypeNum(std::string name) {
        std::pair<typemap::iterator, bool> x = typeMap_.insert(typemap::value_type(name, typeCtr_++));
        if (!x.second) {
          typeCtr_--;
        }
        return (*(x.first)).second;
      }

      //////////////////////////////////////////////////////////////////////////
      // setLogFile: choose the logfile
      //////////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(__CYGWIN__) 
#define fopen64 fopen
#endif
      void setLogFile(const char * fileName) {
        fileName_ = fileName;
        if (logFile_ != NULL) {
          fclose(logFile_);
          logFile_ = NULL;
        }
        logFile_ = fopen(fileName_, "wb");
        // Header
        setvbuf(logFile_, write_buffer, _IOFBF, 1024*1024);
        const unsigned char magic[3] = {0x0A, 0x51, 0x50};
        const unsigned char lsb_test_char[4] = { 0x01, 0x02, 0x03, 0x04 };
        const unsigned int lsb_test_int = 0x01020304;
        fwrite(magic, sizeof(unsigned char), 3, logFile_);
        const unsigned char version = 0x04;
        fwrite((unsigned char *)&version, sizeof(unsigned char), 1, logFile_);
        fwrite(lsb_test_char, sizeof(unsigned char), 4, logFile_);
        fwrite((unsigned char *)&lsb_test_int, sizeof(unsigned int), 1, logFile_);
      }
#if defined(WIN32) || defined(__CYGWIN__) 
#undef fopen64
#endif

      //////////////////////////////////////////////////////////////////////////
      // setAlternateFile: choose the mapFile
      //////////////////////////////////////////////////////////////////////////
      void setAlternateFile(const char * fileName) {
        altFileName_ = fileName;
//         fprintf(altLogFile_, "functionMap = {\n");
//         for (functionmap::iterator i = functionMap_.begin(); i != functionMap_.end(); i++) {
//           fprintf(altLogFile_, "  %d -> %s\n", (*i).second, (*i).first.c_str());
//         }
//         fprintf(altLogFile_, "}\n");
//         if (altLogFile_ != stderr) {
//           altLogFile_ = stderr;
//         }
        altLogFile_ = fopen(altFileName_, "wt");
      }

      static DMMLogger * getInstance() {
        static DMMLogger logger;
        return &logger;
      }

    private:
      DMMLogger()
        : fileName_(NULL)
        , altFileName_(NULL)
        , logFile_(NULL)
        , altLogFile_(stderr)
        , functionCtr_(0) {
        write_buffer = new char[1024*1024];
      }

      ~DMMLogger() {
        if (logFile_ != NULL) { fclose(logFile_); }
        fprintf(altLogFile_, "functionMap = {\n");
        for (functionmap::iterator i = functionMap_.begin(); i != functionMap_.end(); i++) {
          fprintf(altLogFile_, "  %d -> %s\n", (*i).second, (*i).first.c_str());
        }
        fprintf(altLogFile_, "}\n");
        fprintf(altLogFile_, "typeMap = {\n");
        for (typemap::iterator i = typeMap_.begin(); i != typeMap_.end(); i++) {
          fprintf(altLogFile_, "  %d -> %s\n", (*i).second, (*i).first.c_str());
        }
        fprintf(altLogFile_, "}\n");
        if ((altLogFile_ != NULL)
         && (altLogFile_ != stderr)) {
          fclose(altLogFile_);
        }
        delete[] write_buffer;

		std::ofstream file2;
		file2.open("log.out");
		file2 << "accesses = " << numReads+numWrites << std::endl;
		file2 << "memory = " << maxMemory << std::endl;
		file2.close();
      }

      DMMLogger & write_header(LogType logType, unsigned short logSize) {
        unsigned short logType_s = (unsigned short) logType;
        if (logFile_ != NULL) {
          fwrite(&logType_s, sizeof(unsigned short), 1, logFile_);
          fwrite(&logSize, sizeof(unsigned short), 1, logFile_);
        }
        return *this;
      }

      DMMLogger & operator<<(const unsigned long num) {
        if (logFile_ != NULL) {
          fwrite(&num, sizeof(unsigned int), 1, logFile_);
        }
        return *this;
      }

      DMMLogger & operator<<(const void * addr) {
        if (logFile_ != NULL) {
          fwrite(&addr, sizeof(unsigned int), 1, logFile_);
        }
        return *this;
      }

      inline FILE * getLogFile() { return logFile_; }
      const char * fileName_;
      const char * altFileName_;
      FILE * logFile_;
      FILE * altLogFile_;

      DMMLogger(const DMMLogger&);
      DMMLogger & operator=(const DMMLogger&);
      typedef std::map<std::string, unsigned int> functionmap;
      typedef std::map<std::string, unsigned int> typemap;
      functionmap functionMap_;
      typemap typeMap_;
      unsigned int functionCtr_;
      unsigned int typeCtr_;
      char * write_buffer;
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif
