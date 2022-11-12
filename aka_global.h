#ifndef AKA_GLOBAL_H
#define AKA_GLOBAL_H
// 全局头文件,存放作用与全局的变量和函数

#include <QApplication>
#include <QMainWindow>

namespace aka {
    const bool KAkaDebugOutput = false; // 是否开启调试输出

    const QString KAkaWindowDefaultTitle = "Akasha Terminal";
    const QSize KAkaWindowDefaultSize = QSize(1920, 1080);
    const QSize KAkaWindowMinimumSize = QSize(560, 310);

    const QFont KAkaInputDefaultFont = QFont("Cascadia Mono", 12);
    //const QFont KAkaInputDefaultFont = QFont("Teyvat", 15);

    const QChar KAkaConselSymbol = '$';
    const QString KAkaConselDefaultHead = QString("root@Akasha:~") + KAkaConselSymbol;

    const QString KAkaDataFolderName = "data";
    const QString KAkaFileSystemRootFolderName = "files";

    // 获取主窗口实例
    static QMainWindow* GetMainWindow()
    {
        static QMainWindow * mainwindow=nullptr;
        if(mainwindow)
           return mainwindow;
        for(const auto& it:qApp->topLevelWidgets())
        {
            mainwindow=qobject_cast<QMainWindow *>(it);
            if(mainwindow)
               return mainwindow;
        }
        return nullptr;
    }

    // 判断字符串是否为纯数字
    static bool StringIsNumber(QString &qstrSrc)
    {
        QByteArray ba = qstrSrc.toLatin1();
        const char *s = ba.data();
        bool bret = true;
        while(*s)
        {
            if(*s>='0' && *s<='9')
            {

            }
            else
            {
                bret = false;
                break;
            }
            s++;
        }
        return bret;
    }

    // 将所有\ \\ // 转为/
    inline static void PathReplace(QString &path)
    {
        path.replace("//", "/");
        path.replace("\\", "/");
        path.replace("\\\\", "/");
    }

}

#endif // AKA_GLOBAL_H
