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
};

#endif // AKADISPLAYCMD_H
