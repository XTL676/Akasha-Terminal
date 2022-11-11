#ifndef AKAEXCEPTIONSYSTEM_H
#define AKAEXCEPTIONSYSTEM_H
#include <QString>
#include <QPlainTextEdit>

class AkaExceptionSystem
{
public:
    static AkaExceptionSystem* GetExceptionSystem();

private:
    static AkaExceptionSystem* Instance_;

private:
    AkaExceptionSystem();
    ~AkaExceptionSystem();
};

#endif // AKAEXCEPTIONSYSTEM_H
