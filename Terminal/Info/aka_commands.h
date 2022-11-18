#ifndef AKA_COMMANDS_H
#define AKA_COMMANDS_H
#include <QStringList>
#include <QMap>
#include <QObject>
#include "Kernel/FileSystem/aka_file_cmd.h"
#include "Terminal/Display/aka_display_cmd.h"
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
    {{"cd", QObject::tr("Change directory.[cd path]")},&AkaFileCmd::cd},
    {{"help", QObject::tr("List all available commands.")},&AkaBasicCmd::help},
    {{"exit", "ex", "e", QObject::tr("Exit Akasha Terminal.")},&AkaBasicCmd::exit},
    {{"ls", QObject::tr("List all files and dirs on current path.[ls path]")},&AkaFileCmd::ls},
    {{"resize", QObject::tr("Resize window.[resize width height]")},&AkaDisplayCmd::resize},
    {{"refont", QObject::tr("Set font.[refont (size) (fontname)]")},&AkaDisplayCmd::refont},
    {{"mkdir", "md", QObject::tr("Create directory.[mkdir path]")},&AkaFileCmd::mkdir},
    {{"rm", QObject::tr("Remove directory or file.[rm -f filepath] or [rm -d dirpath]")},&AkaFileCmd::rm},
    {{"mkf", "mf", QObject::tr("Create file.[mkf path (content)]")},&AkaFileCmd::mkf},
    {{"cp", "copy", QObject::tr("Copy file or directory to another path.[cp frompath topath]")},&AkaFileCmd::cp},
    {{"cls", QObject::tr("Clear the screen.")},&AkaDisplayCmd::cls},
    {{"mv", "move", QObject::tr("Move file or directory to another path.[mv frompath topath]")},&AkaFileCmd::mv},
    {{"edit", "ed", QObject::tr("Edit file content.[edit filepath]")},&AkaFileCmd::edit}
};

#endif // AKA_COMMANDS_H
