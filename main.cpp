#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cassert>
#include "lexical1.h"
#include "syntaxanalyzer_v3.h"
using namespace std;


//Function will get the filename
string getFileName()
{
  string input;

  cout << "Please Enter input file name: ";
  cin >> input;

  return input;
}

int main(int argc, const char * argv[]) {

    string 		fileName;
    ifstream 	inputFile;
    //Call syntaxanalyzer
    SyntaxAnalyzer SA;

    //Getting filename from function below.
    fileName = getFileName();

    //Open input file and start processing
    inputFile.open(fileName);
    if(inputFile)
    {
    	SA.run(fileName);
    }
    else
    {
    	cout << "\n\tError >> Please enter valid input file name\n\n";
    }

    inputFile.close();

    cout << "\nThank you for Using this program.\n";


    return 0;
}
