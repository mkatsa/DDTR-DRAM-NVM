#include "../ddtrlibrary/definitions.h"
//#include "myddtType.h"
#include "../ddtrlibrary/config.h"
#include <iostream>
#include <random>

using namespace std;

#define MAX_ELEMENTS 50 //50000
#define MAX_ELEMENTS_BIG 100 //1000000
#define NUM_LOOPS 100 //100000
#define NUM_LOOPS_BIG 100 //10000
#define NUM_ERASE_BIG 10 //100

//#define STD

int main()
{	
	Tracing::DDTRLogger::getInstance()->setLogFileName(LOGFILENAME);

#ifdef STD
	std::vector<compositeObject> myddt0;
  std::vector<compositeObject> myddt1;
  std::vector<compositeObject> myddt2;
  std::vector<compositeObject> myddt3;
#else
  myddtType0* myddt0 = new myddtType0;
  myddtType1* myddt1 = new myddtType1;
  myddtType2* myddt2 = new myddtType2;
  myddtType3* myddt3 = new myddtType3;
#endif

//===================Pushing Elements====================
  
  //cout << "push_back more to ddt0 and ddt2" << endl;
	for (int y = 0; y < MAX_ELEMENTS; y++)
	{
    
    myddt0->push_back(compositeObject());
		myddt2->push_back(compositeObject());
    
	}

	//cout << "push_back more to ddt0 and ddt2" << endl;

	for (int y = 0; y < MAX_ELEMENTS; y++)
	{
		myddt0->push_back(compositeObject());
		myddt2->push_back(compositeObject());
	}

  //myddt0.resize(16);
  //myddt2.resize(16);
	//cout << "push_back to ddt1" << endl;
	for (int y = 0; y < MAX_ELEMENTS_BIG/2; y++)
	{
		myddt1->push_back(compositeObject());
	}

  //cout << "push_back to ddt3" << endl;
	for (int y = 0; y < MAX_ELEMENTS_BIG; y++)
	{
		myddt3->push_back(compositeObject());
	}

//=======================================================


	//cout << "random access to ddt1" << endl;
	std::default_random_engine generator1;
	std::uniform_int_distribution<int> distribution1(0, NUM_LOOPS_BIG/2 - 1);
	int currentPercentage = 0;

	for (unsigned long long i = 0; i < NUM_LOOPS; i++)
  {
      
      int random_index = distribution1(generator1);  // generates number in the range 0..MAX_ELEMENTS-1

      (*myddt1)[random_index];
      
      //calculate loop completion percentage
      int percentage = (int) (100*(i+1)/NUM_LOOPS);
      if ( currentPercentage < percentage )
      {
        currentPercentage = percentage;
        //cout << currentPercentage << "% ";
      }
    
  }

  
  /*for (int y = 0; y < MAX_ELEMENTS_BIG/2; y++)
	{
		myddt1.pop_back();
	}*/
  //cout << endl;
  //cout << "erase from ddt3" << endl;
	currentPercentage = 0;
  
  srand (time(NULL));
  
  for (unsigned long long i = 0; i < NUM_ERASE_BIG; i++)
  {
    int random_index = rand() % myddt3->size();

    myddt3->erase(myddt3->begin() + random_index);
    //calculate loop completion percentage
    int percentage = (int) (100*(i+1)/NUM_ERASE_BIG);
    if ( currentPercentage < percentage )
    {
      currentPercentage = percentage;
      //cout << currentPercentage << "% ";
    }
  }
  
  //cout << endl;
  //cout << "clearing up" << endl;
  myddt1->clear();
  myddt3->clear();
  myddt0->clear();
  myddt2->clear();
  
  delete myddt0;
  delete myddt1;
  delete myddt2;
  delete myddt3;
}
