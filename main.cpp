#include <iostream>
#include <thread>
#include <fstream>
#include "config.h"
#include <vector>
#include <streambuf>
#include <queue>
#include "mapper.h"
#include <mutex>
using namespace std;

mutex fileQueueLock;
mutex countQueueLock;

queue<string> openFile(string fileName)
{
    string line;
    queue<string> files;
    try
    {
        ifstream myfile (fileName);
        myfile.exceptions ( );
        while (!myfile.eof())
        {
            getline(myfile,line);
            files.push(line);
        }
        myfile.close();
        return files;
    }
    catch(exception const& e)
    {
        cout << "Could not open file" << endl;
    }
}
string readFileContents(string fileName)
{
    //char line[100];
    string line;
    try
    {
        ifstream myfile (fileName);
        myfile.exceptions ( );
        myfile.seekg(0, std::ios::end);   
        line.reserve(myfile.tellg());
        myfile.seekg(0, std::ios::beg);

        line.assign((std::istreambuf_iterator<char>(myfile)),
                    std::istreambuf_iterator<char>());
        myfile.close();
        return line;
    }
    catch(exception const& e)
    {
        cout << "Could not open file" << endl;
    }
}

string toLower2(string strr)
{	
    char str[100];
    string ret;
    strcpy(str,strr.c_str());
    int differ = 'A'-'a';
    char ch;
    int ii = strlen(str);
    for (int i=0; i <ii;i++)                                                           
    {
        strncpy(&ch,str+i,1);
        if (ch>='A' && ch<='Z')
        {
            ch = ch-differ;
            memcpy(str+i,&ch,1);
        }
    }
    ret = str;
    return ret;
}

int countWordOccurence(string file, string word)
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
    return count;
}

void map(queue<string>& fileQueue, queue<int>& countQueue, string word)
{
    string lines;
    string topFile;
    //Lock the fileQueueLock
    fileQueueLock.lock();
    //Get the top file from the queue
    topFile = fileQueue.front();
    //Pop it off the queue
    fileQueue.pop();
    //Unlock the fileQueueLock
    fileQueueLock.unlock();


    //Read the file contents
    lines = readFileContents(topFile);
    //Get a count of the word occurence from the file
    int counter = countWordOccurence(lines, toLower2(word));

    countQueueLock.lock();
    countQueue.push(counter);
    countQueueLock.unlock();

}

void reduce(queue<int>& countQueue)
{
    int first = 0;
    int second = 0;
    if (countQueue.size() >= 2)
    {
        countQueueLock.lock();
        first = countQueue.front();
        countQueue.pop();
        second = countQueue.front();
        countQueue.pop();
        countQueueLock.unlock();

        int sum = first + second;

        countQueueLock.lock();
        countQueue.push(sum);
        countQueueLock.unlock();
    }
    
}


int main() 
{
    Configuration config;

    queue<string> fileQueue;
    queue<int> countQueue;
    
    int fileQueueSize = 0;
    int countQueueSize = 0;
    vector<thread> mapperThreads;
    vector<thread> reducerThreads;
    fileQueue = openFile(config.fileDataName);
    fileQueueSize = fileQueue.size();
    for(int i = 0; i < fileQueueSize; i++)
    {
        mapperThreads.push_back(thread(map, ref(fileQueue), ref(countQueue), config.wordToFind));
    }

    for(auto& map : mapperThreads)
        map.join();

    countQueueSize = countQueue.size();

    for(int i = 0; i < countQueueSize-1; i++)
    {
        reducerThreads.push_back(thread(reduce, ref(countQueue)));
    }

    for(auto& reduce : reducerThreads)
        reduce.join();

    cout << "The word '"<< config.wordToFind << "' was found: " << countQueue.front() << " times" << endl;

    return 0;
}