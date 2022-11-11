#include "aka_input_system.h"
#include <QStringBuilder>
#include "Terminal/Info/aka_commands.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "aka_global.h"

#include <QDebug>

// 初始化静态成员
AkaInputSystem *AkaInputSystem::Instance_ = nullptr;

AkaInputSystem *AkaInputSystem::GetInputSystem()
{
    if(Instance_ == nullptr)
        Instance_ = new AkaInputSystem();
    return Instance_;
}

QString AkaInputSystem::GetLineFromPlain(QString in)
{
    return in.mid(in.indexOf(aka::KAkaConselSymbol)+1);
}

QStringList AkaInputSystem::SplitLine(QString line)
{
    if(line.length() <= 0) return QStringList();

    // 先按引号分隔 TODO
    QString s_temp = "";
    QStringList list;
    bool hit = false, isEscape = false;
    for(QChar ch:line)
    {
        if(!isEscape)
        {
            // 转义符号"\"
            if(ch == '\\')
            {
                isEscape = true;
                continue;
            }
        }
        else
        {
            // 对转义符号的处理
            if(ch == '"' || ch == '\\')
            {
                isEscape = false;
                s_temp = s_temp % ch;
                continue;
            }
        }

        if(ch == '"') hit = !hit;

        if((hit && ch != '"') || (!hit && ch != ' ' && ch != '"'))
            s_temp = s_temp % ch;
        else
        {
            list.append(s_temp);
            s_temp = "";
        }
    }

    // 引号是否成对出现
    if(hit)
    {
        aka::PrintError("Missing quotes or quotes are not pairs.", KAkaMissingQuote);
        return QStringList();
    }

    list.append(s_temp);
    list.removeAll("");

    return list;
}

int AkaInputSystem::Execute(QStringList args)
{
    if(args.length() <= 0) return 1; // 没有参数，不执行命令
    for(auto cmds:KBuiltin_Cmds.toStdMap())
    {
        // 循环命令名称及其别名
        for(int i = 0; i < cmds.first.length()-1; i++)
        {
            if(cmds.first[i] == args[0])
            {
                if(cmds.second == nullptr)
                {
                    // 绑定命令的函数不存在(为null)或者绑定出错
                    aka::PrintError("Command bind null func.", KAkaInvlidCmdFunc);
                    return 1;
                }
                return (*cmds.second)(args);
            }
        }
    }

    // 未找到命令
    aka::PrintError("Commands Not Found.", KAkaCmdNotFound);
    return 1;
}

AkaInputSystem::AkaInputSystem()
{
}

AkaInputSystem::~AkaInputSystem()
{
    if(Instance_ != nullptr)
        delete Instance_;
}
