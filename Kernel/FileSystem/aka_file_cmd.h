#ifndef AKAFILECMD_H
#define AKAFILECMD_H
#include <QStringList>

class AkaFileCmd
{
public:
    // 创建文件夹
    static int mkdir(QStringList args);
    // 删除文件或文件夹
    static int rm(QStringList args);
    // 创建文件
    static int mkf(QStringList args);
    // 更换当前路径(进入目录)
    static int cd(QStringList args);
};

#endif // AKAFILECMD_H
