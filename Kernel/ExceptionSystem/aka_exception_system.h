#ifndef AKAEXCEPTIONSYSTEM_H
#define AKAEXCEPTIONSYSTEM_H
#include <QString>
#include <QPlainTextEdit>

class AkaExceptionSystem
{
public:
    static AkaExceptionSystem* Instance_;

public:
    static AkaExceptionSystem* GetExceptionSystem();

private:
    AkaExceptionSystem();
    ~AkaExceptionSystem();
};

#endif // AKAEXCEPTIONSYSTEM_H
