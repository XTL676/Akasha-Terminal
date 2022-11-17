#ifndef TERMINALMANAGER_H
#define TERMINALMANAGER_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QApplication>
#include "Terminal/Input/aka_input_system.h"

class TerminalManager
{
public:
    static TerminalManager* GetTerminalManager();
    void Init(QMainWindow* MainWindow, QPlainTextEdit* MainEditArea);

    void Print(QString msg);
    void Print(QString msg, QColor color);
    void PrintError(QString msg, int code);

    QPlainTextEdit* GetMainEditArea();
    QMainWindow* GetMainWindow();
    QApplication* GetApp();

    AkaInputSystem* GetInputSystem();

private:
    static TerminalManager* Instance_;
    QPlainTextEdit* MainEditArea_ = nullptr;
    QMainWindow* MainWindow_ = nullptr;

    AkaInputSystem* InputSystem_ = nullptr;

private:
    TerminalManager();
    ~TerminalManager();
};

#endif // TERMINALMANAGER_H
