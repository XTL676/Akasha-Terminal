#include "aka_basic_cmd.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Terminal/Info/aka_commands.h"
#include "Kernel/kernel_manager.h"

int AkaBasicCmd::help(QStringList args)
{
    KernelManager::GetKernelManager()->Print("\t" + QObject::tr("Akasha Terminal Commands"));
    for(auto it:KBuiltin_Cmds.toStdMap())
        KernelManager::GetKernelManager()->Print(it.first.front() + "\t" + it.first.back());
    return 1;
}

int AkaBasicCmd::exit(QStringList args)
{
    return KAkaNormalExit;
}
