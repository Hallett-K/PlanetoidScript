#pragma once

#include "Value.hpp"

struct ApplicationState
{
    bool hasError = false;

    bool canBreak = false;
    bool canContinue = false;
    bool canReturn = false;

    bool breakCalled = false;
    bool continueCalled = false;
    bool returnCalled = false;
    Value returnValue;

    bool canDefineObject = true;

    std::string currentDirectory = "";
};