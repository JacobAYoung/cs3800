#include <iostream>
#include <thread>
#include <fstream>
#include "config.h"
#include <vector>
#include <streambuf>
#include <queue>
#include <mutex>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

mutex fileQueueLock;
mutex countQueueLock;

/* Reads the initial file to get the list of file names */
queue<string> openFile(string fileName)
{
    string line;
    queue<string> files;
    ifstream myfile(fileName);
    if (myfile)
    {
        while (!myfile.eof())
        {
            getline(myfile,line);
            files.push(line);
        }
        myfile.close();
    } else {
        cout << "File: " << fileName << " doesn't exist." << endl;
    }
    return files;
}

/* Takes in a file and reads the lines for that file */
string readFileContents(string fileName)
{
    string line;
    ifstream myfile (fileName);
    if (myfile)
    {
        myfile.seekg(0, std::ios::end);   
        line.reserve(myfile.tellg());
        myfile.seekg(0, std::ios::beg);

        line.assign((std::istreambuf_iterator<char>(myfile)),
                    std::istreambuf_iterator<char>());
        myfile.close();
        return line;
    } else {
        cout << "File: " << fileName << " doesn't exist." << endl;
        exit(1);
    }
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

    //Lowercase the lines of the file and the word to find
    std::transform(lines.begin(), lines.end(), lines.begin(), ::tolower);
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    //Get a count of the word occurence from the file
    int counter = countWordOccurence(lines, word);

    countQueueLock.lock();
    countQueue.push(counter);
    countQueueLock.unlock();
}

void reduce(queue<int>& countQueue)
{
    int firstNumber = 0;
    int secondNumber = 0;
    if (countQueue.size() >= 2)
    {
        countQueueLock.lock();
        firstNumber = countQueue.front();
        countQueue.pop();
        secondNumber = countQueue.front();
        countQueue.pop();
        countQueueLock.unlock();

        //Add the sum of the word count
        int sum = firstNumber + secondNumber;

        //Push the sum back onto the queue
        countQueueLock.lock();
        countQueue.push(sum);
        countQueueLock.unlock();
    }
}

int main() 
{
    Configuration config;
    //Queue of files and the word occurrence of the files
    queue<string> fileQueue;
    queue<int> countQueue;
    
    int fileQueueSize = 0;
    int countQueueSize = 0;

    //A vector of threads containing each of the mapper threads and reducer threads
    vector<thread> mapperThreads;
    vector<thread> reducerThreads;

    fileQueue = openFile(config.fileDataName);
    fileQueueSize = fileQueue.size();
    if (fileQueueSize > 0) {
        for(int i = 0; i < fileQueueSize; i++)
        {
            mapperThreads.push_back(thread(map, ref(fileQueue), ref(countQueue), config.wordToFind));
        }

        //Wait for the mapper threads to finish
        for(auto& map : mapperThreads)
            map.join();
        //Get the accurate size of the countQueue
        countQueueSize = countQueue.size();

        for(int i = 0; i < countQueueSize-1; i++)
        {
            reducerThreads.push_back(thread(reduce, ref(countQueue)));
        }
        //Wait for the reducer threads to finish
        for(auto& reduce : reducerThreads)
            reduce.join();

        cout << "The word '"<< config.wordToFind << "' was found: " << countQueue.front() << " times" << endl;
    } else {
        cout << "Not enough files." << endl;
    }
    

    return 0;
}