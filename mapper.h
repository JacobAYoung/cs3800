#ifndef MAPPER_H
#define MAPPER_H

#include <iostream>

using namespace std;

class Mapper
{
    public:
    int countWordOccurence(string file, string word)
    {
        int count = 0;
        int pos = 0;
        while(true) { 
            pos =  file.find(word, ++pos);
            if (pos != std::string::npos) {
                count++;
            } else break;
        }
        return count;
    }



};


#endif