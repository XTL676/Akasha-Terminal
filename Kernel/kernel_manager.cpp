#include "kernel_manager.h"

KernelManager* KernelManager::Instance_ = nullptr;

KernelManager *KernelManager::GetKernelManager()
{
    if(Instance_ == nullptr)
        Instance_ = new KernelManager();
    return Instance_;
}

void KernelManager::Init(QPlainTextEdit* MainEditArea)
{
    // 初始化所有受管理的系统
    GetExceptionSystem();
    GetFileSystem();

    // 设置输出接口
    MainEditArea_ = MainEditArea;
}

void KernelManager::Print(QString msg)
{
    MainEditArea_->appendPlainText(msg);
}

void KernelManager::Print(QString msg, QColor color)
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

void KernelManager::PrintError(QString msg, int code)
{
    Print("Error:" + msg + "[" + QString::number(code) + "]", QColor("red"));
}

QPlainTextEdit *KernelManager::GetMainEditArea()
{
    return MainEditArea_;
}

AkaExceptionSystem *KernelManager::GetExceptionSystem()
{
    if(ExceptionSystem_ == nullptr)
        ExceptionSystem_ = new AkaExceptionSystem();
    return ExceptionSystem_;
}

AkaFileSystem *KernelManager::GetFileSystem()
{
    if(FileSystem_ == nullptr)
        FileSystem_ = new AkaFileSystem();
    return FileSystem_;
}

KernelManager::KernelManager()
{
}

KernelManager::~KernelManager()
{
}
