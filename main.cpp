#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include "aka_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 国际化
    QTranslator Translator;
    if(Translator.load(aka::KAkaWindowDefaultTranslate))
        a.installTranslator(&Translator);

    MainWindow w;
    w.show();
    return a.exec();
}
