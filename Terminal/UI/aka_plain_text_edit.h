#ifndef AKAPLAINTEXTEDIT_H
#define AKAPLAINTEXTEDIT_H

#include "aka_global.h"
#include <QPlainTextEdit>

class AkaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit AkaPlainTextEdit(QWidget *parent = nullptr);
    explicit AkaPlainTextEdit(const QString &text, QWidget *parent = nullptr);

    // 初始化
    void Init();

    // 获取显示头
    const QString GetConsoleHead();
    // 设置显示头
    void SetConsoleHead(QString user, QString path);

protected:
    // 事件过滤器
    bool eventFilter(QObject *target, QEvent *event);

    // 回车事件
    void EnterEvent();

private:
    QString ConsoleHead_;

signals:

};

#endif // AKAPLAINTEXTEDIT_H
