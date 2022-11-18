#include "file.h"
#include <QStringBuilder>

void File::Serialize(QTextStream &out)
{
    /**
     * 使用?作为每个变量的开头，分隔多个字符串用>
     */
    // 写入父级文件夹名称
    out << '?';
    out << ParentFolderName_;

    // 写入名称
    out << '?';
    out << Name_;

    // 写入后缀名
    out << '?';
    out << Suffix_;

    // 写入创建日期
    out << '?';
    out << CreateDateTime_.toString();

    // 写入修改日期
    out << '?';
    out << ModifyDateTime_.toString();

    // 写入所有者
    out << '?';
    out << Owner_;

    // 写入内容(从第二行开始到结束)
    out << '?';
    out << '\n';
    out << Content_;
}

void File::Deserialize(QTextStream &in)
{
    /**
     * 使用?作为每个变量的开头，分隔多个字符串用>
     */
    // 读取第一行文件基本信息
    QString head = in.readLine();

    // 读取第二行文件内容
    QString content;
    while (!in.atEnd())
        content = content % in.readLine() % '\n';

    /**
     * @brief list 所有存储变量的值
     * 结构:
     *      QList("", 父级文件夹名称, 文件名称, 后缀名，创建日期, 修改日期，所有者)
     */
    QStringList list = head.split("?");
    list.pop_front(); // 去除第一个无用数据

    // 父级文件夹名称
    ParentFolderName_ = list[0];

    // 文件名称
    Name_ = list[1];

    // 后缀名
    Suffix_ = list[2];

    // 创建日期
    CreateDateTime_ = QDateTime().fromString(list[3]);

    // 修改日期
    ModifyDateTime_ = QDateTime().fromString(list[4]);

    // 所有者
    Owner_ = list[5];

    // 内容
    Content_ = content;
}

QString File::GetSuffix()
{
    return Suffix_;
}

QString File::GetContent()
{
    return Content_;
}

void File::SetSuffix(QString suffix)
{
    Suffix_ = suffix;
}

void File::SetContent(QString content)
{
    Content_ = content;
}

bool File::IsChildClass()
{
    return true;
}

File::File()
{
}

File::File(QString ParentFolderName, QString Name, QString Suffix, QString Owner, QString Content)
{
    ParentFolderName_ = ParentFolderName;
    Name_ = Name;
    Suffix_ = Suffix;
    Owner_ = Owner;
    Content_ = Content;
}

File::File(
       QString ParentFolderName,
       QString Name, QString Suffix,
       QDateTime CreateDateTime, QDateTime ModifyDateTime,
       QString Owner, QString Content)
{
    ParentFolderName_ = ParentFolderName;
    Name_ = Name;
    Suffix_ = Suffix;
    CreateDateTime_ = CreateDateTime;
    ModifyDateTime_ = ModifyDateTime;
    Owner_ = Owner;
    Content_ = Content;
}

File::~File()
{
}
