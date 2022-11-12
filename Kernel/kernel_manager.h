#ifndef KERNELMANAGER_H
#define KERNELMANAGER_H
#include "Kernel/ExceptionSystem/aka_exception_system.h"
#include "Kernel/FileSystem/aka_file_system.h"

class KernelManager
{
public:
    static KernelManager* GetKernelManager();
    AkaExceptionSystem* GetExceptionSystem();
    AkaFileSystem* GetFileSystem();

private:
    static KernelManager* Instance_;
    AkaExceptionSystem* ExceptionSystem_;
    AkaFileSystem* FileSystem_;

private:
    KernelManager();
    ~KernelManager();
};

#endif // KERNELMANAGER_H
