#ifndef AKABASICCMD_H
#define AKABASICCMD_H
#include <QStringList>

class AkaBasicCmd
{
public:
    // 帮助（列出所有命令）
    static int help(QStringList args);
    // 退出终端
    static int exit(QStringList args);
};

#endif // AKABASICCMD_H
