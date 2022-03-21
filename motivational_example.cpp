#include "definitions.h"
//#include "myddtType.h"
#include "config.h"
#include <iostream>
#include <random>

using namespace std;

#define MAX_ELEMENTS 50000
#define MAX_ELEMENTS_BIG 1000000
#define NUM_LOOPS 100000
#define NUM_LOOPS_BIG 10000

unsigned long long Tracing::DMMLogger::numReads;
unsigned long long Tracing::DMMLogger::numWrites;
unsigned long long Tracing::DMMLogger::maxMemory;
unsigned long long Tracing::DMMLogger::currMemory;
std::map<const void*, size_t> Tracing::DMMLogger::allocationMap;
std::map<const void*, size_t>::iterator Tracing::DMMLogger::m_Iter;

typedef int myddtEncType;

int main()
{	
	Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);

	myddtType0 myddt0;
    myddtType1 myddt1;
    myddtType2 myddt2;
    myddtType3 myddt3;

//===================Pushing Elements====================
    //cout << "0" << endl;
	//cout << "Random access pattern on DDT1, DDT3" << endl;
	//cout << "LIFO access pattern on DDT2" << endl;
	for (int y = 0; y < MAX_ELEMENTS; y++)
	{
		myddt0.push_back(*new compositeObject);
		myddt2.push_back(*new compositeObject);
	}

	//cout << "1" << endl;

	for (int y = 0; y < MAX_ELEMENTS; y++)
	{
		myddt0.push_back(*new compositeObject);
		myddt2.push_back(*new compositeObject);
	}


	//cout << "2" << endl;
	for (int y = 0; y < MAX_ELEMENTS_BIG/2; y++)
	{
		myddt1.push_back(*new compositeObject);
	}


	for (int y = 0; y < MAX_ELEMENTS_BIG; y++)
	{
		myddt3.push_back(*new compositeObject);		
	}

//=======================================================


	//cout << "3" << endl;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, NUM_LOOPS_BIG/2 - 1);
	int currentPercentage = 0;

	for (unsigned long long i = 0; i < NUM_LOOPS; i++)
    {
        
        int random_index = distribution(generator);  // generates number in the range 0..MAX_ELEMENTS-1

        myddt1[random_index];
        
        //calculate loop completion percentage
        int percentage = (int) (100*(i+1)/NUM_LOOPS);
        if ( currentPercentage < percentage )
        {
            currentPercentage = percentage;
        }
    }


    /*for (int y = 0; y < MAX_ELEMENTS_BIG/2; y++)
	{
		myddt1.pop_back();
	}*/


	currentPercentage = 0;

	for (unsigned long long i = 0; i < NUM_LOOPS_BIG; i++)
    {
        
        int random_index = distribution(generator);  // generates number in the range 0..MAX_ELEMENTS-1

        myddt3[random_index];
        
        //calculate loop completion percentage
        int percentage = (int) (100*(i+1)/NUM_LOOPS_BIG);
        if ( currentPercentage < percentage )
        {
            currentPercentage = percentage;
        }
    }

    //cout << "5" << endl;

}