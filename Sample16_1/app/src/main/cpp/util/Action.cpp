#include "Action.h"
Action::~Action()
{
    for (int i = 0; i < 10; ++i) {
        delete [] data[i];
    }
}