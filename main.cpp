#include <iostream>
#include <thread>
#include <fstream>
#include "config.h"
#include <vector>
#include <streambuf>
#include "mapper.h"
using namespace std;

int main() 
{
    vector<string> lines;
    string line;
    Configuration config;
    Mapper mapper;
    lines = config.openFile(config.fileDataName);
    line = config.readFileContents(config.fileDataName);
    cout << line << endl;
    int counter = mapper.countWordOccurence(line, "the");
    cout << counter << endl;
    //thread first (foo);
    return 0;
}