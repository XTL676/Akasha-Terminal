#include "aka_file_system.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QDataStream>
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
        KernelManager::GetKernelManager()->PrintError("Broken data folder.", KAkaBrokenDataFolder);
        return false;
    }
    return true;
}

/// 加载文件夹(读取.dir)
/// \brief AkaFileSystem::LoadDir
/// \param dirPath 要加载的文件夹的路径(以"/"开头)
/// \return 文件夹对象
///
Directory *AkaFileSystem::LoadDir(QString dirPath)
{
    if(dirPath == "/") return RootDirectory_;

    QString fullPath = RootDirPath_ + dirPath + ".dir";
    aka::PathReplace(fullPath);

    QFile dir(fullPath);
    if(!dir.exists())
    {
        KernelManager::GetKernelManager()->PrintError("Directory not exists.", KAkaInvalidPath);
        return nullptr;
    }

    // 读取.dir文件
    Directory inDir;
    QFile f(fullPath);
    f.open(QIODevice::ReadOnly);
    // 反序列化
    QDataStream in(&f);
    in >> inDir;
    f.close();
    return new Directory(inDir);
}

/// 加载文件夹(读取.dat)
/// \brief AkaFileSystem::LoadFile
/// \param filePath 要加载的文件的路径(以"/"开头)
/// \return 文件对象
///
File AkaFileSystem::LoadFile(QString filePath)
{
    QString fullPath = RootDirPath_ + filePath + ".dat";
    aka::PathReplace(fullPath);

    File inFile;

    QFile file(fullPath);
    if(!file.exists())
    {
        KernelManager::GetKernelManager()->PrintError("File data not exists.", KAkaInvalidPath);
        return inFile;
    }

    // 读取.dat文件
    QFile f(fullPath);
    f.open(QIODevice::ReadOnly);
    // 反序列化
    QDataStream in(&f);
    in >> inFile;
    f.close();
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
        f.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QDataStream out(&f);
        out << *fileObj;
        f.close();
    }
    else if(dynamic_cast<Directory*>(file) != NULL)
    {
        // 为Directory类
        outputDatFullPath += ".dir";
        Directory* dirObj = (Directory*)file;
        // 序列化Directory对象
        QFile f(outputDatFullPath);
        f.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QDataStream out(&f);
        out << *dirObj;
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
        KernelManager::GetKernelManager()->PrintError("Directory path(" + path + ") not exists.", KAkaInvalidPath);
        return false;
    }

    if(QDir(dirPath + name).exists())
    {
        KernelManager::GetKernelManager()->PrintError("Directory already exists.", KAkaFileAlreadyExists);
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
        Directory dir = *LoadDir("/" + dirPath_s.join("/"));
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
        KernelManager::GetKernelManager()->PrintError("Invalid directory path \"/\".", KAkaInvalidPath);
        return false;
    }

    QString fullPath = RootDirPath_ + path;
    if(!QFile(fullPath + ".dir").exists())
    {
        KernelManager::GetKernelManager()->PrintError("Directory not exists.", KAkaInvalidPath);
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
    Directory dir = *LoadDir(parentDirAtPath + "/" + parentDirName);

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
    Directory dir = *LoadDir(ParentDirAtPath + "/" + ParentDirName);

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
        KernelManager::GetKernelManager()->PrintError("File not exists.", KAkaInvalidPath);
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
        Directory dirObj = *LoadDir(parentDirPath);
        dirObj.RemoveSubFile(fullName);
        GenFileData(&dirObj, RootDirPath_ + parentDirAtPath);
    }

    // 删除文件
    QFile f(RootDirPath_ + parentDirPath + "/" + fullName + ".dat");
    f.remove();

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

    Directory* dir = LoadDir("/" + list.join("/"));
    if(dir == nullptr)
        return false;

    CurrentPath_ = "/" + list.join("/");
    CurrentDirectory_ = dir;

    // 设置输出接口的头显示
    ((AkaPlainTextEdit*)KernelManager::GetKernelManager()->GetMainEditArea())
            ->SetConsoleHead(aka::KAkaConselDefaultUser, CurrentPath_);

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
    for(auto file:QDir(RootDirPath_).entryInfoList())
    {
        QString name = file.baseName();
        QStringList list = file.fileName().split(".");
        list.removeAll("");
        if(list.length() > 1)
        {
            list.pop_back();
            name = list.join(".");
        }

        if(file.suffix() == "dir")
            RootDirectory_->AddSubFolder(name);
        else if(file.suffix() == "dat")
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
