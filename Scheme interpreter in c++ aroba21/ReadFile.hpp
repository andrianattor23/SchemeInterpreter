#ifndef ReadFile_hpp
#define ReadFile_hpp

#include <iostream>
#include <string>
#include <fstream>

class ReadFile {
public:
    static std::string readLine();
    static std::string readFile(std::string fileName);
    virtual ~ReadFile();
private:
    ReadFile();
};

#endif 
