#ifndef MAPPER_H
#define MAPPER_H
#include "config.h"
#include <iostream>
using namespace std;

class Mapper
{
    public:
    void countWordOccurence(string file, string word)
    {
        int count = 0;
        int pos = 0;
        Configuration config;
        while(true) { 
            pos =  file.find(word, ++pos);
            if (pos != std::string::npos) {
                count++;
            } else break;
        }
        cout << count << endl;
        //return count;
    }



};


#endif