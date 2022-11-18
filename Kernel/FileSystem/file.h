#ifndef FILE_H
#define FILE_H
#include <QDataStream>
#include "base_file.h"

class File: public BaseFile
{
    // 重载序列化
    friend QDataStream &operator<<(QDataStream &output , const File &dataInfo);
    // 重载反序列化
    friend QDataStream &operator>>(QDataStream &input , File &dataInfo);

private:
    QString Suffix_; // 后缀名
    QString Content_; // 文件内容

public:
    // 获取后缀名
    QString GetSuffix();
    // 获取内容
    QString GetContent();

    // 设置后缀名
    void SetSuffix(QString suffix);
    // 设置内容
    void SetContent(QString content);

    bool IsChildClass() override;

    File();
    File(QString ParentFolderName,
         QString Name, QString Suffix,
         QString Owner, QString Content);
    File(QString ParentFolderName,
         QString Name, QString Suffix,
         QDateTime CreateDateTime, QDateTime ModifyDateTime,
         QString Owner, QString Content);
    ~File();
};

#endif // FILE_H
