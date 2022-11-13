#ifndef AKAFILESYSTEM_H
#define AKAFILESYSTEM_H
#include <QString>
#include "Kernel/abstract_kernel_system.h"
#include "aka_global.h"
#include "directory.h"
#include "file.h"

class AkaFileSystem : public AbstractKernelSystem
{
    friend class KernelManager;

public:
    const QString GetRootDirPath();
    Directory* GetRootDirectory();
    const QString GetCurrentDirPath();
    Directory* GetCurrentDir();

    // 检测数据文件夹是否在当前程序目录下
    bool DetectDataFolder();

    // 加载文件夹(.dir文件)
    Directory* LoadDir(QString dirPath);
    // 加载文件(.dat文件)
    File LoadFile(QString filePath);
    // 创建文件夹
    bool CreateDir(QString path, QString name);
    // 删除文件夹
    bool DeleteDir(QString path);
    // 创建文件
    bool CreateFileA(QString path, QString name, QString suffix, QString content);
    // 删除文件
    bool DeleteFileA(QString fullPath);

    // 更换目录
    bool ChangeDir(QString path);

private:
    QString RootDirPath_; // 包含程序目录路径
    Directory* RootDirectory_ = nullptr;
    QString CurrentPath_ = aka::KAkaConselDefaultPath;
    Directory* CurrentDirectory_ = nullptr; // 当前所在文件夹的文件夹对象(父级文件夹对象)

private:
    // 生成数据文件(文件和文件夹的序列化)
    void GenFileData(BaseFile* file, QString path);
    // 初始化此系统
    void Initialize();

    AkaFileSystem();
    ~AkaFileSystem();
};

#endif // AKAFILESYSTEM_H
