#include "aka_plain_text_edit.h"
#include <QTextBlock>
#include <QApplication>
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Terminal/terminal_manager.h"
#include "aka_global.h"

AkaPlainTextEdit::AkaPlainTextEdit(QWidget *parent)
    : QPlainTextEdit{parent}
{
    Init();
}

void AkaPlainTextEdit::Init()
{
    // 设置完后自动调用其eventFilter函数
    installEventFilter(this);

    ConsoleHead_ = aka::KAkaConselDefaultUser + aka::KAkaConselName + aka::KAkaConselDefaultPath + aka::KAkaConselSymbol;
    State_ = EPlainTextEditState::Command;
}

const QString AkaPlainTextEdit::GetConsoleHead()
{
    return ConsoleHead_;
}

void AkaPlainTextEdit::SetConsoleHead(QString user, QString path)
{
    ConsoleHead_ = user + aka::KAkaConselName + path + aka::KAkaConselSymbol;
}

AkaPlainTextEdit::EPlainTextEditState AkaPlainTextEdit::GetState()
{
    return State_;
}

void AkaPlainTextEdit::SetState(EPlainTextEditState State)
{
    State_ = State;
}

void AkaPlainTextEdit::EnterEvent()
{
    // 获取当前光标位置所在行内容
    QTextCursor tc = textCursor(); // 获取当前光标
    int rowNum = tc.blockNumber(); // 获取光标所在行的行号
    QString line = document()->findBlockByLineNumber(rowNum).text(); // 获取所在行内容

    // 处理输入行(获取参数)
    AkaInputSystem* input = TerminalManager::GetTerminalManager()->GetInputSystem();
    QStringList parameters = input->SplitLine(input->GetLineFromPlain(line));

    // 执行命令
    int status = 1; // 解决“开启时马上输入命令并回车程序会异常退出的BUG”
    if(parameters.length() >= 1)
        status = input->Execute(parameters);
    if(status <= KAkaNormalExit)
        qApp->exit(status); // 关闭程序

    // 下一次头显示
    appendPlainText(ConsoleHead_);
    // 将光标移到末尾
    tc.movePosition(QTextCursor::End);
    setTextCursor(tc);
}

bool AkaPlainTextEdit::eventFilter(QObject *target, QEvent *event)
{
    // 按键按下
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent*>(event);

        // 回车键
        if(key->key() == Qt::Key::Key_Return)
        {
            // 回车事件
            EnterEvent();
            return true;
        }

        // 退格键
        if(key->key() == Qt::Key::Key_Backspace)
        {
            // 回车事件(禁止删除头显示(如：root@Akasha:~$))
            QTextCursor tc = textCursor();
            int nCurpos = tc.position() - tc.block().position(); // 当前光标在本行内的相对位置
            if(nCurpos <= ConsoleHead_.length())
                return true;
        }
    }
    return QWidget::eventFilter(target,event);
}
