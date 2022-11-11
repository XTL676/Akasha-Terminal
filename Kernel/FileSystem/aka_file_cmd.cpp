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

int AkaFileCmd::rm(QStringList args)
{
    if(args.length() < 3)
    {
        aka::PrintError("No or missing parameters.Should be [rm -f filepath] or [rm -d dirpath]", KAkaMissingParameter);
        return 1;
    }

    else if(args.length() > 3)
    {
        aka::PrintError("Too many parameters.There should only be one file or dir path.", KAkaTooManyParameters);
        return 1;
    }

    if(args[1] == "-f")
    {
        // 删除文件
    }
    else if(args[1] == "-d")
    {
        // 删除文件夹
        QString dirPath = args[2];
        if(!args[2].startsWith("/")) dirPath = "/" + dirPath; // 如果不以/开头，则默认删除根目录下的文件夹
        if(AkaFileSystem::GetFileSystem()->DeleteDir(dirPath))
            aka::Print("Delete directory (" + dirPath + ") successfully.", QColor("green"));
    }
    else
        aka::PrintError("Invalid parameter.Should be -f for file or -d for dir.", KAkaInvalidParameter);

    return 1;
}
