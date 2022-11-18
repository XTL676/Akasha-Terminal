#include "aka_plain_text_edit.h"
#include <QTextBlock>
#include <QApplication>
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Kernel/kernel_manager.h"
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

void AkaPlainTextEdit::ShowEditFileUI(QString path)
{
    EditFilePath_ = path;
    // 保存编辑区域原内容
    PlainContent_ = toPlainText();
    // 设置状态
    State_ = EPlainTextEditState::EditFile;
    // 清空区域
    clear();
    // 获取文件对象
    bool success;
    File file = KernelManager::GetKernelManager()->GetFileSystem()->LoadFile(path, success);
    if(!success) return;
    // 添加文件内容到编辑区域
    appendPlainText(file.GetContent());
    // 设置可读写
    setReadOnly(false);
}

void AkaPlainTextEdit::CloseEditFileUI()
{
    // 清空区域
    clear();
    // 还原之前的命令界面
    appendPlainText(PlainContent_);
    appendPlainText(GetConsoleHead());
    // 移动光标
    moveCursor(QTextCursor::End);
    // 设置状态
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
    if(State_ == EPlainTextEditState::EditFile) return;

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

    if(State_ != EPlainTextEditState::EditFile)
    {
        // 下一次头显示
        appendPlainText(ConsoleHead_);
        // 将光标移到末尾
        tc.movePosition(QTextCursor::End);
        setTextCursor(tc);
    }
}

void AkaPlainTextEdit::ESCEvent()
{
    /**
     * 文件编辑界面
     *   退出界面：按下ESC
     */
    if(State_ == EPlainTextEditState::EditFile)
    {
        CloseEditFileUI();
    }
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
            if(State_ == EPlainTextEditState::EditFile) return QWidget::eventFilter(target,event);

            // 回车事件
            EnterEvent();
            return true;
        }

        // 退格键
        if(key->key() == Qt::Key::Key_Backspace)
        {
            if(State_ == EPlainTextEditState::EditFile) return QWidget::eventFilter(target,event);

            // 回车事件(禁止删除头显示(如：root@Akasha:~$))
            QTextCursor tc = textCursor();
            int nCurpos = tc.position() - tc.block().position(); // 当前光标在本行内的相对位置
            if(nCurpos <= ConsoleHead_.length())
                return true;
        }

        // ESC
        if(key->key() == Qt::Key::Key_Escape)
        {
            // ESC事件
            ESCEvent();
            return true;
        }

        // Ctrl + S 保存编辑
        if(key->key() == Qt::Key::Key_S  &&  key->modifiers() == Qt::ControlModifier)
        {
            KernelManager::GetKernelManager()->GetFileSystem()->ModifyFileContent(EditFilePath_, toPlainText());
            return true;
        }
    }
    return QWidget::eventFilter(target,event);
}
