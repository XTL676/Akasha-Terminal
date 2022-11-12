#ifndef AKAEXCEPTIONSYSTEM_H
#define AKAEXCEPTIONSYSTEM_H
#include <QString>
#include <QPlainTextEdit>
#include "Kernel/abstract_kernel_system.h"

class AkaExceptionSystem : public AbstractKernelSystem
{
    friend class KernelManager;

private:
    AkaExceptionSystem();
    ~AkaExceptionSystem();
};

#endif // AKAEXCEPTIONSYSTEM_H
