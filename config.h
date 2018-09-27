#ifndef CONFIG_H
#define CONFIG_H
#include <fstream>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

class Configuration
{
    public:
    string fileDataName = "files.dat";
    string wordToFind = "The";
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

    string toLower(string strr)
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

};


#endif