#ifndef AKADISPLAYCMD_H
#define AKADISPLAYCMD_H
#include <QStringList>

class AkaDisplayCmd
{
public:
    // 重设窗口大小
    static int resize(QStringList args);

    // 重设字体
    static int refont(QStringList args);

    // 清空当前编辑区
    static int cls(QStringList args);
};

#endif // AKADISPLAYCMD_H
