#ifndef AKA_COMMANDS_H
#define AKA_COMMANDS_H
#include <QStringList>
#include <QMap>
#include <Terminal/Display/aka_display_cmd.h>
#include "Kernel/Basic/aka_basic_cmd.h"

typedef int(*builtin_func)(QStringList);
typedef const QMap<const QList<QString>, builtin_func> builtin_cmd_map;

// 所有内核命令
/**
 *  格式：
 *      {{"命令名称", "命令别名1", "命令别名2", ..., "命令别名n", "命令描述"},&函数名称}
 */
static const builtin_cmd_map KBuiltin_Cmds =
{
    {{"cd", "Change directory.[cd path]"},nullptr},
    {{"help", "List all available commands."},&AkaBasicCmd::help},
    {{"exit", "ex", "e", "Exit Akasha Terminal."},&AkaBasicCmd::exit},
    {{"ls", "List all files and dirs on current path."},nullptr},
    {{"resize", "Resize window.[resize width height]"},&AkaDisplayCmd::resize},
    {{"refont", "Set font.[refont (size) (fontname)]"},&AkaDisplayCmd::refont}
};

#endif // AKA_COMMANDS_H
