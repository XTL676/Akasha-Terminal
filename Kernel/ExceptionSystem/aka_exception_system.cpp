#include "aka_exception_system.h"
#include "aka_global.h"

// 初始化静态成员
AkaExceptionSystem *AkaExceptionSystem::Instance_ = nullptr;

AkaExceptionSystem *AkaExceptionSystem::GetExceptionSystem()
{
    if(Instance_ == nullptr)
        Instance_ = new AkaExceptionSystem();
    return Instance_;
}

AkaExceptionSystem::AkaExceptionSystem()
{
}

AkaExceptionSystem::~AkaExceptionSystem()
{
}
