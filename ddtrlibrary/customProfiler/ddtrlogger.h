//
//  ddtrlogger.h
//  ddtr
//
//  Created by Christos Baloukas on 13/1/21.
//

#ifndef ddtrlogger_h
#define ddtrlogger_h


#include <string>
#include <map>
#include <cstdio>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // DDTRLogger
  //   This is a logger for DDTR. It captures accesses and
    // memory footprint and presents it in a log.out file.
  //////////////////////////////////////////////////////////////////////////////
class objectStats {
public:
    
private:
    
    unsigned long long numReads;
    unsigned long long numWrites;
    unsigned long long maxMemory;
    unsigned long long currMemory;
    
    friend class DDTRLogger;
    
};

  class DDTRLogger {
      

    public:
      
      unsigned long samplingReadCounter;
      unsigned long samplingWriteCounter;
      unsigned int samplingPercentage;
      unsigned int samplingCycles;
      unsigned long long numReads, numWrites, maxMemory, currMemory;
      
      std::map<int, objectStats> statsMap;
      std::map<const void*, size_t> allocationMap;
      std::map<const void*, size_t>::iterator m_Iter;
      typedef std::pair<const void*, size_t> allocationPair;
      
      const char* _fileName;
      
      
      void setLogFileName(const char* fileName) {
          _fileName = fileName;
      }
      void setSamplingPercentage(unsigned int percentage) {
          samplingPercentage = percentage;
          samplingCycles = 100/percentage;
      }
        
        static void addAllocation(const void* address, size_t sz)
        {
            getInstance()->allocationMap[address] = sz;
        }
        static size_t allocationSize(const void* address)
        {
            /*
            getInstance()->m_Iter = getInstance()->allocationMap.find(address);
            if (getInstance()->m_Iter == getInstance()->allocationMap.end())
                std::cout << "problem" << std::endl;
            return getInstance()->m_Iter->second;
             */
            size_t allocSize = getInstance()->allocationMap[address];
            if (allocSize == 0)
                std::cout << "problem" << std::endl;
            return allocSize;
        }
        ////////////////////////////////////////////////////////////////////////
        // DDT Library
        ////////////////////////////////////////////////////////////////////////
      
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
                const unsigned int old_instance_id) {};
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
                const size_t index)
            {}
            ~object_get() {}
        };

        struct object_insert {
            object_insert(
                const char * type_id,
                const unsigned int type_num,
                const unsigned int instance_id,
                const size_t sz) {}
            ~object_insert() {}
        };

        struct object_remove {
            object_remove(
                const char * type_id,
                const unsigned int type_num,
                const unsigned int instance_id,
                const size_t sz) {}
            ~object_remove() {}
        };

        struct object_clear {
            object_clear(
                const char * type_id,
                const unsigned int type_num,
                const unsigned int instance_id,
                const size_t sz)
            {}
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
                const void * addr) {}
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
                const unsigned int instance_id) {};
            ~iterator_isdone() {}
        };


      //////////////////////////////////////////////////////////////////////////
      // sequence ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct sequence_get {
        sequence_get(const unsigned int id, const size_t n, const size_t sz) {}
        ~sequence_get() {}
      };
      struct sequence_add {
        sequence_add(const unsigned int id, const size_t n, const size_t sz) {}
        ~sequence_add() {}
      };
      struct sequence_remove {
        sequence_remove(const unsigned int id, const size_t n, const size_t sz) {}
        ~sequence_remove() {}
      };
      struct sequence_clear {
        sequence_clear(const unsigned int id, const size_t sz) {}
        ~sequence_clear() {}
      };

      //////////////////////////////////////////////////////////////////////////
      // map ddt related logging
      //////////////////////////////////////////////////////////////////////////
      struct map_get {
        map_get(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {}
          
        void present() { present_ = true; }
        ~map_get() {}
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_add {
        map_add(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {}
          
        void present() { present_ = true; }
          
        ~map_add() {}
          
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_remove {
        map_remove(const unsigned int id, const size_t n, const size_t sz)
        : id_(id), n_(n), size_(sz), present_(false) {}
        void present() { present_ = true; }
        ~map_remove() {}
        private: const unsigned int id_; const size_t n_; const size_t size_; bool present_;
      };
      struct map_clear {
        map_clear(const unsigned int id, const size_t sz)
        : id_(id), size_(sz) {}
        ~map_clear() {}
        private: const unsigned int id_; const size_t size_;
      };

      //////////////////////////////////////////////////////////////////////////
      // var related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_read(const void * addr, const unsigned int id, const size_t sz) {
          if (++getInstance()->samplingReadCounter == getInstance()->samplingCycles) {
              getInstance()->statsMap[id].numReads++;
              getInstance()->numReads++;
              //statsMap[id].numReads++;
              getInstance()->samplingReadCounter = 0;
          }
          
      }

      inline static void log_write(const void * addr, const unsigned int id, const size_t sz) {
          if (++getInstance()->samplingWriteCounter == getInstance()->samplingCycles) {
              getInstance()->statsMap[id].numWrites++;
              getInstance()->numWrites++;
              //statsMap[id].numWrites++;
              getInstance()->samplingWriteCounter = 0;
          }
          
      }

      //////////////////////////////////////////////////////////////////////////
      // allocated related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
        
          getInstance()->currMemory += sz;
          if (getInstance()->maxMemory < getInstance()->currMemory)
              getInstance()->maxMemory = getInstance()->currMemory;
          
          getInstance()->statsMap[id].currMemory += sz;
          if (getInstance()->statsMap[id].maxMemory < getInstance()->statsMap[id].currMemory)
              getInstance()->statsMap[id].maxMemory = getInstance()->statsMap[id].currMemory;
      }

      inline static void log_malloc_end(const unsigned int id, const size_t sz, const void * addr) {
        
          getInstance()->addAllocation(addr, sz);
      }

      inline static void log_free_begin(const unsigned int id, const void * addr) {
          //std::cout << "currMemory = " << getInstance()->currMemory << std::endl;
          //std::cout << "allocationSize = " << getInstance()->allocationSize(addr) << std::endl;
          DDTRLogger* instance = getInstance();
          size_t allocationSize = getInstance()->allocationSize(addr);
          getInstance()->currMemory -= getInstance()->allocationSize(addr);
          getInstance()->statsMap[id].currMemory -= getInstance()->allocationSize(addr);
          //std::cout << "new currMemory = " << getInstance()->currMemory << std::endl;
        
      }

      inline static void log_free_end(const unsigned int id, const void * addr) {
        
      }

      //////////////////////////////////////////////////////////////////////////
      // pool related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_address_range(const unsigned int id, const void * addr, const size_t sz) { }

      inline static void log_used_memory(const unsigned int id, const void * addr, const size_t sz) {}

      //////////////////////////////////////////////////////////////////////////
      // scope related logging
      //////////////////////////////////////////////////////////////////////////
      inline static void log_scope_begin(const std::string & scopeName) {}

      inline static void log_scope_end(const std::string & scopeName) {}


      static DDTRLogger * getInstance() {
        static DDTRLogger logger;
        return &logger;
      }
      
    private:
      DDTRLogger(): samplingPercentage(100), samplingCycles(1), numReads(0), numWrites(0), maxMemory(0), currMemory(0) {}

      ~DDTRLogger() {
          std::ofstream file2;
          long numReads = getInstance()->numReads;
          long numWrites = getInstance()->numWrites;
          long maxMemory = getInstance()->maxMemory;
          
          file2.open(_fileName);
          file2 << "totalReads = " << numReads << std::endl;
          file2 << "totalWrites = " << numWrites << std::endl;
          file2 << "totalAccesses = " << numReads + numWrites << std::endl;
          file2 << "totalMaxMemory = " << maxMemory << std::endl;
          
          file2 << std::endl << "STATS PER DDT" << std::endl;
          //unsigned long i = 0;
          for (std::map<int, objectStats>::iterator iter = statsMap.begin(); iter != statsMap.end(); iter++)
          {
              file2 << std::endl;
              file2 << "id = " << iter->first << std::endl;
              file2 << "{" << std::endl;
              file2 << "reads = " << iter->second.numReads << std::endl;
              file2 << "writes = " << iter->second.numWrites << std::endl;
              file2 << "total accesses = " << iter->second.numReads + iter->second.numWrites << std::endl;
              file2 << "max memory = " << iter->second.maxMemory << std::endl;
              file2 << "}" << std::endl;
          }
          file2.close();
      }


      
  };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////


#endif /* ddtrlogger_h */
