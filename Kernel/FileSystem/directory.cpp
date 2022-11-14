#include "directory.h"

void Directory::Serialize(QTextStream &out)
{
    /**
     * 使用?作为每个变量的开头，分隔多个字符串用>
     */
    // 写入父级文件夹名称
    out << '?';
    out << ParentFolderName_;

    // 写入所有子文件夹名称
    out << '?';
    for(QString name:SubFolderNames_)
    {
        out << name;
        out << '>';
    }

    // 写入所有子文件名称
    out << '?';
    for(QString name:SubFileNames_)
    {
        out << name;
        out << '>';
    }

    // 写入名称
    out << '?';
    out << Name_;

    // 写入所有者
    out << '?';
    out << Owner_;

    // 写入创建日期
    out << '?';
    out << CreateDateTime_.toString();

    // 写入修改日期
    out << '?';
    out << ModifyDateTime_.toString();
}

void Directory::Deserialize(QTextStream &in)
{
    /**
     * 使用?作为每个变量的开头，分隔多个字符串用>
     */
    // 读取所有内容
    QString content = in.readAll();

    /**
     * @brief list 所有存储变量的值
     * 结构:
     *      QList("", 父级文件夹名称, 所有子级文件夹名称, 所有子级文件名称, 文件夹名称, 所有者, 创建日期, 修改日期)
     */
    QStringList list = content.split("?");
    list.pop_front(); // 去除第一个无用数据

    // 父级文件夹名称
    ParentFolderName_ = list[0];

    // 所有子级文件夹名称
    QStringList dirs = list[1].split(">");
    dirs.removeAll("");
    SubFolderNames_ = dirs;

    // 所有子级文件名称
    QStringList files = list[2].split(">");
    files.removeAll("");
    SubFileNames_ = files;

    // 文件夹名称
    Name_ = list[3];

    // 所有者
    Owner_ = list[4];

    // 创建日期
    CreateDateTime_ = QDateTime().fromString(list[5]);

    // 修改日期
    ModifyDateTime_ = QDateTime().fromString(list[6]);
}

const QStringList Directory::GetSubFolderNames()
{
    return SubFolderNames_;
}

const QStringList Directory::GetSubFileNames()
{
    return SubFileNames_;
}

void Directory::AddSubFile(QString fileName)
{
    if(!SubFileNames_.contains(fileName))
        SubFileNames_.append(fileName);
}

void Directory::AddSubFolder(QString dirName)
{
    if(!SubFolderNames_.contains(dirName))
        SubFolderNames_.append(dirName);
}

void Directory::RemoveSubFile(QString fileName)
{
    SubFileNames_.removeAll(fileName);
}

void Directory::RemoveSubFolder(QString dirName)
{
    SubFolderNames_.removeAll(dirName);
}

bool Directory::IsChildClass()
{
    return true;
}

Directory::Directory()
{
}

Directory::Directory(QString ParentFolderName, QString Name, QString Owner)
{
    ParentFolderName_ = ParentFolderName;
    Name_ = Name;
    CreateDateTime_ = QDateTime::currentDateTime();
    ModifyDateTime_ = QDateTime::currentDateTime();
    Owner_ = Owner;
}

Directory::Directory(
        QString ParentFolderName,
        QString Name, QString Owner,
        QDateTime CreateDateTime, QDateTime ModifyDateTime)
{
    ParentFolderName_ = ParentFolderName;
    Name_ = Name;
    CreateDateTime_ = CreateDateTime;
    ModifyDateTime_ = ModifyDateTime;
    Owner_ = Owner;
}

Directory::~Directory()
{
}
