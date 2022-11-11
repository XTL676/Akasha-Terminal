#ifndef UTILS_H
#define UTILS_H
#include <QDir>
#include "Kernel/FileSystem/aka_file_system.h"
#include "Kernel/FileSystem/directory.h"
#include "Kernel/FileSystem/file.h"
#include "aka_global.h"

namespace util {
    // 自动生成数据文件(.dat)
    static void AutoGenDataFile(QString path)
    {
//        QString root = path;
//        for(QFileInfo file:QDir(root).entryInfoList())
//        {
//            if(file.fileName() == "." || file.fileName() == "..") continue;
//            if(file.isDir())
//            {
//                qDebug() << file.fileName();

//                Directory dir;
//                if(root == AkaFileSystem::GetFileSystem()->GetRootDirPath())
//                    dir = Directory(aka::KAkaRootDirectoryObj, file.fileName(), "SYSTEM",
//                                    QDateTime::currentDateTime(), QDateTime::currentDateTime());
//                else
//                {
//                }

//                AkaFileSystem::GetFileSystem()->GenFileData(&dir, root);

//                if(QDir(file.filePath() + "/").entryInfoList().length() > 2)
//                    AutoGenDataFile(root + "/" + file.fileName());
//            }
//        }

//        for(auto dir:folders)
//        {
//            Directory dict(aka::KAkaRootDirectoryObj, dir, "SYSTEM", QDateTime::currentDateTime(), QDateTime::currentDateTime());
//            AkaFileSystem::GetFileSystem()->GenFileData(&dict, "/");
//        }
    }
}
#endif // UTILS_H
