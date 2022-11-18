#ifndef FILE_H
#define FILE_H
#include <QTextStream>
#include "base_file.h"

class File: public BaseFile
{
private:
    QString Suffix_; // 后缀名
    QString Content_; // 文件内容

public:
    // 序列化
    void Serialize(QTextStream& out);
    // 反序列化
    void Deserialize(QTextStream& in);

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
