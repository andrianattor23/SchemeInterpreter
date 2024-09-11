#include "NativeProcedures.hpp"
#include <vector>


using namespace std;

SchemeList add (const SLists& argv,Environment* env)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal += atof((*vi).val().c_str());
    return SchemeList(dVal);
}

SchemeList subtract (const SLists& argv,Environment* env)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal -= atof((*vi).val().c_str());
    return SchemeList(dVal);
}

SchemeList multiply (const SLists& argv,Environment* env)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal *= atof((*vi).val().c_str());
    return SchemeList(dVal);
}

SchemeList divide (const SLists& argv,Environment* env)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal /= atof((*vi).val().c_str());
    return SchemeList(dVal);
}

SchemeList mod (const SLists& argv,Environment* env)  {
    int iVal = atoi(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        iVal %= atoi((*vi).val().c_str());
    return SchemeList(iVal);
}

SchemeList sqrt (const SLists& arg,Environment* env)  {
    return SchemeList(sqrt(atof(arg[0].val().c_str())));
}



SchemeList abs (const SLists& arg,Environment* env) {
    return SchemeList(fabs(atof(arg[0].val().c_str())));
}

SchemeList greater_than (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) >= atof((*vi).val().c_str()))
            return SchemeList("#f");
    }
    return SchemeList("#t");
}

SchemeList less_than (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) <= atof((*vi).val().c_str()))
            return SchemeList("#f");
    }
    return SchemeList("#t");
}

SchemeList equal_num (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (fabs(atof((*(vi+1)).val().c_str())-atof((*vi).val().c_str()))>0.00000001) {
            return SchemeList("#f");
        }
    }
    return SchemeList("#t");
}

SchemeList greater_equal (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) > atof((*vi).val().c_str()))
            return SchemeList("#f");
    }
    return SchemeList("#t");
}

SchemeList less_equal (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) < atof((*vi).val().c_str()))
            return SchemeList("#f");
    }
    return SchemeList("#t");
}

SchemeList append (const SLists& argv,Environment* env) {
    SchemeList l;
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        l.pushList(vi->getList());
        
    }
    return SchemeList(l);
} 

SchemeList apply (const SLists& argv,Environment* env) {
    std::string funcName = argv[0].getPrintString();
    SLists newA = argv;
    if(argv[0].getTypeString()!="PROC"){
        SLists finalArgs;
        SchemeList first = newA[0].getList()[1];        
        SchemeList firstSec = newA[0].getList()[0];       
        SchemeList lambd("lambda");      
        SLists newArgs;
        newArgs.push_back(lambd);
        newArgs.push_back(first);
        newArgs.push_back(firstSec);
        SchemeList body(newArgs);
        finalArgs.push_back(body);
        for(int i=0;i<newA[1].getList().size();i++){
            SchemeList arg = newA[1].getList()[i];
            finalArgs.push_back(arg);
        }
        SchemeList lambdaList(finalArgs);
        lambdaList.setType(SchemeList::LAMBDA);
        return lambdaList;
    }else{
        return newA[0].getProc()(newA[1].getList(),env);
    }   
}

SchemeList map (const SLists& argv,Environment* env) {
    SchemeList newList(SchemeList::LIST);
    
    for (int i = 0; i < argv[1].getList().size(); i++) {
        SLists n;
        SchemeList args(SchemeList::LIST);
        if(argv[0].getType()!=SchemeList::PROC){
            n.push_back(argv[0]);

        }else{
            n.push_back(argv[0].getProc());
        }
        for (int j = 1; j < argv.size(); j++) {
            args.push(argv[j].getList()[i]);
        }
        n.push_back(args);
        newList.push(apply(n,env));
    }
    return newList;
}

SchemeList max (const SLists& argv,Environment* env) {
    double max = -std::numeric_limits<double>::infinity();
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        if (atof(vi->val().c_str()) > max)
            max = atof(vi->val().c_str());
    }
    return max;
}

SchemeList min (const SLists& argv,Environment* env) {
    
    double min = std::numeric_limits<double>::infinity();
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        if (atof(vi->val().c_str()) < min)
            min = atof(vi->val().c_str());
    }
    return min;
}

SchemeList eqv (const SLists& argv,Environment* env) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (vi->getType() != (vi+1)->getType()) {
            return SchemeList("#f");
        } else if (vi->getType() == SchemeList::SYMBOL) {
            if (vi->val() == "#f" || vi->val() != (vi+1)->val())
                return SchemeList("#f");
        } else if (vi->getType() == SchemeList::NUMBER) {
            if (fabs(atof((*(vi+1)).val().c_str())-atof((*vi).val().c_str()))>0.00000001)
                return SchemeList("#f");
        } else if (vi->getType() == SchemeList::LAMBDA || vi->getType() == SchemeList::PROC) {
            if (vi->getProc() != (vi+1)->getProc())
                return SchemeList("#f");
        } else {
            return eqv(vi->getList(),env);
        }
    }
    return SchemeList("#t");
}

SchemeList isNumber (const SLists& arg,Environment* env)  {
    return arg[0].getType()==SchemeList::NUMBER ? SchemeList("#t") : SchemeList("#f");
}

SchemeList isProcedure (const SLists& arg,Environment* env)  {
    return arg[0].getType()==SchemeList::PROC ? SchemeList("#t") : SchemeList("#f");
}

SchemeList isSymbol (const SLists& arg,Environment* env)  {
    return arg[0].getType()==SchemeList::SYMBOL ? SchemeList("#t") : SchemeList("#f");
}

SchemeList isList (const SLists& arg,Environment* env)  {
    return arg[0].getType()==SchemeList::LIST ? SchemeList("#t") : SchemeList("#f");
}

SchemeList isNull (const SLists& arg,Environment* env) {
    if (arg[0].getType()!=SchemeList::LIST) return SchemeList("#f");
    if (arg[0].getList().size()==0) return SchemeList("#t");
    return SchemeList("#f");
}

SchemeList length (const SLists& arg,Environment* env)  {
    return arg[0].getList().size();
}

SchemeList list (const SLists& argv,Environment* env) {
    SchemeList newList(SchemeList::LIST);
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        newList.push(*vi);
    }
    return newList;
}

SchemeList cons (const SLists& argv,Environment* env) {
    SchemeList newList(SchemeList::LIST);
    newList.push(argv[0]);
    if (argv[1].getType()== 2){
        std::vector <SchemeList> x = argv[1].getList();
        for(int i=0;i<x.size();i++){
            newList.push(x[i]);
        }
    }else{
        SchemeList period(".");
        newList.push(period);
        newList.push(argv[1]);
    }
    return newList; 
}

SchemeList car (const SLists& argv,Environment* env) {
    return argv[0].getList()[0];
}

SchemeList cdr (const SLists& argv,Environment* env) {
    SchemeList s(SchemeList::LIST);
    for (int i = 1; i < argv[0].getList().size(); i++) {
        s.push(argv[0].getList()[i]);
    }
    
    return s;
}

/*void declareEnv(Environment* std_env){
    pEnv = std_env;
}*/

/*SchemeList and_(const SLists& argv){
   for (auto vi = argv.begin() ; vi != argv.end(); vi++){
        if(*vi->)
   }
    return SchemeList("#t");
}*/