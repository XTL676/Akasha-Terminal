#ifndef AKAFILESYSTEM_H
#define AKAFILESYSTEM_H
#include <QString>
#include "Kernel/abstract_kernel_system.h"
#include "directory.h"
#include "file.h"

class AkaFileSystem : public AbstractKernelSystem
{
    friend class KernelManager;

public:
    const QString GetRootDirPath();
    Directory* GetRootDirectory();

    // 将文件和文件夹以一种形式保存到程序目录下
    bool Save();
    // 检测数据文件夹是否在当前程序目录下
    bool DetectDataFolder();
    // 加载文件夹(.dir文件)
    Directory LoadDir(QString dirPath);
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

private:
    QString RootDirPath_;
    Directory* RootDirectory_;

    // 生成数据文件(文件和文件夹的序列化)
    void GenFileData(BaseFile* file, QString path);
    // 初始化此系统
    void Initialize();

private:
    AkaFileSystem();
    ~AkaFileSystem();
};

#endif // AKAFILESYSTEM_H
