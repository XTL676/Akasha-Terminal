#ifndef AKAINPUTSYSTEM_H
#define AKAINPUTSYSTEM_H
#include <QStringList>
#include "Terminal/abstract_terminal_system.h"

class AkaInputSystem : public AbstractTerminalSystem
{
    friend class TerminalManager;

public:
    QString GetLineFromPlain(QString in); // 从多行文本框获取行内容
    QStringList SplitLine(QString line); // 分隔行，获取命令参数集合
    int Execute(QStringList args); // 执行终端命令

private:
    AkaInputSystem();
    ~AkaInputSystem();
};

#endif // AKAINPUTSYSTEM_H
