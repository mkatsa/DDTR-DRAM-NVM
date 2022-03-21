#include "../../definitions.h"
#include "../../myddtType.h"
#include <iostream>
using namespace std;

#define MAX_ELEMENTS 1000
#define NUM_LOOPS 10

unsigned long long Tracing::DMMLogger::numReads;
unsigned long long Tracing::DMMLogger::numWrites;
unsigned long long Tracing::DMMLogger::maxMemory;
unsigned long long Tracing::DMMLogger::currMemory;
std::map<const void*, size_t> Tracing::DMMLogger::allocationMap;
std::map<const void*, size_t>::iterator Tracing::DMMLogger::m_Iter;

typedef int myddtEncType;

//typedef ddt_helper<compositeObject, 0>::vectorType myddtType;

int main()
{
	//Tracing::DMMLogger::getInstance()->setLogFile("log.bin");
	//Tracing::DMMLogger::getInstance()->setAlternateFile("log.map");
    Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);
    
	myddtType0 myddt0;
    myddtType1 myddt1;
    myddtType2 myddt2;
    myddtType3 myddt3;
    myddtType4 myddt4;
	
	cout << "Testing FIFO access pattern with push_back and pop_back" << endl;
	//Fill up to the maximum number of elements and then pop them all back
	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
		myddt0.push_back(*new compositeObject);
        myddt1.push_back(*new compositeObject);
        myddt2.push_back(*new compositeObject);
        myddt3.push_back(*new compositeObject);
        myddt4.push_back(*new compositeObject);
	}

	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
        myddt0.pop_back();
		myddt1.pop_back();
        myddt2.pop_back();
        myddt3.pop_back();
        myddt4.pop_back();
	}
	
	// Fill up to a random number (but smaller than MAX_ELEMENTS and then pop them back. Repeat NUM_LOOPS times)
	for (int i = 0; i < NUM_LOOPS; i++)
	{
		int maxElements = rand() % MAX_ELEMENTS;
		for (int y = 0; y < maxElements; y++)
		{
            myddt0.push_back(*new compositeObject);
			myddt1.push_back(*new compositeObject);
            myddt2.push_back(*new compositeObject);
            myddt3.push_back(*new compositeObject);
            myddt4.push_back(*new compositeObject);
		}
	
		for (int y = 0; y < maxElements; y++)
		{
            myddt0.pop_back();
			myddt1.pop_back();
            myddt2.pop_back();
            myddt3.pop_back();
            myddt4.pop_back();
		}
	}
	
	
	
	
	return 0;
	
}
