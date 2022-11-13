#include "aka_display_cmd.h"
#include <QException>
#include <QMainWindow>
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "aka_global.h"
#include "Terminal/terminal_manager.h"

int AkaDisplayCmd::resize(QStringList args)
{
    if(args.length() < 3)
    {
        TerminalManager::GetTerminalManager()->PrintError("No or missing parameters.Should be [resize width height]", KAkaMissingParameter);
        return 1;
    }
    int width = -1, height = -1;
    width = args[1].toInt(), height = args[2].toInt();
    if(width <= 0 || height <= 0)
    {
       TerminalManager::GetTerminalManager()->PrintError("Invalid window size.", KAkaInvalidNum);
        return 1;
    }

    if(width < aka::KAkaWindowMinimumSize.width() || height < aka::KAkaWindowMinimumSize.height())
    {
       TerminalManager::GetTerminalManager()->PrintError("Window size to small.", KAkaInvalidNum);
        return 1;
    }

    TerminalManager::GetTerminalManager()->GetMainWindow()->resize(width, height);
    TerminalManager::GetTerminalManager()->Print("The window size is set to " +
               QString::number(TerminalManager::GetTerminalManager()->GetMainWindow()->width()) + "*" +
               QString::number(TerminalManager::GetTerminalManager()->GetMainWindow()->height()) + " successfully.", QColor("green"));
    return 1;
}

int AkaDisplayCmd::refont(QStringList args)
{
    if(args.length() < 2)
    {
       TerminalManager::GetTerminalManager()->PrintError("No or missing parameters.Should be [resize width height]", KAkaMissingParameter);
        return 1;
    }

    // 只有一个参数时
    if(args.length() == 2)
    {
        QFont font = TerminalManager::GetTerminalManager()->GetMainEditArea()->font();
        // 参数是否为数字
        if(aka::StringIsNumber(args[1]))
            font.setPointSize(args[1].toInt());
        else
            font.setFamily(args[1]);

        TerminalManager::GetTerminalManager()->GetMainEditArea()->setFont(font);
        TerminalManager::GetTerminalManager()->Print("The font is set to " + args[1] + " successfully.", QColor("green"));
        return 1;
    }

    if(args.length() == 3)
    {
        if(aka::StringIsNumber(args[1]))
        {
            TerminalManager::GetTerminalManager()->GetMainEditArea()->setFont(QFont(args[2], args[1].toInt()));
            TerminalManager::GetTerminalManager()->Print("The font is set to " + args[2] + ",size=" + args[1] + " successfully.", QColor("green"));
        }
        else if(aka::StringIsNumber(args[2]))
        {
            TerminalManager::GetTerminalManager()->GetMainEditArea()->setFont(QFont(args[1], args[2].toInt()));
            TerminalManager::GetTerminalManager()->Print("The font is set to " + args[1] + ",size=" + args[2] + " successfully.", QColor("green"));
        }
        else
           TerminalManager::GetTerminalManager()->PrintError("Invalid font name or font size.", KAkaInvalidNum);

        return 1;
    }

   TerminalManager::GetTerminalManager()->PrintError("Too many parameters.", KAkaTooManyParameters);
    return 1;
}
