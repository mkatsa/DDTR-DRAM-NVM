#include "../../definitions.h"
#include "../../config.h"
#include <iostream>
#include <random>

unsigned long long Tracing::DMMLogger::numReads;
unsigned long long Tracing::DMMLogger::numWrites;
unsigned long long Tracing::DMMLogger::maxMemory;
unsigned long long Tracing::DMMLogger::currMemory;
std::map<const void*, size_t> Tracing::DMMLogger::allocationMap;
std::map<const void*, size_t>::iterator Tracing::DMMLogger::m_Iter;

typedef int myddtEncType;


#define MAX_ELEMENTS 1000
#define NUM_LOOPS 1000

void printDDT(ddt_helper<int, 100>::SLLLIST v) {
    std::cout << "printing vector" << std::endl;
    for (auto i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
void printDDT(ddt_helper<int, 101>::SLLLIST sll) {
    std::cout << "printing list" << std::endl;
    for (auto i : sll) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


int main()
{
	//Tracing::DMMLogger::getInstance()->setLogFile("log.bin");
	//Tracing::DMMLogger::getInstance()->setAlternateFile("log.map");
    Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);
	ddt_helper<compositeObject, 0>::SLLLIST myddt0;
    ddt_helper<compositeObject , 1>::SLLLIST myddt1;
	
	std::cout << "pushing back MAX_ELEMENTS elements" << std::endl;
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        myddt0.push_back(*new compositeObject);
        myddt1.push_back(*new compositeObject);
    }
    
    std::cout << "accessing NUM_LOOPS elements randomly" << std::endl;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, MAX_ELEMENTS - 1);
    int currentPercentage = 0;
    
    for (unsigned long long i = 0; i < NUM_LOOPS; i++)
    {
        
        int random_index = distribution(generator);  // generates number in the range 0..MAX_ELEMENTS-1

        myddt0[random_index];
        myddt1[random_index];
        
        //calculate loop completion percentage
        int percentage = (int) (100*(i+1)/NUM_LOOPS);
        if ( currentPercentage < percentage )
        {
            currentPercentage = percentage;
            std::cout << ".." << currentPercentage << "%" << std::flush;
        }
    }
    
    //ddt_helper<compositeObject, 0>::SLLLIST::smartElementPointer ptr = &myddt0[0];
    //*ptr;
    //++ptr;
    std::cout << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    
    ddt_helper<int, 100>::SLLLIST vTest1;
    ddt_helper<int, 101>::SLLLIST sllTest1;
    
    //add 0 - 10
    for (int i = 0; i < 10; i++) {
        vTest1.push_back(i);
        sllTest1.push_back(i);
    }
    
    printDDT(vTest1);
    printDDT(sllTest1);
    
    ddt_helper<int, 100>::SLLLIST::iterator vIt1,vIt2;
    vIt1 = vTest1.begin() + 0;
    vIt2 = vTest1.begin() + 8;
    vTest1.erase(vIt1, vIt2);
    
    printDDT(vTest1);
    
    ddt_helper<int, 101>::SLLLIST::iterator sllIt1,sllIt2;
    sllIt1 = sllTest1.begin() + 0;
    sllIt2 = sllTest1.begin() + 9;
    sllTest1.erase(sllIt1, sllIt2);
    
    printDDT(sllTest1);
    
#define TEST_ELEMENTS 1000000 // 1.000.000
    
    std::default_random_engine generator2;
    std::uniform_int_distribution<int> distribution2(0, TEST_ELEMENTS - 1);
    //for (int j = 0; j < 100; j++) {
        ddt_helper<int, 101>::SLLLIST sllTest2;
        for (int i = 0; i < TEST_ELEMENTS; i++) {
            int random_index = distribution2(generator2);
            sllTest2.push_back(random_index);
        }
    std::cout << "printing list sllTest2 before sorting" << std::endl;
        //printDDT(sllTest2);
        
        //std::sort(sllTest2.begin()+1, sllTest2.end()-1);
    
    std::cout << "printing list sllTest2 after sorting" << std::endl;
        //printDDT(sllTest2);
    //}
    
    
    int array[] = {102, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102};
    
    ddt_helper<int, 101>::SLLLIST sllTest3;
    for (int i = 0; i < 103; i++) {
        sllTest3.push_back(array[i]);
    }
    printDDT(sllTest3);
    //std::sort(sllTest3.begin()+1, sllTest3.end()-1);
    printDDT(sllTest3);
    
    
    std::cout << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    ddt_helper<int, 101>::SLLLIST sllTest4;
    for (int i = 0; i < 1000; i++) {
        int random_index = distribution(generator);
        sllTest4.push_back(random_index);
    }
    printDDT(sllTest4);
    /*
    for (int i = 0; i < 500; i++) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, sllTest4.size());
        int random_index = distribution(generator);
        sllTest4.erase(sllTest4.begin()+random_index);
    }
     */
    sllTest4.erase(sllTest4.begin()+ 100, sllTest4.begin()+900);
    printDDT(sllTest4);
	
    ddt_helper<int, 101>::SLLLIST sllTest5;
    for (int i = 0; i < 5; i++) {
        sllTest5.push_back(i);
    }
    printDDT(sllTest5);
    sllTest5.begin() + 6;
    
	return 0;
}
