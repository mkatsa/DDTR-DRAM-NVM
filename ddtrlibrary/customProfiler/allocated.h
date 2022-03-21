/*
 * Tracing LIBRARY
 * Authors: Christophe Poucet
 * This work is property of Christophe Poucet
 *
 */
#ifndef TRACING_ALLOCATED_HPP
#define TRACING_ALLOCATED_HPP
#include <iostream>
#include <fstream>

#include <string>     // std::string, std::stoi

#ifdef DDTR_QUARTZ
#include <pmalloc.h>
#endif


#ifdef OPTANE
#include <memkind.h>
#include <limits.h>
#define PMEM_MAX_SIZE (102410241024)
static char path[PATH_MAX]="/mnt/pmem1"; 
static struct memkind *pmem_kind=NULL;
#endif

#include <cstdlib>
////////////////////////////////////////////////////////////////////////////////
// Begin namespace Tracing
////////////////////////////////////////////////////////////////////////////////
namespace Tracing {
  //////////////////////////////////////////////////////////////////////////////
  // Standard allocator:
  //   This is the template that an allocator must follow
  //   The standard allocator uses the system's malloc and free
  //////////////////////////////////////////////////////////////////////////////

  class std_allocator {
    public:
      inline static void * malloc(const size_t sz) { return ::malloc(sz); }
      inline static void free(void * ptr)          { return ::free(ptr); }
  };

  //////////////////////////////////////////////////////////////////////////////
  // Empty allocator
  //   This allocator does nothing.  Use this for variables that must be traced
  //   inside a struct that derives from allocated
  //////////////////////////////////////////////////////////////////////////////
  class no_allocator {
    public:
      inline static void * malloc(const size_t sz) { return 0; }
      inline static void free(void * ptr)          { ; }
  };

  //////////////////////////////////////////////////////////////////////////////
  // Standard MemLogger logger:
  //   This is an empty class that is used by default by the MemLogger
  //////////////////////////////////////////////////////////////////////////////
  class std_memlogger_logger {
    public:
      inline static void log_malloc_begin(const unsigned int id, const size_t sz) {}
      inline static void log_malloc_end(const unsigned int id, const size_t sz, const void * addr) {}
      inline static void log_free_begin(const unsigned int id, const void * addr) {}
      inline static void log_free_end(const unsigned int id, const void * addr) {}

  };

#ifdef DDTR_QUARTZ

static std::string filename = "/NVMemul/DDTR-NVM/allocator/allocation_results/DDTs_allocation.csv";
static int memory_table[100] = {0};    //Modify that to list or something else...
static int init_alloc = 0;
static int total_objects = 0;

template <int N, typename L = std_memlogger_logger>
    class logged_allocator {
        public:

            //add function for pmalloc()
            inline static void * malloc(const size_t sz) {

                if( init_alloc == 0 ){
                    init_allocation();
                    init_alloc = 1;
                    for(int i = 0; i <= total_objects - 1; i++)
                    {
                        if(memory_table[i] == 0)
                            std::cout << i << " on DRAM" << std::endl;
                        else
                            std::cout << i << " on NVM" << std::endl;
                    }
                }

                if(memory_table[N] == 0)
                {
                    //allocate on DRAM
                    L::log_malloc_begin(N, sz);
                    void * ptr = ::malloc(sz);
                    L::log_malloc_end(N, sz, ptr);
                    return ptr;
                }
                else
                {
                    //allocate on NVM
                    void *ptr = pmalloc(sz);
                    return ptr;
                }
            }

            inline static void free(void * ptr) {

                if(memory_table[N] == 0)
                {
                    //std::cout << "DRAM" << std::endl;
                    //allocate on DRAM
                    L::log_free_begin(N, ptr);
                    ::free(ptr);
                    L::log_free_end(N, ptr);
                }
                else
                {
                    //free on NVM
                    pfree(ptr, sizeof(ptr));
                }
            }


            inline static void * init_allocation() {

                std::ifstream myFile(filename);
                std::string line, colname;
                int lineno = 0;
                std::string target_mem;
                std::string id;
                int i = 0;
                int j = 0;

                if (!myFile.is_open()) {
                    std::cout << "No allocation file found." << std::endl;
                    std::cout << "Assuming Single DRAM system!" << std::endl;
                    //throw std::runtime_error("Could not open file");
                }
                
                if (myFile.good()) {
                    std::cout << "Assuming Hybrid DRAM-NVM system!" << std::endl;
                    while (std::getline(myFile, line))
                    {
                        if (lineno == 0)
                        {
                            lineno++;
                            continue;
                        }
                        else
                        {
                            std::string delimiter = ",";
                            size_t pos = 0;
                            int columnno = 0;
                            std::string token;
                            while ((pos = line.find(delimiter)) != std::string::npos) {

                                token = line.substr(0, pos);
                                if (columnno == 0 )
                                {
                                    id = token;
                                }
                                else if (columnno == 2)
                                {
                                    target_mem = token;
                                    if ( target_mem == "NVM" )        //1 for NVM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 1;
                                        total_objects++;
                                    }
                                    else if ( target_mem == "DRAM" )    //0 for DRAM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 0;
                                        total_objects++;
                                    }
                                    
                                    break;
                                }
                                columnno++;
                                line.erase(0, pos + delimiter.length());
                            }
                        }
                    }
                }
            }
    };




    template <int N>
    class no_logging_allocator {
    public:
        inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
            
        }
        inline static void log_free_begin(const unsigned int id, const void * addr) {
            
        }
        inline static void * malloc(const size_t sz) {

          if( init_alloc == 0 ){
                    init_allocation();
                    init_alloc = 1;
                    for(int i = 0; i <= total_objects - 1; i++)
                    {
                        if(memory_table[i] == 0)
                            std::cout << i << " on DRAM" << std::endl;
                        else
                            std::cout << i << " on NVM" << std::endl;
                    }
                }

                if(memory_table[N] == 0)
                {
                    //allocate on DRAM
                    log_malloc_begin(N, sz);
                    void * ptr = ::malloc(sz);
                    return ptr;
                }

                else
                {
                    //allocate on NVM
                    void *ptr = pmalloc(sz);
                    return ptr;
                }
        }
        inline static void free(void * ptr)          
        {

          if(memory_table[N] == 0)
                {
                    //std::cout << "DRAM" << std::endl;
                    //allocate on DRAM
                    log_free_begin(N, ptr);
                    ::free(ptr);
                }
                else
                {
                    //free on NVM
                    pfree(ptr, sizeof(ptr));
                }
        }

        inline static void * init_allocation() {

                std::ifstream myFile(filename);
                std::string line, colname;
                int lineno = 0;
                std::string target_mem;
                std::string id;
                int i = 0;
                int j = 0;

                if (!myFile.is_open()) {
                    std::cout << "No allocation file found." << std::endl;
                    std::cout << "Assuming Single DRAM system!" << std::endl;
                    //throw std::runtime_error("Could not open file");
                }
                
                if (myFile.good()) {
                    std::cout << "Assuming Hybrid DRAM-NVM system!" << std::endl;
                    while (std::getline(myFile, line))
                    {
                        if (lineno == 0)
                        {
                            lineno++;
                            continue;
                        }
                        else
                        {
                            std::string delimiter = ",";
                            size_t pos = 0;
                            int columnno = 0;
                            std::string token;
                            while ((pos = line.find(delimiter)) != std::string::npos) {

                                token = line.substr(0, pos);
                                if (columnno == 0 )
                                {
                                    id = token;
                                }
                                else if (columnno == 2)
                                {
                                    target_mem = token;
                                    if ( target_mem == "NVM" )        //1 for NVM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 1;
                                        total_objects++;
                                    }
                                    else if ( target_mem == "DRAM" )    //0 for DRAM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 0;
                                        total_objects++;
                                    }
                                    
                                    break;
                                }
                                columnno++;
                                line.erase(0, pos + delimiter.length());
                            }
                        }
                    }
                }
            }
    };

//===================================================START OPTANE CODE HERE===================================================================

#elif OPTANE

static std::string filename = "/NVMemul/DDTR-NVM/allocator/allocation_results/DDTs_allocation.csv";
static int memory_table[100] = {0};    //Modify that to list or something else...
static int init_alloc = 0;
static long long int ddts_footprint = 0;
static int total_objects = 0;

template <int N, typename L = std_memlogger_logger>
    class logged_allocator {
        public:

            //add function for pmalloc()
            inline static void * malloc(const size_t sz) {

                if( init_alloc == 0 ){
                    init_allocation();
                    init_alloc = 1;
                    for(int i = 0; i <= total_objects - 1; i++)
                    {
                        if(memory_table[i] == 0)
                            std::cout << i << " on DRAM" << std::endl;
                        else
                            std::cout << i << " in OPTANE" << std::endl;
                    }
                }

                if(memory_table[N] == 0)
                {
                    //allocate on DRAM
                    L::log_malloc_begin(N, sz);
                    void * ptr = ::malloc(sz);
                    L::log_malloc_end(N, sz, ptr);
                    return ptr;
                }
                else
                {
                    //change to allocate on Optane
                    //std::cout << "OPTANE Logged Malloc" << std::endl;
                    void *ptr = ( void * )memkind_malloc(pmem_kind,sz);
                    return ptr;
                }
            }

            inline static void free(void * ptr) {

                if(memory_table[N] == 0)
                {
                    //std::cout << "DRAM" << std::endl;
                    //allocate on DRAM
                    L::log_free_begin(N, ptr);
                    ::free(ptr);
                    L::log_free_end(N, ptr);
                }
                else
                {
                    //change to free on Optane
                    //std::cout << "OPTANE Logged Free" << std::endl;
                    memkind_free(pmem_kind, ptr);
                }
            }


            inline static void * init_allocation() {

                int err = memkind_create_pmem(path, PMEM_MAX_SIZE, &pmem_kind);
                if (err) 
                { 
                    exit(1); 
                }

                std::ifstream myFile(filename);
                std::string line, colname;
                int lineno = 0;
                std::string target_mem;
                std::string id;
                int i = 0;
                int j = 0;

                if (!myFile.is_open()) {
                    std::cout << "No allocation file found." << std::endl;
                    std::cout << "Assuming Single DRAM system!" << std::endl;
                }
                
                if (myFile.good()) {
                    std::cout << "Assuming Hybrid DRAM-NVM system!" << std::endl;
                    while (std::getline(myFile, line))
                    {
                        if (lineno == 0)
                        {
                            lineno++;
                            continue;
                        }
                        else
                        {
                            //std::cout << line << std::endl;
                            std::string delimiter = ",";
                            size_t pos = 0;
                            int columnno = 0;
                            std::string token;
                            while ((pos = line.find(delimiter)) != std::string::npos) {

                                token = line.substr(0, pos);
                                if ( columnno == 0 )
                                {
                                    id = token;
                                }
                                else if ( columnno == 1 )
                                {
                                    //std::cout << token << std::endl;
                                    ddts_footprint += stoll(token);
                                    //std::cout << ddts_footprint << std::endl;
                                }
                                else if ( columnno == 2 )
                                {
                                    target_mem = token;
                                    if ( target_mem == "NVM" )        //1 for NVM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 1;
                                        total_objects++;
                                    }
                                    else if ( target_mem == "DRAM" )    //0 for DRAM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 0;
                                        total_objects++;
                                    }
                                    
                                    break;
                                }
                                columnno++;
                                line.erase(0, pos + delimiter.length());
                            }
                        }
                    }
                }
            }
    };




    template <int N>
    class no_logging_allocator {
    public:
        inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
            
        }
        inline static void log_free_begin(const unsigned int id, const void * addr) {
            
        }
        inline static void * malloc(const size_t sz) {

          if( init_alloc == 0 ){
                    init_allocation();
                    init_alloc = 1;
                    for(int i = 0; i <= total_objects - 1; i++)
                    {
                        if(memory_table[i] == 0)
                            std::cout << i << " on DRAM" << std::endl;
                        else
                            std::cout << i << " in OPTANE" << std::endl;
                    }
                }

                if(memory_table[N] == 0)
                {
                    //allocate on DRAM
                    log_malloc_begin(N, sz);
                    void * ptr = ::malloc(sz);
                    return ptr;
                }

                else
                {
                    //allocate on NVM
                    //std::cout << "OPTANE No-Logged Malloc" << std::endl;
                    void *ptr = ( void * )memkind_malloc(pmem_kind,sz);
                    return ptr;
                }
        }
        inline static void free(void * ptr)          
        {

          if(memory_table[N] == 0)
                {
                    //std::cout << "DRAM" << std::endl;
                    //allocate on DRAM
                    log_free_begin(N, ptr);
                    ::free(ptr);
                }
                else
                {
                    //free on NVM
                    //std::cout << "OPTANE No-Logged Free" << std::endl;
                    memkind_free(pmem_kind, ptr);
                }
        }

        inline static void * init_allocation() {

                int err = memkind_create_pmem(path, PMEM_MAX_SIZE, &pmem_kind);
                if (err) 
                { 
                    exit(1); 
                }

                std::ifstream myFile(filename);
                std::string line, colname;
                int lineno = 0;
                std::string target_mem;
                std::string id;
                int i = 0;
                int j = 0;

                if (!myFile.is_open()) {
                    std::cout << "No allocation file found." << std::endl;
                    std::cout << "Assuming Single DRAM system!" << std::endl;
                }
                
                if (myFile.good()) {
                    std::cout << "Assuming Hybrid DRAM-NVM system!" << std::endl;
                    while (std::getline(myFile, line))
                    {
                        if (lineno == 0)
                        {
                            lineno++;
                            continue;
                        }
                        else
                        {
                            //std::cout << line << std::endl;
                            std::string delimiter = ",";
                            size_t pos = 0;
                            int columnno = 0;
                            std::string token;
                            while ((pos = line.find(delimiter)) != std::string::npos) {

                                token = line.substr(0, pos);
                                if ( columnno == 0 )
                                {
                                    id = token;
                                }
                                else if ( columnno == 1 )
                                {
                                    //std::cout << token << std::endl;
                                    ddts_footprint += stoll(token);
                                    //std::cout << ddts_footprint << std::endl;
                                }
                                else if ( columnno == 2 )
                                {
                                    target_mem = token;
                                    if ( target_mem == "NVM" )        //1 for NVM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 1;
                                        total_objects++;
                                    }
                                    else if ( target_mem == "DRAM" )    //0 for DRAM
                                    {
                                        int int_id = stoi(id);
                                        memory_table[int_id] = 0;
                                        total_objects++;
                                    }
                                    
                                    break;
                                }
                                columnno++;
                                line.erase(0, pos + delimiter.length());
                            }
                        }
                    }
                }
            }
    };


//===================================================END OPTANE CODE HERE=====================================================================


#else

  //////////////////////////////////////////////////////////////////////////////
  // Logged allocator
  //   This allocator does nothing.  Use this for variables that must be traced
  //   inside a struct that derives from allocated
  //////////////////////////////////////////////////////////////////////////////
    template <int N>
    class no_logging_allocator {
    public:
        inline static void log_malloc_begin(const unsigned int id, const size_t sz) {
            
        }
        inline static void log_free_begin(const unsigned int id, const void * addr) {
            
        }
        inline static void * malloc(const size_t sz) {
          log_malloc_begin(N, sz);
          //std::cout << "tetet";
          void * ptr = ::malloc(sz);
          return ptr;
        }
        inline static void free(void * ptr)          {
          log_free_begin(N, ptr);
          ::free(ptr);
        }
    };
    
  template <int N, typename L = std_memlogger_logger>
  class logged_allocator {
    public:
      inline static void * malloc(const size_t sz) { 
        
        L::log_malloc_begin(N, sz);
        void * ptr = ::malloc(sz);
        L::log_malloc_end(N, sz, ptr);
        return ptr;
      }
      inline static void free(void * ptr)          {
        L::log_free_begin(N, ptr);
        ::free(ptr);
        L::log_free_end(N, ptr);
      }
  };

#endif


  //////////////////////////////////////////////////////////////////////////////
  // allocated:
  //   Derive from this to use the allocator specified in the template
  //////////////////////////////////////////////////////////////////////////////
  template <class A>
    class allocated{
      public:
        void * operator new(const size_t sz)   { return A::malloc(sz); }
        void operator delete(void * p)         { return A::free(p); }
        void * operator new[](const size_t sz) { return A::malloc(sz); }
        void operator delete[](void * p)       { return A::free(p); }
    };
}
////////////////////////////////////////////////////////////////////////////////
// End namespace Tracing
////////////////////////////////////////////////////////////////////////////////
#endif