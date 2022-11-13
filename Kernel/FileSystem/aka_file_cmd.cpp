#include "aka_file_cmd.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Kernel/kernel_manager.h"
#include "aka_global.h"
#include <QFile>

int AkaFileCmd::mkdir(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError("No or missing parameters.Should be [mkdir path]", KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError("Too many parameters.There should only be one path.", KAkaTooManyParameters);
        return 1;
    }

    QStringList list = args[1].split("/");
    list.removeAll("");
    QString name = list.back();
    list.pop_back();
    if(KernelManager::GetKernelManager()->GetFileSystem()->CreateDir("/" + list.join("/"), name))
        KernelManager::GetKernelManager()->Print("Create directory \"" + name + "\" successfully.(" + args[1] + ")", QColor("green"));

    return 1;
}

int AkaFileCmd::rm(QStringList args)
{
    if(args.length() < 3)
    {
        KernelManager::GetKernelManager()->PrintError("No or missing parameters.Should be [rm -f filepath] or [rm -d dirpath]", KAkaMissingParameter);
        return 1;
    }

    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError("Too many parameters.There should only be one file or dir path.", KAkaTooManyParameters);
        return 1;
    }

    QString path = args[2];
    if(!args[2].startsWith("/")) path = "/" + path; // 如果不以/开头，则默认删除根目录下的文件夹或文件

    if(args[1] == "-f")
    {
        // 删除文件
        if(KernelManager::GetKernelManager()->GetFileSystem()->DeleteFileA(path))
            KernelManager::GetKernelManager()->Print("Delete directory (" + path + ") successfully.", QColor("green"));
    }
    else if(args[1] == "-d")
    {
        // 删除文件夹
        if(KernelManager::GetKernelManager()->GetFileSystem()->DeleteDir(path))
            KernelManager::GetKernelManager()->Print("Delete directory (" + path + ") successfully.", QColor("green"));
    }
    else
        KernelManager::GetKernelManager()->PrintError("Invalid parameter.Should be -f for file or -d for dir.", KAkaInvalidParameter);

    return 1;
}

int AkaFileCmd::mkf(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError("No or missing parameters.Should be [mkf path]", KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError("Too many parameters.", KAkaTooManyParameters);
        return 1;
    }

    // 文件内容，没有参数默认无内容
    QString content = args.length() == 3 ? args[2] : "";

    // 格式化路径变量
    QString args1 = args[1];
    aka::PathReplace(args1);

    // 分析文件路径
    QStringList t_list1 = args1.split("/");
    t_list1.removeAll("");

    QString fileFullName = t_list1.back(); // name + suffix
    QString name = "";
    QString suffix = "";
    QString path = "";
    if(!fileFullName.contains("."))
    {
        // 文件没有后缀名
        name = fileFullName;
    }
    else
    {
        // 文件有后缀名
        QStringList t_list2 = fileFullName.split(".");
        t_list2.removeAll("");
        suffix = t_list2.back();

        t_list2.pop_back();
        name = t_list2.join(".");
    }

    t_list1.pop_back();
    path = "/" + t_list1.join("/");

    // 创建文件.dat
    if(KernelManager::GetKernelManager()->GetFileSystem()->CreateFileA(path, name, suffix, content))
        KernelManager::GetKernelManager()->Print("Create directory \"" + fileFullName + "\" successfully.(" + args[1] + ")", QColor("green"));

    return true;
}
