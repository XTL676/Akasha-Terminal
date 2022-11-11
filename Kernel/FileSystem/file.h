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
