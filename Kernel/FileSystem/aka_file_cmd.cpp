#include "aka_file_cmd.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Kernel/kernel_manager.h"
#include "Terminal/UI/aka_plain_text_edit.h"
#include "aka_global.h"
#include <QFile>

int AkaFileCmd::mkdir(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("No or missing parameters.Should be [mkdir path]"), KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Too many parameters.There should only be one path."), KAkaTooManyParameters);
        return 1;
    }

    QStringList list = args[1].split("/");
    list.removeAll("");
    QString name = list.back();
    // 判断名称是否含有禁词
    for(QString s:aka::KAkaFDNameForbiddenSymbols)
    {
        if(name.contains(s))
        {
            KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory name can't be any of") + " \"" +
                                                          aka::KAkaFDNameForbiddenSymbols.join(" ") + "\".", KAkaInvalidName);
            return 1;
        }
    }
    list.pop_back();

    QString CurrentDirPath = KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath();
    CurrentDirPath = CurrentDirPath.endsWith("/") ? CurrentDirPath : CurrentDirPath + "/";

    if(KernelManager::GetKernelManager()->GetFileSystem()->CreateDir(CurrentDirPath + list.join("/"), name))
        KernelManager::GetKernelManager()->Print(
                    QObject::tr("Create directory") + " \"" + name + "\" " + QObject::tr("successfully.") +
                    "(" + args[1] + ")", QColor("green"));

    return 1;
}

int AkaFileCmd::rm(QStringList args)
{
    if(args.length() < 3)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [rm -f filepath] or [rm -d dirpath]"), KAkaMissingParameter);
        return 1;
    }

    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Too many parameters.There should only be one file or dir path."), KAkaTooManyParameters);
        return 1;
    }

    QString path = args[2];
    QString CurrentDirPath = KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath();
    CurrentDirPath = CurrentDirPath.endsWith("/") ? CurrentDirPath : CurrentDirPath + "/";
    if(!args[2].startsWith("/")) path = CurrentDirPath + path; // 如果不以/开头，则默认删除当前目录下的文件夹或文件

    if(args[1] == "-f")
    {
        // 删除文件
        if(KernelManager::GetKernelManager()->GetFileSystem()->DeleteFileA(path))
            KernelManager::GetKernelManager()->Print(
                        QObject::tr("Delete file") + " (" + path + ") " + QObject::tr("successfully."), QColor("green"));
    }
    else if(args[1] == "-d")
    {
        // 删除文件夹
        if(KernelManager::GetKernelManager()->GetFileSystem()->DeleteDir(path))
            KernelManager::GetKernelManager()->Print(
                        QObject::tr("Delete directory") + " (" + path + ") " + QObject::tr("successfully."), QColor("green"));
    }
    else
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Invalid parameter.Should be -f for file or -d for dir."), KAkaInvalidParameter);

    return 1;
}

int AkaFileCmd::mkf(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [mkf path]"), KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Too many parameters."), KAkaTooManyParameters);
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

    // 判断名称是否含有禁词
    for(QString s:aka::KAkaFDNameForbiddenSymbols)
    {
        if(name.contains(s))
        {
            KernelManager::GetKernelManager()->PrintError(QObject::tr("File name can't be any of") + " \"" +
                                                          aka::KAkaFDNameForbiddenSymbols.join(" ") + "\".", KAkaInvalidName);
            return 1;
        }
    }

    t_list1.pop_back();
    QString CurrentDirPath = KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath();
        CurrentDirPath = CurrentDirPath.endsWith("/") ? CurrentDirPath : CurrentDirPath + "/";
    path = CurrentDirPath + t_list1.join("/");

    // 创建文件.dat
    if(KernelManager::GetKernelManager()->GetFileSystem()->CreateFileA(path, name, suffix, content))
        KernelManager::GetKernelManager()->Print(
                   QObject::tr("Create directory") + " \"" + fileFullName + "\" " + QObject::tr("successfully.") +
                    "(" + args[1] + ")", QColor("green"));

    return 1;
}

int AkaFileCmd::cd(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [cd path]"), KAkaMissingParameter);
        return 1;
    }
    else if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Too many parameters.There should only be one path."), KAkaTooManyParameters);
        return 1;
    }

    // 只有.. ，返回上级目录
    if(args[1] == "..")
    {
        QString path;
        QStringList list = KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath().split("/");

        if(list.length() == 2)
            path = "/";
        else
        {
            list.removeAll("");
            list.pop_back();
            path = list.join("/");
        }

        KernelManager::GetKernelManager()->GetFileSystem()->ChangeDir(path);
        return 1;
    }

    KernelManager::GetKernelManager()->GetFileSystem()->ChangeDir(MakeFullPath(args[1]));

    return 1;
}

int AkaFileCmd::ls(QStringList args)
{
    if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Too many parameters.There should only be one path."), KAkaTooManyParameters);
        return 1;
    }

    // 只有一个ls，列出当前目录下的文件和文件夹
    if(args.length() == 1)
    {
        KernelManager::GetKernelManager()->GetFileSystem()->List(
                    KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath());
        return 1;
    }

    // 只有.. ，返回上级目录
    if(args[1] == "..")
    {
        QString path;
        QStringList list = KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath().split("/");

        if(list.length() == 2)
            path = "/";
        else
        {
            list.removeAll("");
            list.pop_back();
            path = list.join("/");
        }

        KernelManager::GetKernelManager()->GetFileSystem()->List(path);
        return 1;
    }
    KernelManager::GetKernelManager()->GetFileSystem()->List(MakeFullPath(args[1]));

    return true;
}

int AkaFileCmd::cp(QStringList args)
{
    if(args.length() < 3)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [cp frompath topath]"), KAkaMissingParameter);
        return 1;
    }

    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Too many parameters."), KAkaTooManyParameters);
        return 1;
    }

    if(KernelManager::GetKernelManager()->GetFileSystem()->Copy(MakeFullPath(args[1]), MakeFullPath(args[2]), false))
    {
        QString arg1 = args[1], arg2 = args[2];
        aka::PathReplace(arg1);
        aka::PathReplace(arg2);
        KernelManager::GetKernelManager()->Print(
                    QObject::tr("Copy") + " " + arg1 + " " + QObject::tr("to") +
                    " " + arg2 + " " + QObject::tr("successfully."), QColor("green"));
        return 1;
    }

    return 1;
}

int AkaFileCmd::mv(QStringList args)
{
    if(args.length() < 3)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [mv frompath topath]"), KAkaMissingParameter);
        return 1;
    }

    else if(args.length() > 3)
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Too many parameters."), KAkaTooManyParameters);
        return 1;
    }

    if(KernelManager::GetKernelManager()->GetFileSystem()->Copy(MakeFullPath(args[1]), MakeFullPath(args[2]), true))
    {
        QString arg1 = args[1], arg2 = args[2];
        aka::PathReplace(arg1);
        aka::PathReplace(arg2);
        KernelManager::GetKernelManager()->Print(
                    QObject::tr("Move") + " " + arg1 + " " + QObject::tr("to") +
                    " " + arg2 + " " + QObject::tr("successfully."), QColor("green"));
        return 1;
    }

    return 1;
}

int AkaFileCmd::edit(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [edit filepath]"), KAkaMissingParameter);
        return 1;
    }

    if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Too many parameters.There should only be one path."), KAkaTooManyParameters);
        return 1;
    }

    if(args.length() == 2)
        ((AkaPlainTextEdit*)KernelManager::GetKernelManager()->GetMainEditArea())->ShowEditFileUI(MakeFullPath(args[1]));

    return 1;
}

int AkaFileCmd::view(QStringList args)
{
    if(args.length() < 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("No or missing parameters.Should be [edit filepath]"), KAkaMissingParameter);
        return 1;
    }

    if(args.length() > 2)
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Too many parameters.There should only be one path."), KAkaTooManyParameters);
        return 1;
    }

    if(args.length() == 2)
        KernelManager::GetKernelManager()->GetFileSystem()->ViewFileContent(MakeFullPath(args[1]));

    return 1;
}

QString AkaFileCmd::MakeFullPath(QString shortPath)
{
    // 以"/"开头，完整路径
    if(shortPath.startsWith("/"))
        return shortPath;
    else
        // 无"/"开头，当前路径
        return KernelManager::GetKernelManager()->GetFileSystem()->GetCurrentDirPath() + "/" + shortPath;
}
