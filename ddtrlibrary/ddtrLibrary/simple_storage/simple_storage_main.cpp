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

#define MAX_ELEMENTS 10
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
	
	cout << "Testing FIFO access pattern with push_back and pop_back" << endl;
	//Fill up to the maximum number of elements and then pop them all back
	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
		myddt.push_back(i);
	}
	
    //myddtEncType* element = myddt.getElement(3);
	
	int newElement = 5000;
	//myddt.insert_after(newElement, element);
	//myddt.insert_after(newElement, myddt.getElement(0));
	//myddt.insert_after(newElement, myddt.getElement(9));
	
	for (int i = 0; i < 10; i++)
	{
		cout << "printing array" << endl;
		cout << i << " = " << myddt[i] << endl;;
	}
	cout << "-------------------------------------------" << endl;
	myddt.erase(myddt.getElement(1));
	myddt.erase(myddt.getElement(4));
	myddt.erase(myddt.getElement(6));
	for (int i = 0; i < myddt.size(); i++)
	{
		cout << "printing array" << endl;
		cout << i << " = " << myddt[i] << endl;;
	}
	
	return 0;
}
