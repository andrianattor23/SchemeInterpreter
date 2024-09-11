#include "SchemeList.hpp"
#include <sstream>
#include "Environment.hpp"

using namespace std;

SchemeList::SchemeList(){};
SchemeList::SchemeList(sType type): type(type) {};
SchemeList::SchemeList(std::string s) : value(s), type (SYMBOL) {}
SchemeList::SchemeList(double s) : value(std::to_string(s)), type(NUMBER) {}
SchemeList::SchemeList(SLists s) : list(s), type(LIST) {}
SchemeList::SchemeList(proc s) : p(s) , type(PROC) {}

SchemeList::SchemeList(proc s,Environment* env) : p(s) , type(PROC) {}

void SchemeList::setList(const std::vector<SchemeList>& elements) {
    list = elements;
    type = LIST;  // Assuming that a list is being set
}


bool double_is_int(std::string dVal) {
    double trouble = atof(dVal.c_str());
    return fabs(trouble) == floor(fabs(trouble));
} 

std::string double_to_int(std::string dVal) {
    return std::to_string((int)atof(dVal.c_str()));
}

void SchemeList::setType (sType t) {
    type = t;
}

void SchemeList::push(SchemeList s) {
    list.push_back(s);
}

SchemeList:: SchemeList( std::vector<SchemeList>:: iterator start, int size){
    int count=0;
    while(count!=size){
        list.push_back(*start);
        count++;
        start++;
    }
}


void SchemeList::pushList(SLists s) {
    for (auto vi = s.begin(); vi != s.end(); vi++)
        list.push_back(*vi);
}

SchemeList::sType SchemeList::getType() const {
    return type;
}

std::string SchemeList::getTypeString() const {
    if (type == SYMBOL) return "SYMBOL";
    else if (type == NUMBER) return "NUMBER";
    else if (type == LIST) return "LIST";
    else if (type == PROC) return "PROC";
    else return "LAMBDA";
}

SLists SchemeList::getList() const {
    return list;
}

SchemeList::proc SchemeList::getProc() const {
    return p;
}

std::string SchemeList::val() const {return value;}

std::string SchemeList::listToString() {
    if (type == SYMBOL) return value;
    if (type == NUMBER) return double_is_int(value) ? double_to_int(value) : value;
    std::string strVal = "";
    for (auto vi = list.begin(); vi != list.end(); vi++) {
        strVal += vi->listToString();
        if (vi!=list.end()-1)
            strVal += " ";
    }
    return strVal;
}


//for debugging
std::string SchemeList::getPrintString() const {
    std::string s = "";
    if (type == NUMBER) {
        s+=(double_is_int(value) ? double_to_int(value) : value);
    } else if (type == SYMBOL) {
        if (value == "#t")
            return "true";
        else if (value == "#f")
            return "false";
        else
            return value;
    } else if (type == PROC) {
        return "PROC";
    } else {
        s+="(";
        for (auto vi = list.cbegin(); vi != list.cend(); vi++) {
            s+=vi->getPrintString();
            if (vi!=list.cend()-1) s+= " ";
        }
        if (s.back() == ' ') s.pop_back();
        s+=") ";
    }
    return s;
}

