#include "aka_file_system.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include "directory.h"
#include "file.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "aka_global.h"

// 初始化静态成员
AkaFileSystem *AkaFileSystem::Instance_ = nullptr;

AkaFileSystem *AkaFileSystem::GetFileSystem()
{
    if(Instance_ == nullptr)
        Instance_ = new AkaFileSystem();
    return Instance_;
}

const QString AkaFileSystem::GetRootDirPath()
{
    return RootDirPath_;
}

Directory *AkaFileSystem::GetRootDirectory()
{
    return RooDirectory_;
}

bool AkaFileSystem::Save()
{
}

bool AkaFileSystem::DetectDataFolder()
{
    // 获取程序当前运行目录路径
    QDir root(RootDirPath_);
    if(!root.exists())
    {
        aka::PrintError("Broken data folder.", KAkaBrokenDataFolder);
        return false;
    }
    return true;
}

/// 加载文件夹(读取.dir)
/// \brief AkaFileSystem::LoadDir
/// \param dirPath 要加载的文件夹的路径(以"/"开头)
/// \return 文件夹对象
///
Directory AkaFileSystem::LoadDir(QString dirPath)
{
    QString fullPath = RootDirPath_ + "/" + dirPath + ".dir";
    aka::PathReplace(fullPath);

    Directory inDir;

    QFile dir(fullPath);
    if(!dir.exists())
    {
        aka::PrintError("Directory not exists.", KAkaInvalidPath);
        return inDir;
    }

    // 读取.dir文件
    QFile f(fullPath);
    f.open(QIODevice::ReadOnly);
    // 反序列化
    QDataStream in(&f);
    in >> inDir;
    f.close();
    return inDir;
}

void AkaFileSystem::GenFileData(BaseFile *file, QString path)
{
    QString outputDatFullPath = path + "/" + file->GetName();
    // 判断具体为哪个子类
    if(dynamic_cast<File*>(file) != NULL)
    {
        // 为File类
        outputDatFullPath += ".dat";
        File* fileObj = (File*)file;
        // 序列化File对象
        QFile f(outputDatFullPath);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
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
        f.open(QIODevice::WriteOnly | QIODevice::Text);
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

    QString dirPath = path == "/" ? RootDirPath_ + "/" + path + "/" : RootDirPath_ + "/";
    if(!QDir(dirPath).exists())
    {
        aka::PrintError("Directory path not exists.", KAkaInvalidPath);
        return false;
    }

    if(QDir(dirPath + name).exists())
    {
        aka::PrintError("Directory already exists.", KAkaFileAlreadyExists);
        return true;
    }

    if(!QDir(dirPath).mkdir(name))
        return false;

    QString ParentFolderName = path == "/" ? "/" : dirPath.split("/").back();
    Directory dir(ParentFolderName, name, "SYSTEM");
    AkaFileSystem::GetFileSystem()->GenFileData(&dir, AkaFileSystem::GetFileSystem()->RootDirPath_ + "/" + path);
    return true;
}

AkaFileSystem::AkaFileSystem()
{
    RootDirPath_ = QCoreApplication::applicationDirPath() + "/" +
                aka::KAkaDataFolderName + "/" +
                aka::KAkaFileSystemRootFolderName;

    // 设置根文件夹("/")对象
    RooDirectory_ = new Directory("", "/", "SYSTEM");

    // 读取根目录
    for(auto file:QDir(RootDirPath_).entryInfoList())
    {
        if(file.suffix() == "dir")
            this->RooDirectory_->AddSubFolder(file.fileName());
        else if(file.suffix() == "dat")
            this->RooDirectory_->AddSubFile(file.fileName());
    }
}

AkaFileSystem::~AkaFileSystem()
{
}