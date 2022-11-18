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
    // 列出目录下的所有文件和文件夹
    static int ls(QStringList args);
    // 拷贝文件或文件夹
    static int cp(QStringList args);
    // 移动文件或文件夹
    static int mv(QStringList args);
    // 编辑文件
    static int edit(QStringList args);

private:
    // 将缺省转化为全路径（绝对路径）
    static QString MakeFullPath(QString shortPath);
};

#endif // AKAFILECMD_H
