#include "aka_file_cmd.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "aka_file_system.h"
#include "aka_global.h"

int AkaFileCmd::mkdir(QStringList args)
{
    if(args.length() < 2)
    {
        aka::PrintError("No or missing parameters.Should be [mkdir path]", KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 2)
    {
        aka::PrintError("Too many parameters.There should only be one path.", KAkaTooManyParameters);
        return 1;
    }
    QStringList list = args[1].split("/");
    list.removeAll("");
    QString name = list.back();
    list.pop_back();
    if(AkaFileSystem::GetFileSystem()->CreateDir("/" + list.join("/"), name))
        aka::Print("Create directory \"" + name + "\" successfully.(" + args[1] + ")", QColor("green"));
    return 1;
}
