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
    void Print(QString msg, bool newline);
    void Print(QString msg, QColor color);
    void Print(QString msg, QColor color, bool newline);
    void PrintError(QString msg, int code);

    QPlainTextEdit* GetMainEditArea();
    AkaExceptionSystem* GetExceptionSystem();
    AkaFileSystem* GetFileSystem();

private:
    static KernelManager* Instance_;
    QPlainTextEdit* MainEditArea_ = nullptr;
    AkaExceptionSystem* ExceptionSystem_ = nullptr;
    AkaFileSystem* FileSystem_ = nullptr;

private:
    KernelManager();
    ~KernelManager();
};

#endif // KERNELMANAGER_H
