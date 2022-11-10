#include "aka_basic_cmd.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Terminal/Info/aka_commands.h"
#include "aka_global.h"

int AkaBasicCmd::help(QStringList args)
{
    aka::Print("\tAkasha Terminal Commands");
    for(auto it:KBuiltin_Cmds.toStdMap())
        aka::Print(it.first.front() + "\t" + it.first.back());
    return 1;
}

int AkaBasicCmd::exit(QStringList args)
{
    return KAkaNormalExit;
}
