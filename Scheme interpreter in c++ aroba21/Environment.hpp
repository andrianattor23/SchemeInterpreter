#ifndef Environment_hpp
#define Environment_hpp
#include <unordered_map>
#include "SchemeList.hpp"

class SchemeList;

typedef std::string symbol;
typedef std::unordered_map<symbol, SchemeList> envType;




class Environment {
public:
    Environment ();
    Environment (SLists params, SLists args, Environment* outerEnv);
    void insert(symbol,SchemeList);
    envType* find (symbol);
    envType env;
    
private:
    
    Environment* outerEnv;
};
#endif 



