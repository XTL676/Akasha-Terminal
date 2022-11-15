#include "base_file.h"

const QString BaseFile::GetName()
{
    return Name_;
}

const QString BaseFile::GetOwner()
{
    return Owner_;
}

const QDateTime BaseFile::GetCreateDateTime()
{
    return CreateDateTime_;
}

const QDateTime BaseFile::GetModifyDateTime()
{
    return ModifyDateTime_;
}

const QString BaseFile::GetParentFolderName()
{
    return ParentFolderName_;
}

void BaseFile::SetName(QString name)
{
    Name_ = name;
}

void BaseFile::SetParentFolderName(QString name)
{
    ParentFolderName_ = name;
}
