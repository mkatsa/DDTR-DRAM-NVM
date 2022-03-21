#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void ddtCombinations(vector<vector<string>>& vCombinations, vector<string> ddtTypes, vector<string> prefix, const unsigned long n, const int length)
{
    if (length == 1)
    {

        for (int j = 0; j < n; j++)
        {
            prefix.push_back(ddtTypes[j]);
            vCombinations.push_back(prefix); // the vector that we will use to store the permutations in
            prefix.pop_back();
        }

    }
    else
    {

        for (int i = 0; i < n; i++) {
            prefix.push_back(ddtTypes[i]);
            ddtCombinations(vCombinations, ddtTypes, prefix, n, length - 1);
            prefix.pop_back();
        }
    }
}

void printCombinations(vector<vector<string>>& vCombinations, const char* fileName) {
    std::ofstream file;
    
    file.open(fileName);
    
    unsigned long i = 0;
    for (vector<vector<string>>::iterator it1 = vCombinations.begin(); it1 != vCombinations.end(); it1++)
    {
        file << i++ << " -> ";
        for (vector<string>::iterator it2 = it1->begin(); it2 != it1->end(); it2++)
        {
            file << *it2 << " ";
        }
        file << endl;
    }
    file.close();
}

int main(int argc, char * argv[]) 
{
    //system("rm myddtType.h")
    vector<string> vTests;
    unsigned int numOfDDTs = 0;
    
    if (argc == 1)
    {
        vTests.push_back("random_access");
        numOfDDTs = 1;
    }
    else
    {
        numOfDDTs = atoi(argv[1]);
        for (int i = 2; i < argc; i++)
        {
            vTests.push_back(argv[i]);
        }
    }
    
    int length = numOfDDTs;

    vector<string> vDDTTypes = {"VECTOR", "SLLLIST", "DLLLIST", "SLLROVINGLIST", "DLLROVINGLIST"};
    vector<vector<string>> vCombinations;
    vector<string> vPrefix;
    
    ddtCombinations(vCombinations, vDDTTypes, vPrefix, vDDTTypes.size(), length);
    printCombinations(vCombinations, "log_combinations.out");
    for (int i = 0; i < vTests.size(); i++)
    {
        string test = vTests[i];
        
        for (int y = 0; y < vCombinations.size(); y++)
        {
            ofstream myfile;
            myfile.open ("config.h");
    
            if (myfile.is_open())
            {
                myfile << "#ifndef CONFIG_H" << endl << "#define CONFIG_H" << endl << endl << "#include \"definitions.h\"" << endl;
                myfile << "class compositeObject;" << endl << endl;
                myfile << "#define LOGFILENAME " << "\"log_" << y << ".out\"" << endl;
                
                for (int z = 0; z < numOfDDTs; z++) {
                    string type = vCombinations[y][z];
                    myfile << "typedef ddt_helper<compositeObject, " << z << ">::" << type << " myddtType" << z << ";" << endl;
                }
                myfile << "#endif";
                myfile.close();
            }
            else {
                cout << "Unable to open myddtType.h file" << endl;
            }
            
            system ("make clean");
            string testCommand = "make " + test;
            system (testCommand.c_str());
            string testRunCommand = "./" + test + "_main";
            system (testRunCommand.c_str());
    
            /*
            string logName = "log_" + test + "_" + type + ".out";
            string renameCommand = "mv log.out " + logName;
            system (renameCommand.c_str());
             */
                
            
        }
        
        
    }
    
    /*
    
    
    
	
	
	*/
	return 0;
}



/*
change definitions file
make
rename log.out
clean
repeat
*/
