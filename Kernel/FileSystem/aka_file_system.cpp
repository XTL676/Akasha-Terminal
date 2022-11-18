#include "aka_file_system.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QStringBuilder>
#include "Terminal/UI/aka_plain_text_edit.h"
#include "directory.h"
#include "file.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Kernel/kernel_manager.h"
#include "aka_global.h"

const QString AkaFileSystem::GetRootDirPath()
{
    return RootDirPath_;
}

Directory *AkaFileSystem::GetRootDirectory()
{
    return RootDirectory_;
}

const QString AkaFileSystem::GetCurrentDirPath()
{
    return CurrentPath_;
}

Directory *AkaFileSystem::GetCurrentDir()
{
    return CurrentDirectory_;
}

bool AkaFileSystem::DetectDataFolder()
{
    // 获取程序当前运行目录路径
    QDir root(RootDirPath_);
    if(!root.exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Broken data folder."), KAkaBrokenDataFolder);
        return false;
    }
    return true;
}

/// 加载文件夹(读取.dir)
/// \brief AkaFileSystem::LoadDir
/// \param dirPath 要加载的文件夹的路径(以"/"开头)
/// \return 文件夹对象
///
Directory AkaFileSystem::LoadDir(QString dirPath, bool &success)
{
    if(dirPath == "/") return *RootDirectory_;

    QString fullPath = RootDirPath_ + dirPath + ".dir";
    aka::PathReplace(fullPath);

    QFile dir(fullPath);
    if(!dir.exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory not exists."), KAkaInvalidPath);
        success = false;
        return Directory();
    }

    // 读取.dir文件
    Directory inDir;
    QFile f(fullPath);
    f.open(QIODevice::ReadOnly);
    // 反序列化
    QTextStream in(&f);
//    in >> inDir;
    inDir.Deserialize(in);
    f.close();

    success = true;
    return inDir;
}

/// 加载文件(读取.dat)
/// \brief AkaFileSystem::LoadFile
/// \param filePath 要加载的文件的路径(以"/"开头)
/// \return 文件对象
///
File AkaFileSystem::LoadFile(QString filePath, bool &success)
{
    QString fullPath = RootDirPath_ + filePath + ".dat";
    aka::PathReplace(fullPath);

    File inFile;

    QFile file(fullPath);
    if(!file.exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("File data not exists."), KAkaInvalidPath);
        success = false;
        return inFile;
    }

    // 读取.dat文件
    QFile f(fullPath);
    f.open(QIODevice::ReadOnly);
    // 反序列化
    QTextStream in(&f);
//    in >> inFile;
    inFile.Deserialize(in);
    f.close();

    success = true;
    return inFile;
}

void AkaFileSystem::GenFileData(BaseFile *file, QString path)
{
    QString outputDatFullPath = path + "/" + file->GetName();
    // 判断具体为哪个子类
    if(dynamic_cast<File*>(file) != NULL)
    {
        // 为File类
        File* fileObj = (File*)file;
        outputDatFullPath += !fileObj->GetSuffix().isEmpty() ? "." + fileObj->GetSuffix() + ".dat" : ".dat";
        // 序列化File对象
        QFile f(outputDatFullPath);
        if(f.exists()) f.remove();
        f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        QTextStream out(&f);
//        out << *fileObj;
        fileObj->Serialize(out);
        f.close();
    }
    else if(dynamic_cast<Directory*>(file) != NULL)
    {
        // 为Directory类
        outputDatFullPath += ".dir";
        Directory* dirObj = (Directory*)file;
        // 序列化Directory对象
        QFile f(outputDatFullPath);
        if(f.exists()) f.remove();
        f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        QTextStream out(&f);
//        out << *dirObj;
        dirObj->Serialize(out);
        f.close();
    }
}

/// 创建文件夹
/// \brief AkaFileSystem::CreateDict
/// \param path 在哪个目录下创建
/// \param name 文件夹的名称
/// \return 是否创建成功
///
bool AkaFileSystem::CreateDir(QString path, QString name)
{
    aka::PathReplace(path);

    QString dirPath = RootDirPath_ + path + "/";  // 如果不以/开头，则默认在根目录下创建文件夹

    if(!QDir(dirPath).exists())
    {
        KernelManager::GetKernelManager()->PrintError(
                    QObject::tr("Directory path") + "(" + path + ")" + QObject::tr(" not exists."), KAkaInvalidPath);
        return false;
    }

    if(QDir(dirPath + name).exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory already exists."), KAkaFileAlreadyExists);
        return false;
    }

    if(!QDir(dirPath).mkdir(name))
        return false;

    QString ParentFolderName;
    if(path == "/")
    {
        ParentFolderName = "/";
        KernelManager::GetKernelManager()->GetFileSystem()->GetRootDirectory()->AddSubFolder(name);
    }
    else
    {
        QStringList dirPath_s = path.split("/");
        dirPath_s.removeAll("");
        ParentFolderName = dirPath_s.back();
        // 获取父类的.dir文件
        bool b;
        Directory dir = LoadDir("/" + dirPath_s.join("/"), b);
        dir.AddSubFolder(name);
        // 刷新父类的.dir文件
        dirPath_s.pop_back();
        GenFileData(&dir, RootDirPath_ + "/" + dirPath_s.join("/"));
    }
    Directory dir(ParentFolderName, name, "SYSTEM");
    KernelManager::GetKernelManager()->GetFileSystem()->GenFileData(&dir, RootDirPath_ + "/" + path);
    return true;
}

/// 删除文件夹
/// \brief AkaFileSystem::DeleteDir
/// \param path 文件夹路径("/")
/// \return 是否成功删除
///
bool AkaFileSystem::DeleteDir(QString path)
{
    // 禁止删除根目录
    if(path == "/")
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Invalid directory path") + " \"/\".", KAkaInvalidPath);
        return false;
    }

    QString fullPath = RootDirPath_ + path;
    if(!QFile(fullPath + ".dir").exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory not exists."), KAkaInvalidPath);
        return false;
    }

    QStringList list = path.split("/");
    list.removeAll("");
    QString deleteDirName = list.back();
    list.pop_back();
    if(list.isEmpty())
    {
        // 删除记录
        KernelManager::GetKernelManager()->GetFileSystem()->GetRootDirectory()->RemoveSubFolder(deleteDirName);
        // 删除文件夹
        QDir(fullPath).removeRecursively();
        // 删除.dir文件
        QFile(RootDirPath_ + "/" + deleteDirName + ".dir").remove();
        return true;
    }
    QString parentDirName = list.back();
    QString parentDirPath = "/" + list.join("/"); // 父级目录路径
    list.pop_back();
    QString parentDirAtPath = "/" + list.join("/"); // 父级目录所在的目录路径

    // 反序列化父级文件夹
    bool b;
    Directory dir = LoadDir(parentDirAtPath + "/" + parentDirName, b);

    dir.RemoveSubFolder(deleteDirName);
    GenFileData(&dir, RootDirPath_ + parentDirAtPath);
    QDir(fullPath).removeRecursively();
    QFile(RootDirPath_ + parentDirPath + "/" + deleteDirName + ".dir").remove();
    return true;
}

/// 创建文件
/// \brief AkaFileSystem::CreateFile
/// \param path 文件所在路径("/")
/// \param name 文件名称(不带后缀名)
/// \param suffix 文件后缀名
/// \return 是否成功创建文件
///
bool AkaFileSystem::CreateFileA(QString path, QString name, QString suffix, QString content)
{
    if(path == "/")
    {
        // 向父级文件夹添加记录
        if(!suffix.isEmpty())
            RootDirectory_->AddSubFile(name + "." + suffix);
        else
            RootDirectory_->AddSubFile(name);

        File file("/", name, suffix, "SYSTEM", content);
        GenFileData(&file, RootDirPath_ + path);
        return true;
    }

    QStringList list = path.split("/");
    list.removeAll("");
    QString ParentDirName = list.back();
    list.pop_back();
    QString ParentDirAtPath = "/" + list.join("/");

    // 反序列化父级文件夹
    bool b;
    Directory dir = LoadDir(ParentDirAtPath + "/" + ParentDirName, b);
    if(!b) return false;

    // 向父级文件夹添加记录
    if(!suffix.isEmpty())
        dir.AddSubFile(name + "." + suffix);
    else
        dir.AddSubFile(name);
    GenFileData(&dir, RootDirPath_ + ParentDirAtPath);

    //序列化文件
    File file(ParentDirName, name, suffix, "SYSTEM", content);
    GenFileData(&file, RootDirPath_ + path);

    return true;
}

/// 删除文件
/// \brief AkaFileSystem::DeleteFileA
/// \param fullPath 文件的全路径，包括文件名称和后缀名("/")
/// \return 是否成功删除文件
///
bool AkaFileSystem::DeleteFileA(QString fullPath)
{
    if(!QFile(RootDirPath_ + fullPath + ".dat").exists())
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("File not exists."), KAkaInvalidPath);
        return false;
    }

    QStringList list = fullPath.split("/");
    list.removeAll("");
    QString fullName = list.back();
    QString parentDirPath;

    if(list.length() == 1)
    {
        // 删除根记录
        RootDirectory_->RemoveSubFile(fullName);
        parentDirPath = "/";
    }
    else
    {
        list.pop_back();
        parentDirPath = "/" + list.join("/");
        list.pop_back();
        QString parentDirAtPath = list.join("/");

        // 删除记录
        bool b;
        Directory dirObj = LoadDir(parentDirPath, b);
        dirObj.RemoveSubFile(fullName);
        GenFileData(&dirObj, RootDirPath_ + parentDirAtPath);
    }

    // 删除文件
    QFile f(RootDirPath_ + parentDirPath + "/" + fullName + ".dat");
    f.remove();

    return true;
}

/// 拷贝文件或者文件夹
/// \brief AkaFileSystem::Copy
/// \param from 从哪个路径("/")
/// \param to   到哪个路径("/")
/// \param DeleteSrc 是否删除原始文件/文件夹
/// \return 是否成功拷贝
///
bool AkaFileSystem::Copy(QString from, QString to, bool DeleteSrc)
{
    /**
     * to为拷贝到哪个路径，
     *      如果以/结尾，则是拷贝到目录下；(/from/dir1, /to/ ---> /to/dir1)
     *      否则将其命名后拷贝到最近一个目录下(/from/dir1, /to ---> /to)
     */
    if(from == "/")
    {
        KernelManager::GetKernelManager()->PrintError(QObject::tr("Root dir can't copy."), KAkaInvalidPath);
        return false;
    }

    // 判断是文件还是文件夹
    // 只要路径后缀不带"/"的一律认为是文件反之亦然
    if(from.endsWith("/"))
    {
        // 文件夹
        bool success;
        Directory from_dir = LoadDir(from.mid(0, from.length()-1), success);
        if(!success) return false;

        // 是否重命名文件
        if(to.endsWith("/"))
        {
            // 拷贝到目标目录下

            // 检测目标路径是否已经存在同名文件夹
            if(QFile(RootDirPath_ + to + from_dir.GetName() + ".dir").exists())
            {
                // TODO 询问是否覆盖
                KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory already exists."), KAkaFileAlreadyExists);
                return false;
            }


            if(to == "/")
            {
                // 重设父级文件夹名称
                from_dir.SetParentFolderName("/");

                // 向父级文件夹添加记录
                RootDirectory_->AddSubFolder(from_dir.GetName());
            }
            else
            {
                // 重设父级文件夹名称
                from_dir.SetParentFolderName(GetParentDirNameFromPath(to));

                // 向目标文件夹添加记录
                Directory to_dir = LoadDir(to.mid(0, to.length()-1), success);
                if(!success) return false;

                to_dir.AddSubFolder(from_dir.GetName());
                GenFileData(&to_dir, RootDirPath_ + GetParentDirAtPath(to));
            }

            // 创建记录
            GenFileData(&from_dir, RootDirPath_ + to);
            // 拷贝物理文件夹
            CopyPhysicalDir(RootDirPath_ + from.mid(0, from.length()-1), RootDirPath_ + to + from_dir.GetName(), true);
        }
        else
        {
            // 重命名禁词判断
            for(QString s:aka::KAkaFDNameForbiddenSymbols)
            {
                if(GetParentDirNameFromPath(to).contains(s))
                {
                    KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory name can't be any of") + " \"" +
                                                                  aka::KAkaFDNameForbiddenSymbols.join(" ") + "\".", KAkaInvalidName);
                    return false;
                }
            }
            // 命名后拷
            from_dir.SetName(GetParentDirNameFromPath(to));

            // 检测目标路径是否已经存在同名文件夹
            if(QFile(RootDirPath_ + GetParentDirAtPath(to) + "/" + from_dir.GetName() + ".dir").exists())
            {
                // TODO 询问是否覆盖
                KernelManager::GetKernelManager()->PrintError(QObject::tr("Directory already exists."), KAkaFileAlreadyExists);
                return false;
            }

            if(to == "/" || GetParentDirAtPath(to) == "/")
            {
                // 重设父级文件夹名称
                from_dir.SetParentFolderName("/");

                // 向父级文件夹添加记录
                RootDirectory_->AddSubFolder(from_dir.GetName());
            }
            else
            {
                // 重设父级文件夹名称
                from_dir.SetParentFolderName(GetParentDirNameFromPath(GetParentDirAtPath(to)));

                // 向目标文件夹添加记录
                Directory to_dir = LoadDir(GetParentDirAtPath(to), success);
                if(!success) return false;

                to_dir.AddSubFolder(from_dir.GetName());
                GenFileData(&to_dir, RootDirPath_ + GetParentDirAtPath(GetParentDirAtPath(to)));
            }

            // 创建记录
            GenFileData(&from_dir, RootDirPath_ + GetParentDirAtPath(to));
            // 拷贝物理文件夹
            CopyPhysicalDir(RootDirPath_ + from.mid(0, from.length()-1),
                            RootDirPath_ + GetParentDirAtPath(to) + "/" + from_dir.GetName(), true);
        }

        // 删除原始文件夹
        if(DeleteSrc)
        {
            if(GetParentDirAtPath(from) == "/")
                RootDirectory_->RemoveSubFolder(GetParentDirNameFromPath(from));
            else
            {
                // 向父级文件夹删除记录
                Directory src_parent_dir = LoadDir(GetParentDirAtPath(GetParentDirAtPath(from)) + "/" +
                                                   GetParentDirNameFromPath(GetParentDirAtPath(from)), success);
                if(!success) return false;

                src_parent_dir.RemoveSubFolder(GetParentDirNameFromPath(from));
                GenFileData(&src_parent_dir, RootDirPath_ + GetParentDirAtPath(GetParentDirAtPath(from)) + "/");
            }
            // 删除文件夹记录
            QFile(RootDirPath_ + from.mid(0, from.length()-1) + ".dir").remove();
            // 删除物理文件夹
            QDir(RootDirPath_ + from).removeRecursively();
        }
    }
    else
    {
        // 文件
        bool success;
        File from_file = LoadFile(from, success);
        if(!success) return false;
        File src_file = LoadFile(from, success);

        if(to.endsWith("/"))
        {
            // 拷贝到目标目录下
            QString fileName = from_file.GetSuffix().isEmpty() ?
                        from_file.GetName() : from_file.GetName() + "." + from_file.GetSuffix();

            // 检测目标路径是否已经存在同名文件夹
            if(QFile(RootDirPath_ + to + fileName + ".dat").exists())
            {
                // TODO 询问是否覆盖
                KernelManager::GetKernelManager()->PrintError(QObject::tr("File already exists."), KAkaFileAlreadyExists);
                return false;
            }

            if(to == "/")
            {
                // 重设父级文件夹名称
                from_file.SetParentFolderName("/");

                // 向父级文件夹添加记录
                RootDirectory_->AddSubFile(fileName);
            }
            else
            {
                // 重设父级文件夹名称
                from_file.SetParentFolderName(GetParentDirNameFromPath(to));

                // 向目标文件夹添加记录
                Directory to_dir = LoadDir(to.mid(0, to.length()-1), success);
                if(!success) return false;

                to_dir.AddSubFile(fileName);
                GenFileData(&to_dir, RootDirPath_ + GetParentDirAtPath(to));
            }

            // 创建记录
            GenFileData(&from_file, RootDirPath_ + to);
        }
        else
        {
            // 重命名禁词判断
            for(QString s:aka::KAkaFDNameForbiddenSymbols)
            {
                if(GetParentDirNameFromPath(to).contains(s))
                {
                    KernelManager::GetKernelManager()->PrintError(QObject::tr("File name can't be any of") + " \"" +
                                                                  aka::KAkaFDNameForbiddenSymbols.join(" ") + "\".", KAkaInvalidName);
                    return false;
                }
            }

            // 命名后拷
            if(GetParentDirNameFromPath(to).contains("."))
            {
                QStringList l = GetParentDirNameFromPath(to).split(".");
                l.removeAll("");
                if(l.length() == 2)
                {
                    from_file.SetName(l.front());
                    from_file.SetSuffix(l.back());
                }
                else
                {
                    // 如果文件以"."开头，则是一个没有名字只有后缀的文件
                    if(GetParentDirNameFromPath(to).startsWith("."))
                    {
                        from_file.SetName("");
                        from_file.SetSuffix(GetParentDirNameFromPath(to));
                    }
                    from_file.SetSuffix(l.back());
                    l.pop_back();
                    from_file.SetName(l.join("."));
                }
            }
            else
            {
                from_file.SetName(GetParentDirNameFromPath(to));
                from_file.SetSuffix("");
            }

            QString fileName = from_file.GetSuffix().isEmpty() ?
                        from_file.GetName() : from_file.GetName() + "." + from_file.GetSuffix();

            // 检测目标路径是否已经存在同名文件夹
            if(QFile(RootDirPath_ + GetParentDirAtPath(to) + "/" + fileName + ".dat").exists())
            {
                // TODO 询问是否覆盖
                KernelManager::GetKernelManager()->PrintError(QObject::tr("File already exists."), KAkaFileAlreadyExists);
                return false;
            }

            if(to == "/" || GetParentDirAtPath(to) == "/")
            {
                // 重设父级文件夹名称
                from_file.SetParentFolderName("/");

                // 向父级文件夹添加记录
                RootDirectory_->AddSubFile(fileName);
            }
            else
            {
                // 重设父级文件夹名称
                from_file.SetParentFolderName(GetParentDirNameFromPath(GetParentDirAtPath(to)));

                // 向目标文件夹添加记录
                Directory to_dir = LoadDir(GetParentDirAtPath(to), success);
                if(!success) return false;

                to_dir.AddSubFile(fileName);
                GenFileData(&to_dir, RootDirPath_ + GetParentDirAtPath(GetParentDirAtPath(to)));
            }

            // 创建记录
            GenFileData(&from_file, RootDirPath_ + GetParentDirAtPath(to));
        }

        // 删除原始文件
        if(DeleteSrc)
        {
            if(src_file.GetParentFolderName() == "/")
                RootDirectory_->RemoveSubFile(src_file.GetSuffix().isEmpty() ?
                                                    src_file.GetName() : src_file.GetName() + "." + src_file.GetSuffix());
            else
            {
                // 向父级文件夹删除记录
                Directory src_parent_dir = LoadDir(GetParentDirAtPath(GetParentDirAtPath(from)) + "/" +
                                                   GetParentDirNameFromPath(GetParentDirAtPath(from)), success);
                if(!success) return false;

                src_parent_dir.RemoveSubFile(src_file.GetSuffix().isEmpty() ?
                                                 src_file.GetName() : src_file.GetName() + "." + src_file.GetSuffix());
                GenFileData(&src_parent_dir, RootDirPath_ + GetParentDirAtPath(GetParentDirAtPath(from)) + "/");
            }
            // 删除文件记录
            QFile(RootDirPath_ + from + ".dat").remove();
        }
    }
    return true;
}

/// 更换目录
/// \brief AkaFileSystem::ChangeDir
/// \param path 要切换的目录路径("/")
/// \return
///
bool AkaFileSystem::ChangeDir(QString path)
{
    if(path == "/")
    {
        CurrentPath_ = "/";
        CurrentDirectory_ = RootDirectory_;
        // 设置输出接口的头显示
        ((AkaPlainTextEdit*)KernelManager::GetKernelManager()->GetMainEditArea())
                ->SetConsoleHead(aka::KAkaConselDefaultUser, "/");
        return true;
    }

    aka::PathReplace(path);

    // 去除path末尾的"/"
    QStringList list = path.split("/");
    list.removeAll("");

    bool b;
    Directory dir = LoadDir("/" + list.join("/"), b);
    if(!b) return false;

    CurrentPath_ = "/" + list.join("/");
    CurrentDirectory_ = &dir;

    // 设置输出接口的头显示
    ((AkaPlainTextEdit*)KernelManager::GetKernelManager()->GetMainEditArea())
            ->SetConsoleHead(aka::KAkaConselDefaultUser, CurrentPath_);

    return true;
}

/// 列出某文件夹下的所有文件夹和文件
/// \brief AkaFileSystem::List
/// \param path 哪个文件夹下
/// \return 是否成功
///
bool AkaFileSystem::List(QString path)
{
    aka::PathReplace(path);
    QStringList dirs;
    QStringList files;

    // 根目录
    if(path == "/")
    {
        dirs = RootDirectory_->GetSubFolderNames();
        files = RootDirectory_->GetSubFileNames();
    }
    else
    {
        // 其它目录
        QStringList l = path.split("/");
        l.removeAll("");
        bool b;
        Directory dir = LoadDir("/" + l.join("/"), b);
        if(!b) return false;

        dirs = dir.GetSubFolderNames();
        files = dir.GetSubFileNames();
    }

    int col = aka::KAkaLSDefaultDisplayColumn; // 不能小于3
    // 输出
    int col_t = 0;
    QString str;

    // 输出文件夹
    for (int i = 0; i < dirs.length(); i++) {
        if(col_t != 0 && col_t%col == 0)
        {
            KernelManager::GetKernelManager()->Print(str, aka::KAkaFolderDefaultDisplayColor);
            str = "";
            col_t = 0;
        }
        str = str % dirs[i] % aka::KAkaLSSplitSymbol;
        col_t++;
    }
    if(!str.isEmpty())
    {
        QStringList l = str.split(aka::KAkaLSSplitSymbol);
        l.removeAll("");
        KernelManager::GetKernelManager()->Print(l.join(aka::KAkaLSSplitSymbol), aka::KAkaFolderDefaultDisplayColor);
    }
    str = "";
    int col_ = col_t; // 有几个文件夹是在多出来一行里的

    // 输出文件
    int row = 0; // 有几行包含了文件
    for (int i = 0; i < files.length(); i++) {
        if(col_t != 0 && col_t%col == 0)
        {
            KernelManager::GetKernelManager()->Print(str);
            str = "";
            col_t = 0;
            row++;
        }
        str = str % files[i] % aka::KAkaLSSplitSymbol;
        col_t++;
    }
    if(!str.isEmpty())
    {
        QStringList l = str.split(aka::KAkaLSSplitSymbol);
        l.removeAll("");
        KernelManager::GetKernelManager()->Print(l.join(aka::KAkaLSSplitSymbol));
    }

    if(col_ != 0 && dirs.length() >= 1 && files.length() >= 1)
    {
        // 移动光标到行首
        QTextCursor tc = KernelManager::GetKernelManager()->GetMainEditArea()->textCursor();
        tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, files.join(aka::KAkaLSSplitSymbol).length() + row);
        KernelManager::GetKernelManager()->GetMainEditArea()->setTextCursor(tc);
        // 删除前面一个字符
        KernelManager::GetKernelManager()->GetMainEditArea()->textCursor().deletePreviousChar();

        KernelManager::GetKernelManager()->GetMainEditArea()->insertPlainText(aka::KAkaLSSplitSymbol);

        // 光标移到最末尾
        KernelManager::GetKernelManager()->GetMainEditArea()->moveCursor(QTextCursor::End);
    }
    return true;
}

/// 修改文件内容
/// \brief AkaFileSystem::ModifyFileContent
/// \param path 文件路径("/")
/// \param content 修改后的内容
/// \return
///
bool AkaFileSystem::ModifyFileContent(QString path, QString content)
{
    aka::PathReplace(path);

    bool success;
    File src = LoadFile(path, success);
    if(!success) return false;

    src.SetContent(content); // 设置文件内容
    GenFileData(&src, RootDirPath_ + GetParentDirAtPath(path)); // 重新生成并覆盖源文件

    return true;
}

/// 查看文件内容
/// \brief AkaFileSystem::ViewFileContent
/// \param path 文件路径("/")
/// \return 是否查看成功
///
bool AkaFileSystem::ViewFileContent(QString path)
{
    aka::PathReplace(path);

    bool success;
    File src = LoadFile(path, success);
    if(!success) return false;

    KernelManager::GetKernelManager()->Print(src.GetContent());

    return true;
}

QString AkaFileSystem::GetParentDirNameFromPath(QString path)
{
    aka::PathReplace(path);
    if(path == "/") return path;
    QStringList list = path.split("/");
    list.removeAll("");
    return list.back();
}

QString AkaFileSystem::GetParentDirAtPath(QString path)
{
    aka::PathReplace(path);
    if(path == "/") return path;
    QStringList list = path.split("/");
    list.removeAll("");
    // 根目录
    if(list.length() == 1) return "/";
    list.pop_back();
    return "/" + list.join("/");
}

/// 拷贝物理文件夹
/// \brief AkaFileSystem::CopyPhysicalDir
/// \param fromDir 从哪个文件夹路径
/// \param toDir 目标文件夹路径
/// \param coverFileIfExist 当允许覆盖操作时，将旧文件进行删除操作
/// \return
///
bool AkaFileSystem::CopyPhysicalDir(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists())
    {
        // 如果目标目录不存在，则进行创建
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir())
        {
            // 当为目录时，递归的进行copy
            if(!CopyPhysicalDir(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), coverFileIfExist))
                return false;
        }
        else
        {
            // 当允许覆盖操作时，将旧文件进行删除操作
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName()))
                targetDir.remove(fileInfo.fileName());

            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())))
                return false;
        }
    }
    return true;
}

void AkaFileSystem::Initialize()
{
    RootDirPath_ = QCoreApplication::applicationDirPath() + "/" +
                aka::KAkaDataFolderName + "/" +
                aka::KAkaFileSystemRootFolderName;

    // 设置根文件夹("/")对象
    RootDirectory_ = new Directory("", "/", "SYSTEM");
    CurrentDirectory_ = RootDirectory_;

    // 读取根目录
    for(QFileInfo file:QDir(RootDirPath_).entryInfoList())
    {
        if(file.isDir()) continue;
        QString name = file.baseName();
        QString suffix = file.suffix();
        QStringList list = file.fileName().split(".");
        list.removeAll("");

        if(list.length() > 1)
        {
            suffix = list.back();
            list.pop_back();
            name = list.join(".");
        }

        if(suffix == "dir")
            RootDirectory_->AddSubFolder(name);
        else if(suffix == "dat")
            RootDirectory_->AddSubFile(name);
    }
}

AkaFileSystem::AkaFileSystem()
{
    Initialize();
}

AkaFileSystem::~AkaFileSystem()
{
}
