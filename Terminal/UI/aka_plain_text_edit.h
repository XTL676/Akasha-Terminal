#ifndef AKAPLAINTEXTEDIT_H
#define AKAPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class AkaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit AkaPlainTextEdit(QWidget *parent = nullptr);
    explicit AkaPlainTextEdit(const QString &text, QWidget *parent = nullptr);

    // 初始化
    void Init();

protected:
    // 事件过滤器
    bool eventFilter(QObject *target, QEvent *event);

    // 回车事件
    void EnterEvent();

signals:

};

#endif // AKAPLAINTEXTEDIT_H
