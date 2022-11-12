#include "terminal_manager.h"

TerminalManager* TerminalManager::Instance_ = nullptr;

TerminalManager *TerminalManager::GetTerminalManager()
{
    if(Instance_ == nullptr)
        Instance_ = new TerminalManager();
    return Instance_;
}

AkaInputSystem *TerminalManager::GetInputSystem()
{
    if(InputSystem_ == nullptr)
        InputSystem_ = new AkaInputSystem();
    return InputSystem_;
}

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
}
