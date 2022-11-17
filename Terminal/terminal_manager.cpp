#include "terminal_manager.h"

TerminalManager* TerminalManager::Instance_ = nullptr;

TerminalManager *TerminalManager::GetTerminalManager()
{
    if(Instance_ == nullptr)
        Instance_ = new TerminalManager();
    return Instance_;
}

void TerminalManager::Init(QMainWindow* MainWindow, QPlainTextEdit *MainEditArea)
{
    // 初始化所有受管理的系统
    GetInputSystem();

    // 设置主窗口接口
    MainWindow_ = MainWindow;

    // 设置输出接口
    MainEditArea_ = MainEditArea;
}

void TerminalManager::Print(QString msg)
{
    MainEditArea_->appendPlainText(msg);
}

void TerminalManager::Print(QString msg, QColor color)
{
    // 保存原来的样式
    QTextCharFormat fmt = MainEditArea_->currentCharFormat();
    QBrush cb = fmt.foreground();
    // 字体颜色为红色
    fmt.setForeground(QBrush(color));
    // 应用新样式
    MainEditArea_->mergeCurrentCharFormat(fmt);
    // 输出
    MainEditArea_->appendPlainText(msg);
    // 还原样式
    fmt.setForeground(cb);
    MainEditArea_->mergeCurrentCharFormat(fmt);
}

void TerminalManager::PrintError(QString msg, int code)
{
    Print(QObject::tr("Error:") + msg + "[" + QString::number(code) + "]", QColor("red"));
}

QPlainTextEdit *TerminalManager::GetMainEditArea()
{
    return MainEditArea_;
}

QMainWindow *TerminalManager::GetMainWindow()
{
    return MainWindow_;
}

AkaInputSystem *TerminalManager::GetInputSystem()
{
    if(InputSystem_ == nullptr)
        InputSystem_ = new AkaInputSystem();
    return InputSystem_;
}

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
}
