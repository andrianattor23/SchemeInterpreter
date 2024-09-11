#include "ReadFile.hpp"
#include "SchemeList.hpp"
#include "Parser.hpp"
#include "NativeProcedures.hpp"
#include "Environment.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

SLists getArgs (SchemeList l) {
    SLists unproc_args = l.getList();
    SLists args;
    for (int i = 1; i < unproc_args.size(); i++) {
        
        args.push_back(unproc_args[i]);
    }
    return args;
}


SchemeList evaluate (SchemeList s, Environment* env) {
   
    if (s.getType() == SchemeList::SYMBOL) {             //variable reference
        if (s.val()[0] == '\''){
            return SchemeList(s.val().substr(1,s.val().length()-1));
        }    
        return (*(env->find(s.val())))[s.val()];
    } else if (s.getType() == SchemeList::NUMBER) {      //constant literal
        return s;
    } else if (s.getList().size() == 0) {
        return s;
    } else if (s.getList()[0].val() == "define") {
        if (s.getList()[1].getType() == SchemeList::SYMBOL) {
            // Handle (define var expr) form
            env->env[s.getList()[1].val()] = evaluate(s.getList()[2], env);
            return SchemeList();
        } else if (s.getList()[1].getType() == SchemeList::LIST) {
            // Handle (define (func args...) body) form
            SchemeList funcName = s.getList()[1].getList()[0]; 
            SchemeList sizeSchm = s.getList()[1].getList();
            SchemeList funcArgs = SchemeList(s.getList()[1].getList().begin() + 1, sizeSchm.getList().size()-1);
            SchemeList funcBody = SchemeList(s.getList().begin() + 2,1);    

            // Create a lambda expression and store it in the environment
            SchemeList lambdaExpr({ funcBody,funcArgs});
            lambdaExpr.setList({funcBody,funcArgs });
            lambdaExpr.setType(SchemeList::LAMBDA);
            
            env->env[funcName.val()] = lambdaExpr;

            return SchemeList();
        }
    }else if (s.getList()[0].val() == "lambda") {
        s.setType(SchemeList::LAMBDA);
        return s;
    } else if (s.getList()[0].val() == "quote") {
        return s.getList()[1];
    } else if (s.getList()[0].val() == "set!") {
        (*(env->find(s.getList()[1].val())))[s.getList()[1].val()] = evaluate(s.getList()[2],env);
        return SchemeList();
    } else if (s.getList()[0].val() == "if") {
        return evaluate(s.getList()[1],env).val()=="#t" ? evaluate(s.getList()[2],env) : (s.getList()[3].val() == "else" ? evaluate(s.getList()[4],env) : SchemeList());
    } else if (s.getList()[0].val() == "begin") {
        for (int i = 1; i < s.getList().size()-1; i++) evaluate(s.getList()[i], env);
        return evaluate(s.getList()[s.getList().size()-1],env);
    }else if(s.getList()[0].val() == "and"){
        for (int vi = 1; vi != s.getList().size(); vi++){
            if(evaluate(s.getList()[vi],env).val()=="#f"){
                return SchemeList("#f");
            } 
        }
        return SchemeList("#t");        
    }else if(s.getList()[0].val() == "or"){
        for (int vi = 1; vi != s.getList().size(); vi++){
            if(evaluate(s.getList()[vi],env).val()=="#t"){
                return SchemeList("#t");
            } 
        }
        return SchemeList("#f");
    }else if(s.getList()[0].val() == "eval"){
        if(s.getList()[1].getList()[0].getPrintString()=="quote"){
        
            return evaluate(s.getList()[1].getList()[1],env);
        }
        return evaluate(s.getList()[1],env);
    } else {            //procedure call
        SchemeList p = evaluate(s.getList()[0],env);
        SLists args = getArgs(s);
        for (int i = 0; i < args.size(); i++){   
            args[i] = evaluate (args[i],env);
        } 
        if (p.getType() == SchemeList::LAMBDA) {
            if(p.getList()[0].getPrintString()=="lambda"){
                Environment* newEnv = new Environment(p.getList()[1].getList(), args, env);
                return evaluate(p.getList()[2], newEnv);
            }
            Environment* newEnv = new Environment(p.getList()[1].getList(), args, env);
            return evaluate(p.getList()[0], newEnv);
        } else if (p.getType()==SchemeList::PROC) {
             if(s.getList()[0].val() == "map"){
                SLists mapLst;
                SchemeList tmp = p.getProc()(args,env);
                for(int i =0;i<tmp.getList().size(); i++){
                    SchemeList k = evaluate(tmp.getList()[i],env);
                    mapLst.push_back(k);
                }
                SchemeList res(mapLst);
                return res;
             }
           if(s.getList()[0].val() == "apply"){
                SchemeList tmp = p.getProc()(args,env);

                return evaluate(tmp,env);
           }
            return p.getProc()(args,env);
        } else {
            return evaluate(p,env);
        }
    }
}



void env_setup (Environment* std_env) {
    std_env->env.insert({"#f",SchemeList("#f")});
    std_env->env.insert({"#t",SchemeList("#t")});
    std_env->env.insert({"nil",SchemeList("nil")});
    std_env->env.insert({"+",SchemeList(&add)});
    std_env->env.insert({"-",SchemeList(&subtract)});
    std_env->env.insert({"*",SchemeList(&multiply)});
    std_env->env.insert({"/",SchemeList(&divide)});
    std_env->env.insert({"mod",SchemeList(&mod)});
    std_env->env.insert({"sqrt",SchemeList(&sqrt)});
    std_env->env.insert({"abs",SchemeList(&abs)});
    std_env->env.insert({">",SchemeList(&greater_than)});
    std_env->env.insert({"<",SchemeList(&less_than)});
    std_env->env.insert({"=",SchemeList(&equal_num)});
    std_env->env.insert({"=",SchemeList(&equal_num)});
    std_env->env.insert({"append",SchemeList(&append)});
    std_env->env.insert({"apply",SchemeList(&apply)});
    std_env->env.insert({"map",SchemeList(&map)});
    std_env->env.insert({"max",SchemeList(&max)});
    std_env->env.insert({"min",SchemeList(&min)});
    std_env->env.insert({"eqv",SchemeList(&eqv)});
    std_env->env.insert({"number?",SchemeList(&isNumber)});
    std_env->env.insert({"list?",SchemeList(&isList)});
    std_env->env.insert({"procedure?",SchemeList(&isProcedure)});
    std_env->env.insert({"null?",SchemeList(&isNull)});
    std_env->env.insert({"length",SchemeList(&length)});
    std_env->env.insert({"list",SchemeList(&list)});
    std_env->env.insert({"cons",SchemeList(&cons)});
    std_env->env.insert({"car",SchemeList(&car)});
    std_env->env.insert({"cdr",SchemeList(&cdr)});
   

}

//repl
int main(int argc, const char * argv[]) {
    Environment* std_env = new Environment;
    env_setup (std_env);
                  
    //tests
    for (int i = 0; i<13;i++){

        SchemeList temp;
        //tests for basic arithemtics
        if(i==0){           
            std::string line = "(- (* 2 3) (/ 10 2) (+ 5 1))";
            temp = evaluate(Parser::parse(line), std_env);           
            std:: string result = temp.getPrintString();
            if(result !="-5"){
                cout<<"TEST 1: FAILED"<<endl;
                break;
            }
            line = "(/ (* (- 8 3) (+ 2 4)) (sqrt 16))";
            temp = evaluate(Parser::parse(line), std_env);
            result = temp.getPrintString();
            if(result != "7.500000"){
                cout<<"TEST 1: FAILED"<<endl;
                break;
            }
            line = "(- (* (+ 2 3) 4) (/ 20 2) (mod 8 3))";
            temp = evaluate(Parser::parse(line), std_env);
            result = temp.getPrintString();
            if(result != "8"){
                cout<<"TEST 1: FAILED"<<endl;
                break;
            }
            line = "(* 15 15)";
            temp = evaluate(Parser::parse(line), std_env);
            result = temp.getPrintString();
            if(result != "225"){
                cout<<"TEST 1: FAILED"<<endl;
                break;
            }
            cout<<"TEST 1 (basic arithmetics): PASSED"<<endl;

        }
        //tests for if/else statement
        if(i==1){
            std::string line = "(if (= 2 2) 1 0)";
            temp = evaluate(Parser::parse(line), std_env);           
            std:: string result = temp.getPrintString();
            if(result !="1"){
                cout<<"TEST 2: FAILED"<<endl;
                break;
            }
           
            line = "(define a 5)";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(define b 10)";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "(if (< a b)(if (> a 0)1 2)3)";
            SchemeList res = evaluate(Parser::parse(line), std_env);          
            result = res.getPrintString();
            if(result !="1"){
                cout<<"TEST 2: FAILED"<<endl;
                break;
            }

            line = "(define age 25)";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(define has-license #t)";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "(if (and (< age 30) has-license)1 0)";
            res = evaluate(Parser::parse(line), std_env);          
            result = res.getPrintString();
            if(result !="1"){
                cout<<"TEST 2: FAILED"<<endl;
                break;
            }
            line = "(define result(if (> 11 10)1 0))";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "result";      
            temp = evaluate(Parser::parse(line), std_env);   
            result = temp.getPrintString();
            if(result !="1"){
                cout<<"TEST 2: FAILED"<<endl;
                break;
            }          
            cout<<"TEST 2 (if/else): PASSED"<<endl;

        }
        //logical operations (and, or).
        if(i==2){
            std::string line = "((and (< 4 8) (> 9 7)))";
            temp = evaluate(Parser::parse(line), std_env);           
            std:: string result = temp.getPrintString();

            if(result !="true"){
                cout<<"TEST 3: FAILED"<<endl;
                break;
            }
            line = "((or (< 4 3) (> 9 7)))";
            temp = evaluate(Parser::parse(line), std_env);           
            result = temp.getPrintString();

            if(result !="true"){
                cout<<"TEST 3: FAILED"<<endl;
                break;
            }
            line = "(define grade 'B')";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(define is-passing #t)";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "((or (= grade 'A) is-passing)";  
            if(result !="true"){
                cout<<"TEST 3: FAILED"<<endl;
                break;
            }
            line = "(define x 5)";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(define y 10)";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "(define z 15)";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "(and (< x y) (or (> y z) (= x 5)))";  
            if(result !="true"){
                cout<<"TEST 3: FAILED"<<endl;
                break;
            }
            cout<<"TEST 3 (logical operations): PASSED"<<endl;
        }
        // test for car cons cdr
        if(i==3){
            std::string line = "(define my-list '(1 2 3 4))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "(car my-list)";
            temp = evaluate(Parser::parse(line), std_env);        
            std:: string result = temp.getPrintString();
            if(result !="1"){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            line = "(define lst '(1 2 3 4))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "(cdr lst)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();             
            if(result != "(2 3 4) "){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            line = "(define my-list '(2 3 4))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "(cons 1 my-list)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();                   
            if(result != "(1 2 3 4) "){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            line = "(define nested-list '(1 (2 3) (4 5)))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "(car nested-list)";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(car (cdr nested-list))";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();                     
            if(result != "(2 3) "){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            line = "(define pair (cons 1 2))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "(car pair)";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(cdr pair)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();                     
            if(result != "(. 2) "){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            line = "(define my-list (cons 1 (cons 2 (cons 3 '()))))";
            temp = evaluate(Parser::parse(line), std_env);   
            line = "my-list";
            temp = evaluate(Parser::parse(line), std_env);     
            result = temp.getPrintString();                    
            if(result != "(1 2 3) " ){
                cout<<"TEST 4 (car cons cdr): FAILED"<<endl;
                break;
            }
            cout<<"TEST 4 (car cons cdr): PASSED"<<endl;
        }
        //tests null? 
        if(i==4){
            std::string line = "(null? '())";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "true"){
                cout<<"TEST 5 (null?): FAILED"<<endl;
                break;
            }
            line = "(null? '(1 2 3))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "false"){
                cout<<"TEST 5 (null?): FAILED"<<endl;
                break;
            }
            line = "(define empty-list '())";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "(null? empty-list)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "true"){
                cout<<"TEST 5 (null?): FAILED"<<endl;
                break;
            }
            line = "(null? '(1 (null? 2) 3))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "false"){
                cout<<"TEST 5 (null?): FAILED"<<endl;
                break;
            }
            cout<<"TEST 5 (null?): PASSED"<<endl;
        }
        //tests for length
        if(i==5){
            std::string line = "(length '())";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "0"){
                cout<<"TEST 6 (length): FAILED"<<endl;
                break;
            }
            line = "(length '(1 2 3 4 5))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "5"){
                cout<<"TEST 6 (length): FAILED"<<endl;
                break;
            }
            line = "(length '(1 (2 3) (4 (5) 6))))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "3"){
                cout<<"TEST 6 (length): FAILED"<<endl;
                break;
            }
            line = "(length '(2 '(2 3) 5))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "3"){
                cout<<"TEST 6 (length): FAILED"<<endl;
                break;
            }
            cout<<"TEST 6 (length): PASSED"<<endl;
        }
        //test cases for eval
        if(i==6){
            std::string line = "(eval '(+ (* 2 3) (- 10 5)))";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "11"){
                cout<<"TEST 7 (eval): FAILED"<<endl;
                break;
            }
            line = "(eval '(if (= 1 1) (+ 2 3) (- 7 4)))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "5"){
                cout<<"TEST 7 (eval): FAILED"<<endl;
                break;
            }
            line = "(eval '(car (cdr (list 1 2 3))))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "2"){
                cout<<"TEST 7 (eval): FAILED"<<endl;
                break;
            }
            line = "(eval '(list (+ 1 2) 4 6))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(3 4 6) "){
                cout<<"TEST 7 (eval): FAILED"<<endl;
                break;
            }
            cout<<"TEST 7 (eval): PASSED"<<endl;
        }
        //test cases for apply
        if(i==7){
            std::string line = "(apply * '(2 3 4 5))";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "120"){
                cout<<"TEST 8 (apply): FAILED"<<endl;
                break;
            }
            line = "(define (add-three-nums a b c) (+ a b c))";
            temp = evaluate(Parser::parse(line), std_env);        
            line = "(apply add-three-nums '(1 2 3))";
            temp = evaluate(Parser::parse(line), std_env);            
            result = temp.getPrintString();
            if(result != "6"){
                cout<<"TEST 8 (apply): FAILED"<<endl;
                break;
            }
            line = "(define (nested-lists a b c) (+ a (* b (apply + c))))";
            temp = evaluate(Parser::parse(line), std_env);        
            line = "(nested-lists 1 2 '(3 4 5))";
            temp = evaluate(Parser::parse(line), std_env);            
            result = temp.getPrintString();
            if(result != "25"){
                cout<<"TEST 8 (apply): FAILED"<<endl;
                break;
            }
            cout<<"TEST 8 (apply): PASSED"<<endl;
        }
        //test cases for define
        if(i==8){
            std::string line = "(define x 5)";
            temp = evaluate(Parser::parse(line), std_env);          
            line = "x";
            temp = evaluate(Parser::parse(line), std_env);
            std:: string result = temp.getPrintString();
            if(result != "5"){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 9 (define): FAILED"<<endl;
                break;
            }
            line = "(define (square x) (* x x))";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "(square 4)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();
            if(result != "16"){
                cout<<"RES"<<result<<endl;
                cout<<"TEST 9 (define): FAILED"<<endl;
                break;
            }
            line = "(define (add-three-numbers a b c) (+ a b c))";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "(add-three-numbers 1 2 3)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();
            if(result != "6"){
                cout<<"RES"<<result<<endl;
                cout<<"TEST 9 (define): FAILED"<<endl;
                break;
            }
            line = "(define (add-three-numbers a b c) (+ a b c))";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "(add-three-numbers 1 2 3)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();
            if(result != "6"){
                cout<<"RES"<<result<<endl;
                cout<<"TEST 9 (define): FAILED"<<endl;
                break;
            }
            line = "(define myList '(1 2 3))";
            temp = evaluate(Parser::parse(line), std_env);  
            line = "(car myList)";
            temp = evaluate(Parser::parse(line), std_env);        
            result = temp.getPrintString();
            if(result != "1"){
                cout<<"RES"<<result<<endl;
                cout<<"TEST 9 (define): FAILED"<<endl;
                break;
            }
            cout<<"TEST 9 (define): PASSED"<<endl;
        }
        //lambda function execution
        if(i==9){
            std::string line = "((lambda (x) (* x x)) 5)";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "25"){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 10 (lambda): FAILED"<<endl;
                break;
            }
            line = "((lambda (x y) (+ x (* 2 y))) 3 4)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "11"){
                cout<<"TEST 10 (lambda): FAILED"<<endl;
                break;
            }
            line = "((lambda (f x) (f x)) (lambda (y) (+ y 1)) 5)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "6"){
                cout<<"TEST 10 (lambda): FAILED"<<endl;
                break;
            }
            line = "((lambda (f x) (f (f x))) (lambda (y) (+ y 1)) 3)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "5"){
                cout<<"TEST 10 (lambda): FAILED"<<endl;
                break;
            }
            cout<<"TEST 10 (lambda): PASSED"<<endl;
        }
        //tests for append
        if(i==10){
            std::string line = "(append '(1 2) '(3 4))";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "(1 2 3 4) "){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 11 (append): FAILED"<<endl;
                break;
            }
            line = "(append '(1 2) '())";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(1 2) "){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 11 (append): FAILED"<<endl;
                break;
            }
            line = "(append '(1) '(2) '(3))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(1 2 3) "){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 11 (append): FAILED"<<endl;
                break;
            }
            line = "(append '(1 2) '((3 4) (5 6)))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(1 2 (3 4)  (5 6)) "){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 11 (append): FAILED"<<endl;
                break;
            }
            line = "(append '(1) '(() ()) '(2))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(1 ()  ()  2) "){
                cout<<line<<endl;
                cout<<result<<endl;
               
                cout<<"TEST 11 (append): FAILED"<<endl;
                break;
            }
            cout<<"TEST 11 (append): PASSED"<<endl;
        }
        //list without implementing
        if(i==11){
            std::string line = "'(1 2 3 4)";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "(1 2 3 4) "){
                cout<<"TEST 12 (list without implementing): FAILED"<<endl;
                break;
            }
            line = "'(apple banana cherry)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(apple banana cherry) "){
                cout<<line<<endl;
                cout<<result<<endl;
                cout<<"TEST 12 (list without implementing): FAILED"<<endl;
                break;
            }
            line = "'(apple 42 (banana 3) hello)";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(apple 42 (banana 3)  hello) "){
                break;
            }
            cout<<"TEST 12 (list without implementing): PASSED"<<endl;
        }
        //test cases for map
        if(i==12){
            std::string line = "(map length '('(1 3) '(3 4) '(5 6)))";
            temp = evaluate(Parser::parse(line), std_env);          
            std:: string result = temp.getPrintString();
            if(result != "(2 2 2) "){
                cout<<"TEST 13 (map): FAILED"<<endl;
                break;
            }
            line = "(define (add-two-numbers x y) (+ x y)))";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(map add-two-numbers '(1 2 3) '(4 5 6))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(5 7 9) "){
                cout<<"TEST 13 (map): FAILED"<<endl;
                break;
            }
            line = "(define (add-one x) (+ x 1))";
            temp = evaluate(Parser::parse(line), std_env);
            line = "(define (double x) (* 2 x))";
            temp = evaluate(Parser::parse(line), std_env); 
            line = "(map add-one (map double '(1 2 3)))";
            temp = evaluate(Parser::parse(line), std_env);          
            result = temp.getPrintString();
            if(result != "(3 5 7) "){
                cout<<"TEST 13 (map): FAILED"<<endl;
                break;
            }
            line = "(map car '((1 . 10) (2 . 20) (3 . 30)))";
            temp = evaluate(Parser::parse(line), std_env);    
            result = temp.getPrintString();
            if(result != "(1 2 3) "){
                cout<<"TEST 13 (map): FAILED"<<endl;
                break;
            }
            cout<<"TEST 13 (map): PASSED"<<endl;
        }
        



    }
    int kawaCount = 1;
    while (true) {
        SchemeList temp;
        try {
            
            std::cout << "#|kawa:"<<kawaCount<<"|#";
            std::string line = ReadFile::readLine();
            if (line == "exit")break;
            if (line.length()==0) continue;
            temp = evaluate(Parser::parse(line), std_env);
        } catch (const char* msg) {
           // cerr << msg << endl;
            continue;
        } catch (...) {
            cerr << "Interpreter Error: Unhandled Exception" << endl;
            continue;
        }
        cout << "=> " << temp.getPrintString() << endl;
            
    }



    return 0;
}
