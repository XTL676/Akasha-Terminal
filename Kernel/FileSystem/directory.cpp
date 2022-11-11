#include "directory.h"
#include "Kernel/FileSystem/file.h"

QDataStream &operator<<(QDataStream &output , const Directory &dataInfo)
{
    output << dataInfo.ParentFolderName_
           << dataInfo.SubFolderNames_
           << dataInfo.SubFileNames_
           << dataInfo.Name_
           << dataInfo.Owner_
           << dataInfo.CreateDateTime_
           << dataInfo.ModifyDateTime_;
    return output;
}

QDataStream &operator>>(QDataStream &input , Directory &dataInfo)
{
    input >> dataInfo.ParentFolderName_
          >> dataInfo.SubFolderNames_
          >> dataInfo.SubFileNames_
          >> dataInfo.Name_
          >> dataInfo.Owner_
          >> dataInfo.CreateDateTime_
          >> dataInfo.ModifyDateTime_;
    return input;
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
