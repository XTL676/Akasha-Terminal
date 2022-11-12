#ifndef KERNELMANAGER_H
#define KERNELMANAGER_H
#include <QPlainTextEdit>
#include "Kernel/ExceptionSystem/aka_exception_system.h"
#include "Kernel/FileSystem/aka_file_system.h"

class KernelManager
{
public:
    static KernelManager* GetKernelManager();
    void Init(QPlainTextEdit* MainEditArea);

    void Print(QString msg);
    void Print(QString msg, QColor color);
    void PrintError(QString msg, int code);

    QPlainTextEdit* GetMainEditArea();
    AkaExceptionSystem* GetExceptionSystem();
    AkaFileSystem* GetFileSystem();

private:
    static KernelManager* Instance_;
    QPlainTextEdit* MainEditArea_;
    AkaExceptionSystem* ExceptionSystem_;
    AkaFileSystem* FileSystem_;

private:
    KernelManager();
    ~KernelManager();
};

#endif // KERNELMANAGER_H
