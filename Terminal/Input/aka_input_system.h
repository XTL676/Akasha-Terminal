#ifndef AKAINPUTSYSTEM_H
#define AKAINPUTSYSTEM_H
#include <QStringList>

class AkaInputSystem
{
public:
    static AkaInputSystem* GetInputSystem(); // 获取输入系统
    QString GetLineFromPlain(QString in); // 从多行文本框获取行内容
    QStringList SplitLine(QString line); // 分隔行，获取命令参数集合
    int Execute(QStringList args); // 执行终端命令

private:
    static AkaInputSystem* Instance_;

private:
    AkaInputSystem();
    ~AkaInputSystem();
};

#endif // AKAINPUTSYSTEM_H
