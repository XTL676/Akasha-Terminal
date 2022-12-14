#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 初始化主窗口
    void Init();

private:
    int CursorLine_ = -1;

private slots:
    void on_MainInputArea_cursorPositionChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
