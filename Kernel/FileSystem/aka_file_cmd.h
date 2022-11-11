#ifndef AKAFILECMD_H
#define AKAFILECMD_H
#include <QStringList>

class AkaFileCmd
{
public:
    // 创建文件夹
    static int mkdir(QStringList args);
    // 删除文件或文件夹
};

#endif // AKAFILECMD_H
