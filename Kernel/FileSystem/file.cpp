#include "file.h"

QDataStream &operator<<(QDataStream &output , const File &dataInfo)
{
    output << dataInfo.ParentFolderName_
           << dataInfo.Name_
           << dataInfo.Suffix_
           << dataInfo.CreateDateTime_
           << dataInfo.ModifyDateTime_
           << dataInfo.Owner_
           << dataInfo.Content_;
    return output;
}

QDataStream &operator>>(QDataStream &input , File &dataInfo)
{
    input >> dataInfo.ParentFolderName_
          >> dataInfo.Name_
          >> dataInfo.Suffix_
          >> dataInfo.CreateDateTime_
          >> dataInfo.ModifyDateTime_
          >> dataInfo.Owner_
          >> dataInfo.Content_;
    return input;
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
