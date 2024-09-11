
#ifndef SchemeList_hpp 
#define SchemeList_hpp

//#include "Environment.hpp"
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include<iterator> 
#include <iostream>

class Environment;

class SchemeList {
public:
    typedef SchemeList (*proc)(const std::vector<SchemeList>&,Environment* env);
    enum sType {SYMBOL, NUMBER, LIST, PROC, LAMBDA};
    
    void push(SchemeList s);
    std::string getPrintString() const;
    size_t size() const;
    sType getType() const;
    void setType(sType t);
    std::string val() const;
    std::vector<SchemeList> getList () const;
    std::string listToString();
    proc getProc() const;
    void pushList(std::vector<SchemeList> s);
    std::string getTypeString() const;
    void setList(const std::vector<SchemeList>& elements);

    SchemeList(proc s,Environment* env);

    SchemeList();
    SchemeList(sType type);
    SchemeList(std::string s);
    SchemeList(double s);
    SchemeList(std::vector<SchemeList> s);
    SchemeList(proc s);
    SchemeList(std::vector<SchemeList>:: iterator start,int size);
    
private:
    std::string value;
    std::vector<SchemeList> list;
    proc p;
    
    sType type;
    
    
};

typedef std::vector<SchemeList> SLists;

#endif 
