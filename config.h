#ifndef CONFIG_H
#define CONFIG_H
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Configuration
{
    public:
    string fileDataName = "files.dat";
    vector<string> openFile(string fileName)
    {
        string line;
        vector<string> lines;
        try
        {
            ifstream myfile (fileName);
            myfile.exceptions ( );
            while (!myfile.eof())
            {
                getline(myfile,line);
                lines.push_back(line);
            }
            myfile.close();
            return lines;
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

};


#endif