#ifndef ACTIONGENERATOR_H
#define ACTIONGENERATOR_H
#include <vector>
#include "Action.h"
using namespace std;
class ActionGenerator
{
public:
    static vector<Action*> acVector;
    static void genData();
    ~ActionGenerator();
};
#endif
