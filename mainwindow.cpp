#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <QTextBlock>
#include "aka_global.h"

#include <QDebug>
#include "Kernel/FileSystem/aka_file_system.h"
#include "Kernel/FileSystem/directory.h"
#include "Kernel/FileSystem/file.h"
#include "Terminal/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    // TODO TEST
//    qDebug() << AkaFileSystem::GetFileSystem()->GetRootDirectory()->GetName();
//    qDebug() << AkaFileSystem::GetFileSystem()->GetRootDirectory()->GetSubFolderNames();
//    qDebug() << AkaFileSystem::GetFileSystem()->GetRootDirectory()->GetSubFileNames();
//    AkaFileSystem::GetFileSystem()->CreateDir("/", "abc");
//    qDebug() << AkaFileSystem::GetFileSystem()->LoadDir("/abc").GetName();

    // 设置窗口基本属性
    this->setMinimumSize(aka::KAkaWindowMinimumSize); // 窗口最小大小
    this->setWindowTitle(aka::KAkaWindowDefaultTitle); // 窗口标题
    this->resize(aka::KAkaWindowDefaultSize); // 窗口大小

    // 设置主要编辑区域属性
    this->ui->MainInputArea->setFont(aka::KAkaInputDefaultFont); // 字体
    // 连接主编辑区域光标改变信号和槽
    connect(ui->MainInputArea, SIGNAL(cursorPositionChanged()), SLOT(on_MainInputArea_cursorPositionChanged()));

    // TODO 默认头显示
    ui->MainInputArea->appendPlainText("root@Akasha:~$");

    // 强制英文输入法
    LoadKeyboardLayout((LPCWSTR)QString("0x0409").utf16(), KLF_ACTIVATE);
}

AkaPlainTextEdit *MainWindow::GetMainPlainTextEdit()
{
    return this->ui->MainInputArea;
}

void MainWindow::on_MainInputArea_cursorPositionChanged()
{
    // 如果下一次光标位置不为读取光标位置的下面，则设置只读
    QTextCursor tc = ui->MainInputArea->textCursor();
    int current_line = tc.blockNumber();
    int nCurpos = tc.position() - tc.block().position(); // 当前光标在本行内的相对位置
    // 并禁止删除头显示(如：root@Akasha:~$)
    if(current_line >= CursorLine_ && nCurpos >= QString(aka::KAkaConselDefaultHead).length())
    {
        ui->MainInputArea->setReadOnly(false);
        CursorLine_ = current_line;
    }
    else
        ui->MainInputArea->setReadOnly(true);
}
