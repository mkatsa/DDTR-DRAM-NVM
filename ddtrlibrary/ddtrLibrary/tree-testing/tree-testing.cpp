//
//  tree-testing.cpp
//  tree-testing
//
//  Created by Christos Baloukas on 14/4/21.
//

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include "../../definitions.h"
#include "../../config.h"

void print_map(std::string_view comment, const std::map<std::string, int>& m)
{
    std::cout << comment;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; ";
    }
    std::cout << "\n";
}

void print_map(std::string_view comment, const DDTLibrary::map<std::string, int>& m)
{
    std::cout << comment;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; ";
    }
    std::cout << "\n";
}

void print_map(std::string_view comment, const DDTLibrary::mapAVL<std::string, int>& m)
{
    std::cout << comment;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; ";
    }
    std::cout << "\n";
}
 

class A {
public:
    
    class B {
    public:
        int data[100];
        
        B() {
            std::cout << "B()" << std::endl;
            for (int i = 0; i < 100; i++) {
                data[i] = 2;
            }
        }
        ~B() {
            std::cout << "~B()" << std::endl;
        }
    };
    
    A() {
        std::cout << "A()" << std::endl;
        bPointer = new B;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
    }
    
    B* bPointer;
    
};

#define MAX_TREE_ELEMENTS 100             //   10.000
#define NUM_RANDOM_LOOPS 100             //  100.000

int main(int argc, const char * argv[]) {
    
    auto start = std::chrono::high_resolution_clock::now();
    auto currentTimeCheck = std::chrono::high_resolution_clock::now();
    auto prevTimeCheck = std::chrono::high_resolution_clock::now();
    auto durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - start);
    
    // insert code here...
    Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);
    Tracing::DDTRLogger::getInstance()->setSamplingPercentage(SAMPLING_PERCENTAGE);
    
    ddt_helper<std::pair<int, std::string>, 10>::BINARYTREE binaryTree;
    ddt_helper<std::pair<int, std::string>, 10>::REDBLACKTREE redBlackTree;
    typedef std::pair<int, std::string> pair1;
    
    std::vector< pair1 > v1;
    
    std::cout << "v1 fill with " << MAX_TREE_ELEMENTS << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MAX_TREE_ELEMENTS; i++) {
        std::stringstream ss1;
        ss1 << "test" << i;
        v1.push_back(pair1(i, ss1.str()));
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "v1 fill time taken = " << durationCheck.count() << std::endl;
    
    // ---------------------
    std::cout << "binary tree fill with " << MAX_TREE_ELEMENTS << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (auto it = v1.begin(); it != v1.end(); it++) {
        binaryTree.push_back(*it);
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "binary tree fill with - time taken = " << durationCheck.count() << std::endl;
    
    // ------------------
    std::cout << "red black tree fill with " << MAX_TREE_ELEMENTS << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    
    for (auto it = v1.begin(); it != v1.end(); it++) {
        redBlackTree.push_back(*it);
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "red black tree fill - time taken = " << durationCheck.count() << std::endl;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, MAX_TREE_ELEMENTS - 1);
    
    std::cout << "random_indexes" << std::endl;
    std::cout << "binary tree random index #" << NUM_RANDOM_LOOPS << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RANDOM_LOOPS; i++) {
        int random_index = distribution(generator);
        binaryTree[random_index];
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "binary tree random time taken = " << durationCheck.count() << std::endl;
    
    std::cout << "redblack tree random index #" << NUM_RANDOM_LOOPS << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RANDOM_LOOPS; i++) {
        int random_index = distribution(generator);
        redBlackTree[random_index];
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "redblack tree random time taken = " << durationCheck.count() << std::endl;
    
    /*
    for (auto i = binaryTree.begin(); i != binaryTree.end(); i++) {
        std::cout << "first = " << i->first << " second = " << i->second << std::endl;
    }*/
    //binaryTree.Print();
    
    //Testing map implementation
    
    std::map<std::string, int> m;
    DDTLibrary::map<std::string, int> m2;
    m["string1"] = 0;
    m["string2"] = 10;
    m["string3"] = 5;
    m["string4"] = 20;
    m["abc"] = 20;
    
    m2["string1"] = 0;
    m2["string2"] = 10;
    m2["string3"] = 5;
    m2["string4"] = 20;
    m2["abc"] = 20;
    
    DDTLibrary::mapAVL<std::string, int> m3;
    m3["string1"] = 0;
    m3["string2"] = 10;
    m3["string3"] = 5;
    
    print_map("map m: ", m);
    print_map("map m2: ", m2);
    print_map("map m3: ", m3);
    
    // ---------------------------------------
    // Testing more functions std::map
    
    std::map<std::string, int> mapOfWords;
    DDTLibrary::map<std::string, int> mapOfWords2;
    // Inserting data in std::map
    mapOfWords.insert(std::make_pair("earth", 1));
    mapOfWords.insert(std::make_pair("moon", 2));
    mapOfWords["sun"] = 3;
    // Will replace the value of already added key i.e. earth
    mapOfWords["earth"] = 4;
    // Iterate through all elements in std::map
    std::map<std::string, int>::iterator it = mapOfWords.begin();
    while(it != mapOfWords.end())
    {
        std::cout<<it->first<<" :: "<<it->second<<std::endl;
        it++;
    }
    // Check if insertion is successful or not
    if(mapOfWords.insert(std::make_pair("earth", 1)).second == false)
    {
        std::cout<<"Element with key 'earth' not inserted because already existed"<<std::endl;
    }
    // Searching element in std::map by key.
    if(mapOfWords.find("sun") != mapOfWords.end())
        std::cout<<"word 'sun' found"<<std::endl;
    if(mapOfWords.find("mars") == mapOfWords.end())
        std::cout<<"word 'mars' not found"<<std::endl;
    
    // ---------------------------------------
    // Testing more functions DDTRLibrary::map
    
    mapOfWords2.insert(std::make_pair("earth", 1));
    mapOfWords2.insert(std::make_pair("moon", 2));
    mapOfWords2["sun"] = 3;
    // Will replace the value of already added key i.e. earth
    mapOfWords2["earth"] = 4;
    // Iterate through all elements in std::map
    DDTLibrary::map<std::string, int>::iterator it2 = mapOfWords2.begin();
    while(it2 != mapOfWords2.end())
    {
        std::cout << it2->first <<" :: "<< it2->second << std::endl;
        it2++;
    }
    // Check if insertion is successful or not
    if(mapOfWords2.insert(std::make_pair("earth", 1)).second == false)
    {
        std::cout<<"Element with key 'earth' not inserted because already existed"<<std::endl;
    }
    // Searching element in std::map by key.
    //if(mapOfWords2.find("sun") != mapOfWords2.end())
      //  std::cout<<"word 'sun' found"<<std::endl;
    //if(mapOfWords2.find("mars") == mapOfWords2.end())
       // std::cout<<"word 'mars' not found"<<std::endl;
    
    
    //
    // Testing emplace_back
    /*
    std::vector<compositeObject> vTest1;
    
    vTest1.reserve(5);
    vTest1.push_back(compositeObject());
    vTest1.emplace_back();
    
    
    //ddt_helper<std::pair<unsigned int, double>, 10>::REDBLACKTREE redTree1;
    //redTree1[5] = 30.40;
    
    std::vector<compositeObject> vTest2;
    std::list<compositeObject> vTest3;
    int testSize = 1000000;
    
    std::cout << "pushing back " << testSize << " elements" << std::endl;
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testSize; i++) {
        vTest2.emplace_back();
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "push_back time = " << durationCheck.count() << std::endl;
    
    
    vTest2.clear();
    vTest2.shrink_to_fit();
    
    prevTimeCheck = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testSize; i++) {
        vTest3.emplace_back();
    }
    currentTimeCheck = std::chrono::high_resolution_clock::now();
    durationCheck = std::chrono::duration_cast<std::chrono::microseconds>(currentTimeCheck - prevTimeCheck);
    std::cout << "emplace_back time = " << durationCheck.count() << std::endl;
    
    
    //-------------------------
    // Testing error in emulator
    
    ddt_helper<int>::SLLROVINGLIST myList1;
    for (int i = 0; i < 10; i++) {
        myList1.push_back(i);
    }
    myList1.erase(myList1.begin() + 2);
    
    //-------------------------
    A* a = new A;
    
    A::B* bPointer = a->bPointer;
    
    delete a;
    
    std::cout << "bPointer->[data[2] = " << bPointer->data[2] << std::endl;
    
    delete a->bPointer;
    
    //---------------------
    */
    
    
    return 0;
}
