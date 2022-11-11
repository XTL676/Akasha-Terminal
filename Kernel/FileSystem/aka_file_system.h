#ifndef AKAFILESYSTEM_H
#define AKAFILESYSTEM_H
#include <QString>
#include "directory.h"

class AkaFileSystem
{
public:
    static AkaFileSystem* GetFileSystem();
    const QString GetRootDirPath();
    Directory* GetRootDirectory();

    // 将文件和文件夹以一种形式保存到程序目录下
    bool Save();
    // 检测数据文件夹是否在当前程序目录下
    bool DetectDataFolder();
    // 加载文件夹
    Directory LoadDir(QString dirPath);
    // 生成数据文件(文件和文件夹的序列化)
    void GenFileData(BaseFile* file, QString path);
    // TODO 创建文件夹
    bool CreateDir(QString path, QString name);
    // TODO 创建文件

private:
    static AkaFileSystem* Instance_;
    QString RootDirPath_;
    Directory* RooDirectory_;

private:
    AkaFileSystem();
    ~AkaFileSystem();
};

#endif // AKAFILESYSTEM_H
