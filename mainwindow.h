#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Terminal/UI/aka_plain_text_edit.h"

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

    // 获取只要编辑区域控件实例
    AkaPlainTextEdit* GetMainPlainTextEdit();

private:
    int CursorLine_ = -1;

private slots:
    void on_MainInputArea_cursorPositionChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
