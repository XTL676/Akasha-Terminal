#ifndef BASEFILE_H
#define BASEFILE_H
#include <QDateTime>
#include <QString>
#include <memory>

class BaseFile
{
protected:
    QString ParentFolderName_; // 所在的文件夹对象
    QString Name_; // 名称
    QDateTime CreateDateTime_; // 创建日期
    QDateTime ModifyDateTime_; // 最近修改日期
    QString Owner_; // TODO 文件所有者（默认为创建者）
    // TODO 权限

public:
    virtual bool IsChildClass() = 0;
    // 获取名称
    const QString GetName();
};

#endif // BASEFILE_H
