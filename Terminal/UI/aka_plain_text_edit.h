#ifndef AKAPLAINTEXTEDIT_H
#define AKAPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class AkaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit AkaPlainTextEdit(QWidget *parent = nullptr);
    explicit AkaPlainTextEdit(const QString &text, QWidget *parent = nullptr);
    // 当前编辑区状态枚举
    enum EPlainTextEditState {
        Command, // 输入命令状态(默认)
        ClearScreen, // 清除屏幕状态
        EditFile, // 编辑文件状态
    };

    // 初始化
    void Init();

    // 显示编辑文件界面
    void ShowEditFileUI(QString path);
    // 关闭编辑文件界面
    void CloseEditFileUI();

    // 获取显示头
    const QString GetConsoleHead();
    // 设置显示头
    void SetConsoleHead(QString user, QString path);

    // 获取当前状态
    EPlainTextEditState GetState();
    // 设置当前状态
    void SetState(EPlainTextEditState State);

protected:
    // 事件过滤器
    bool eventFilter(QObject *target, QEvent *event) override;

    // 回车事件
    void EnterEvent();
    // ESC事件
    void ESCEvent();

private:
    QString ConsoleHead_;
    EPlainTextEditState State_;

    QString PlainContent_;
    QString EditFilePath_;

signals:

};

#endif // AKAPLAINTEXTEDIT_H
