#include "Parser.hpp"

SchemeList Parser::parse(const std::string rawInput) {
    auto temp = tokenize(rawInput);
    return processSyntax(temp);
}

std::deque<std::string> Parser::tokenize(const std::string rawInput) {
    std::deque<std::string> tokens;
    for (int i = 0; i < rawInput.length(); ++i) {
        while (rawInput[i] == ' ') ++i;
        if (rawInput[i] == '(') tokens.push_back("(");
        else if (rawInput[i] == ')') tokens.push_back(")");
        else {
            int j = i;
            while (rawInput[j] && rawInput[j] != ')' && rawInput[j] != '(' && rawInput[j] != ' ')
                ++j;
            tokens.push_back(rawInput.substr(i, j - i));
            i = j - 1;
        }
    }
    return tokens;
}

SchemeList Parser::processSyntax(std::deque<std::string>& tokens) {
    if (tokens.empty()) throw "Syntax Error: Unexpected End of Line";
    std::string token = tokens.front();
    tokens.pop_front();
    if (token == "'") {
        int countP = 0;
        for (auto vi = tokens.begin(); vi != tokens.end(); vi++) {
            if (*vi == "(") {
                countP++;
            }
            if (*vi == ")") {
                countP--;
            }
            if (countP == 0) {
                tokens.insert(vi, ")");
                break;
            }
        }
        tokens.push_front("quote");
        tokens.push_front("(");
        return processSyntax(tokens);
    } else if (token == "(") {
        SLists list;
        while (tokens.front() != ")") {
            list.push_back(processSyntax(tokens));
        }
        tokens.pop_front();
        return SchemeList(list);
    } else if (token == ")") {
        throw "Syntax Error: Unexpected Syntax ')'";
    } else {
        return atomic(token);
    }
}

SchemeList Parser::atomic(std::string s) {
    if (isNumber(s)) {
        return SchemeList(atof(s.c_str()));
    }
    return SchemeList(s);
}

bool Parser::isNumber(std::string s) {
    return s.find_first_not_of("0123456789.") == std::string::npos;
}

bool Parser::isInteger(std::string s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char *p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

/*SchemeList Parser::parse(const std::string rawInput) {
    std::deque<std::string> temp = tokenize(rawInput);
    return process_syntax(temp);
}


std::deque<std::string> Parser::tokenize(const std::string rawInput) {
    std::deque<std::string> tokens;
    for (int i = 0; i < rawInput.length(); ++i) {
        while (rawInput[i] == ' ') ++i;
        if (rawInput[i] == '(') tokens.push_back("(");
        else if (rawInput[i] == ')') tokens.push_back(")");
        else {
            int j = i;
            while (rawInput[j] && rawInput[j] != ')' && rawInput[j] != '(' && rawInput[j] != ' ')
                ++j;
            tokens.push_back(rawInput.substr(i,j-i));
            i = j-1;
        }
    }
   
    return tokens;
}


SchemeList Parser::process_syntax (std::deque<std::string>& tokens) {
    if (tokens.size()==0) throw "Syntax Error: Unexpected End of Line";
    std::string token = tokens[0];
    tokens.pop_front();
    if (token == "'") {
        int countP = 0;
        for (auto vi = tokens.begin(); vi != tokens.end(); vi++) {
            if(*vi=="("){
                countP++;
            }
            if (*vi==")") {
                countP--;    
            }
            if(countP==0){
                tokens.insert(vi,")");
                break;
            }
        }
        tokens.push_front("quote");
        tokens.push_front("(");
        return process_syntax (tokens);
    } else if (token == "(") { //expression
        SLists list;
        while (tokens[0]!=")") {
            list.push_back(process_syntax(tokens));
        }
        tokens.pop_front();
        return SchemeList(list);
    } else if (token == ")") {
        throw "Syntax Error: Unexpected Syntax ')'";
    } else {    //atom
        return atomic(token);
    }
}

SchemeList Parser::atomic (std::string s) {
    if (isNumber(s)) {
        return SchemeList(atof(s.c_str()));  //double/float
    }
    return SchemeList(s);    //symbol
}

bool Parser::isNumber (std::string s) {
    return s.find_first_not_of( "0123456789." ) == std::string::npos;
}

bool Parser::isInteger (std::string s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    char * p ;
    strtol(s.c_str(), &p, 10) ;
    return (*p == 0) ;
}*/

