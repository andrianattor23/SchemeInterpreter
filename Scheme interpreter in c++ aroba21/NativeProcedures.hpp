#ifndef NativeProcedures_hpp
#define NativeProcedures_hpp

#include "Environment.hpp"
#include "SchemeList.hpp"
#include "stdlib.h"
#include "math.h"



//primitives
SchemeList add (const SLists& arg,Environment* env);
SchemeList subtract (const SLists& arg,Environment* env);
SchemeList multiply (const SLists& arg,Environment* env);
SchemeList divide (const SLists& arg,Environment* env);
SchemeList mod (const SLists& arg,Environment* env);
SchemeList sqrt (const SLists& arg,Environment* env);
SchemeList max (const SLists& argv,Environment* env);
SchemeList min (const SLists& argv,Environment* env);
SchemeList abs (const SLists& arg,Environment* env);
SchemeList greater_than(const SLists& argv,Environment* env);
SchemeList less_than (const SLists& argv,Environment* env);
SchemeList equal_num (const SLists& argv,Environment* env);
SchemeList append (const SLists& argv,Environment* env);
SchemeList apply (const SLists& argv,Environment* env);
SchemeList map (const SLists& argv,Environment* env);
SchemeList max (const SLists& argv,Environment* env);
SchemeList max (const SLists& argv,Environment* env);
SchemeList eqv (const SLists& argv,Environment* env);

SchemeList isNumber (const SLists& arg,Environment* env);
SchemeList isProcedure (const SLists& arg,Environment* env);
SchemeList isSymbol (const SLists& arg,Environment* env);
SchemeList isList (const SLists& arg,Environment* env);
SchemeList isNull (const SLists& arg,Environment* env);

SchemeList length (const SLists& arg,Environment* env);
SchemeList list (const SLists& argv,Environment* env);
SchemeList cons (const SLists& argv,Environment* env);
SchemeList car (const SLists& argv,Environment* env);
SchemeList cdr (const SLists& argv,Environment* env);




#endif 
