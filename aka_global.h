#ifndef AKA_GLOBAL_H
#define AKA_GLOBAL_H
// 全局头文件,存放作用与全局的变量和函数

#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"
#include "Terminal/UI/aka_plain_text_edit.h"

namespace aka {
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

    // 获取主要编辑区域控件实例
    static AkaPlainTextEdit* GetMainPlainTextEdit()
    {
        MainWindow* main = (MainWindow*)GetMainWindow();
        if(main != nullptr)
            return main->GetMainPlainTextEdit();
        return nullptr;
    }

    // 输出信息到主要编辑区域
    inline static void Print(QString msg)
    {
        GetMainPlainTextEdit()->appendPlainText(msg);
    }

    static void Print(QString msg, QColor color)
    {
        // 获取主要编辑控件
        AkaPlainTextEdit* me = aka::GetMainPlainTextEdit();
        // 保存原来的样式
        QTextCharFormat fmt = me->currentCharFormat();
        QBrush cb = fmt.foreground();
        // 字体颜色为红色
        fmt.setForeground(QBrush(color));
        // 应用新样式
        me->mergeCurrentCharFormat(fmt);
        // 输出
        me->appendPlainText(msg);
        // 还原样式
        fmt.setForeground(cb);
        me->mergeCurrentCharFormat(fmt);
    }

    // 输出错误信息
    inline void PrintError(QString msg, int code)
    {
        aka::Print("Error:" + msg + "[" + QString::number(code) + "]", QColor("red"));
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
