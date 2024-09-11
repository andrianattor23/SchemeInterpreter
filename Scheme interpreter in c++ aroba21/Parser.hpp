
#ifndef Parser_hpp
#define Parser_hpp

#include "ReadFile.hpp"
#include "SchemeList.hpp"
#include <stdlib.h>
#include <deque>
#include <string>

class Parser {
public:
    static SchemeList parse (const std::string rawInput) ;
    virtual ~Parser();
private:
    Parser();
    static std::deque<std::string> tokenize(const std::string rawInput);
    static SchemeList processSyntax (std::deque<std::string>& tokens);
    static SchemeList atomic (std::string s);
    static bool isNumber (std::string s);
    static bool isInteger (std::string s);
};

#endif 
