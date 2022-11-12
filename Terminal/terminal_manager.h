#ifndef TERMINALMANAGER_H
#define TERMINALMANAGER_H
#include "Terminal/Input/aka_input_system.h"

class TerminalManager
{
public:
    static TerminalManager* GetTerminalManager();
    AkaInputSystem* GetInputSystem();

private:
    static TerminalManager* Instance_;
    AkaInputSystem* InputSystem_ = nullptr;

private:
    TerminalManager();
    ~TerminalManager();
};

#endif // TERMINALMANAGER_H
