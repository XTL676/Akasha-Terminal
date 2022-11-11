#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "base_file.h"

class Directory: public BaseFile
{
    // 重载序列化
    friend QDataStream &operator<<(QDataStream &output , const Directory &dataInfo);
    // 重载反序列化
    friend QDataStream &operator>>(QDataStream &input , Directory &dataInfo);

private:
    QStringList SubFolderNames_; // 文件夹所包含的文件夹
    QStringList SubFileNames_; // 文件夹所包含的文件

public:
    // 获取文件夹所包含的文件夹
    const QStringList GetSubFolderNames();
    // 获取文件夹所包含的文件
    const QStringList GetSubFileNames();
    // 添加子文件
    void AddSubFile(QString fileName);
    // 添加子文件夹
    void AddSubFolder(QString dirName);
    // 移除子文件
    void RemoveSubFile(QString fileName);
    // 移除子文件夹
    void RemoveSubFolder(QString dirName);

    bool IsChildClass() override;
    Directory();
    Directory(QString ParentFolderName,
              QString Name, QString Owner);
    Directory(QString ParentFolderName,
              QString Name, QString Owner,
              QDateTime CreateDateTime, QDateTime ModifyDateTime);
    ~Directory();
};

#endif // DIRECTORY_H
