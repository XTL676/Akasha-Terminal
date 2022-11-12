#ifndef TERMINALMANAGER_H
#define TERMINALMANAGER_H
#include <QPlainTextEdit>
#include "Terminal/Input/aka_input_system.h"

class TerminalManager
{
public:
    static TerminalManager* GetTerminalManager();
    void Init(QPlainTextEdit* MainEditArea);

    void Print(QString msg);
    void Print(QString msg, QColor color);
    void PrintError(QString msg, int code);

    QPlainTextEdit* GetMainEditArea();
    AkaInputSystem* GetInputSystem();

private:
    static TerminalManager* Instance_;
    QPlainTextEdit* MainEditArea_ = nullptr;
    AkaInputSystem* InputSystem_ = nullptr;

private:
    TerminalManager();
    ~TerminalManager();
};

#endif // TERMINALMANAGER_H
