#include "kernel_manager.h"

KernelManager* KernelManager::Instance_ = nullptr;

KernelManager *KernelManager::GetKernelManager()
{
    if(Instance_ == nullptr)
        Instance_ = new KernelManager();
    return Instance_;
}

AkaExceptionSystem *KernelManager::GetExceptionSystem()
{
    if(ExceptionSystem_ == nullptr)
        ExceptionSystem_ = new AkaExceptionSystem();
    return ExceptionSystem_;
}

AkaFileSystem *KernelManager::GetFileSystem()
{
    if(FileSystem_ == nullptr)
        FileSystem_ = new AkaFileSystem();
    return FileSystem_;
}

KernelManager::KernelManager()
{
}

KernelManager::~KernelManager()
{
}
