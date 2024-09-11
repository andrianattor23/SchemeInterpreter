#include "Environment.hpp"
#include "SchemeList.hpp"


Environment::Environment() : outerEnv(nullptr) {}

Environment::Environment(SLists params, SLists args, Environment* outerEnv) : outerEnv(outerEnv) {
    auto argIterator = args.cbegin();
    for (const auto& param : params) {
        env[param.val()] = *(argIterator++);
    }
}

envType* Environment::find(symbol s) {
    auto it = env.find(s);
    if (it != env.end()) {
        return &env;
    } else {
        if (outerEnv == nullptr) {
            throw "ERROR: Unbound Symbol";
        } else {
            return outerEnv->find(s);
        }
    }
}
