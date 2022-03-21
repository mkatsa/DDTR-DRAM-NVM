//
//  main.cpp
//  testing-memory
//
//  Created by Christos Baloukas on 30/6/21.
//
#include "../../definitions.h"
#include "../../config.h"
#include <iostream>
#include <random>

template <typename T, int ID>
void testDDT(unsigned int numOfElements) {
    
    Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);
    
    typename ddt_helper<T, ID>::SLLLIST sll1;
    typename ddt_helper<T, ID+1>::VECTOR v1;
    
    for (int i = 0; i < numOfElements; i++) {
        sll1.push_back(*new T);
        v1.push_back(*new T);
        
    }
    
    sll1.erase(sll1.begin()+5);
    
    v1.clear();
    sll1.clear();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    testDDT<compositeObject, 3>(1000);
    
    return 0;
}
