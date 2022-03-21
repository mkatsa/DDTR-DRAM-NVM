# DDTR-DRAM-NVM

This repository provides a tool for dynamic data type exploration and evaluation over heteoegeneous DRAM/NVM systems. We provide the Dynamic Data Type Refinement Library (DDTR) library, an Standard Template Library(STL)-like library with alternative data structure implementations. Moreover, we provide an automated tool for profiling and monitoring based on [Intel VTune Profiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/vtune-profiler.html#gs.u2cjso) and evaluating the energy consumption over DRAM/Intel Optane DC heterogeneous memory systems. 

## Clone this repository

```
git clone --recursive https://github.com/mkatsa/DDTR-DRAM-NVM.git
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
 
