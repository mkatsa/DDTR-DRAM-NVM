# DDTR-DRAM-NVM

This repository provides a tool for dynamic data type exploration and evaluation over heteoegeneous DRAM/NVM systems. We provide the Dynamic Data Type Refinement Library (DDTR) library, an Standard Template Library(STL)-like library with alternative data structure implementations. Moreover, we provide an automated tool for profiling and monitoring based on [Intel VTune Profiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/vtune-profiler.html#gs.u2cjso) and evaluating the energy consumption over DRAM/Intel Optane DC heterogeneous memory systems. 

## Clone this repository

```
git clone --recursive https://github.com/mkatsa/DDTR-DRAM-NVM.git
```

## Dependencies

### PCM Tool Build

PENVMTool relies on [Processor Counter Monitor(PCM)](https://github.com/opcm/pcm) tool, which is an application programmind interface(API) and a set of tools based on the API to monitor performance and energy metrics for Intel® Core™, Xeon®, Atom™ and Xeon Phi™ processors. PCM works on Linux, Windows, Mac OS X, FreeBSD, DragonFlyBSD and ChromeOS operating systems.

Build PCM:

```
$ cd ./pcm
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

All the utilitis of PCM will be built in _build/bin_ directory. If you want to accelerate the building phase you can use the _--parallel_ flag:

```
cmake --build . --parallel
```

### Intel Optane DC configurations and libraries

If you will not utilize Intel Optane DC then you can skip the next steps and proceed directly to the **DDTR-library** section.

Intel Optane DC must be configured in App Direct mode. Documentation about the setup and the usage of Intel Optane in App Direct mode can be found [here](https://www.intel.com/content/dam/support/us/en/documents/memory-and-storage/data-center-persistent-mem/Intel-Optane-DC-Persistent-Memory-Quick-Start-Guide.pdf). In our experiments we configured Intel Optane namespace in _fsdax_ mode, which allows the direct access programming model. 
The execution of PENVMTool on Intel Optane DC Persistent Memory is based on [PMDK](https://github.com/pmem/pmdk).

### Mount Ext4-DAX File System

Next, we create and mount an ext4-dax file to the device _/dev/pmem1_, which corresponds to our persistent memory. If you have specified a different device path, please modify the following commands to indicate the correct device.

```
$ sudo mkfs.ext4 -b 4096 -E stride=512 -F /dev/pmem1
$ sudo mount -o dax /dev/pmem1 /mnt/pmem1
```

### [PMDK](https://github.com/pmem/pmdk) Installation

```
git clone https://github.com/pmem/pmdk.git
cd pmdk
make -j
cd ..
```

## DDTR-library

DDTR library source code exists on _./ddtrlibtary_ directory
We provide a simple motivational example for the usage of the DDTR library on C++ applications. The source code is on _motivational\_example.cpp_ source code file. The definition of the dynamic data types can be configured on _./ddtrlibrary/config.h_ file. Our library consists of 7 alternative data structure implementations:

* **VEC:** An array that can grow or shrink dynamically(Vector). 
* **SLL:** A Single Linked List
* **SLLR:** A Single Linked List with a pointer to the last accessed element(Roving pointer)
* **DLL:** A Double Linked List
* **DLLR:** A Double Linked List with a pointer to the last accessed element(Roving pointer)
* **MAP-RB:** An associative container(map in STL) that stores pairs of key-value in a Red-Black tree
* **MAP-AVL:** An associative container(map in STL) that stores pairs of key-value in a AVL tree

Our library is also built to map all the dynamic memory allocations to Intel Optane 
 
### Build & Execute DDTR Design Space Exploration

We provide an example for providing DDTR exploration over the motivational example. However this can be applied to any application that DDTR has been integrated on.
```
$ cd scripts
$ export SOURCE_CODE_ROOT=/absolute/path/to/source/code/root/directory
$ export EXECUTABLE_ROOT=/absolute/path/to/binary/executable/directory
$ ./run_all.sh motivation 4
```
After the execution in the _SOURCE\_CODE\_ROOT_ directory there have been produced several .out files. The _pareto\_combinations.txt_ file indicates all the ID alternatives for the pareto optimal solutions, aiming to minimize the number of memory footprint and the number of memory accesses. For all the alternative solutions, it exists a _log\_ID.out_ file, which indicates more in detail the data structure implementations utilized for the corresponding solution. Finally, _pareto.pdf_ illustrates the pareto line of the design space exploration.
