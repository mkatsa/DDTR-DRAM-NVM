#include "definitions.h"
#include "myddtType.h"
#include <iostream>
using namespace std;

class compositeObject {
public:
	compositeObject(int xValue, int yValue)
	{
		x = xValue;
		y = yValue;
	}
	
private:
	int x;
	int y;
};

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
	Tracing::DMMLogger::getInstance()->setLogFile("log.bin");
	Tracing::DMMLogger::getInstance()->setAlternateFile("log.map");
	
	myddtType myddt;
	
	cout << "Testing LIFO access pattern with push_back and pop_back" << endl;
	//Fill up to the maximum number of elements and then pop them all back
	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
		myddt.push_back(i);
	}

	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
		myddt.pop_front();
	}
	
	// Fill up to a random number (but smaller than MAX_ELEMENTS and then pop them back. Repeat NUM_LOOPS times)
	for (int i = 0; i < NUM_LOOPS; i++)
	{
		int maxElements = rand() % MAX_ELEMENTS;
		for (int y = 0; y < maxElements; y++)
		{
			myddt.push_back(y);
		}
	
		for (int y = 0; y < maxElements; y++)
		{
			myddt.pop_front();
		}
	}
	
	
	
	
	return 0;
	
}
