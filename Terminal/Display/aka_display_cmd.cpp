#include "aka_display_cmd.h"
#include <QException>
#include <QMainWindow>
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "aka_global.h"

int AkaDisplayCmd::resize(QStringList args)
{
    if(args.length() < 3)
    {
        aka::PrintError("No or missing parameters.Should be [resize width height]", KAkaMissingParameter);
        return 1;
    }
    int width = -1, height = -1;
    width = args[1].toInt(), height = args[2].toInt();
    if(width <= 0 || height <= 0)
    {
        aka::PrintError("Invalid window size.", KAkaInvalidNum);
        return 1;
    }

    if(width < aka::KAkaWindowMinimumSize.width() || height < aka::KAkaWindowMinimumSize.height())
    {
        aka::PrintError("Window size to small.", KAkaInvalidNum);
        return 1;
    }

    aka::GetMainWindow()->resize(width, height);
    aka::Print("The window size is set to " +
               QString::number(aka::GetMainWindow()->width()) + "*" +
               QString::number(aka::GetMainWindow()->height()) + " successfully.", QColor("green"));
    return 1;
}

int AkaDisplayCmd::refont(QStringList args)
{
    if(args.length() < 2)
    {
        aka::PrintError("No or missing parameters.Should be [resize width height]", KAkaMissingParameter);
        return 1;
    }

    // 只有一个参数时
    if(args.length() == 2)
    {
        QFont font = aka::GetMainPlainTextEdit()->font();
        // 参数是否为数字
        if(aka::StringIsNumber(args[1]))
            font.setPointSize(args[1].toInt());
        else
            font.setFamily(args[1]);

        aka::GetMainPlainTextEdit()->setFont(font);
        aka::Print("The font is set to " + args[1] + " successfully.", QColor("green"));
        return 1;
    }

    if(args.length() == 3)
    {
        if(aka::StringIsNumber(args[1]))
        {
            aka::GetMainPlainTextEdit()->setFont(QFont(args[2], args[1].toInt()));
            aka::Print("The font is set to " + args[2] + ",size=" + args[1] + " successfully.", QColor("green"));
        }
        else if(aka::StringIsNumber(args[2]))
        {
            aka::GetMainPlainTextEdit()->setFont(QFont(args[1], args[2].toInt()));
            aka::Print("The font is set to " + args[1] + ",size=" + args[2] + " successfully.", QColor("green"));
        }
        else
            aka::PrintError("Invalid font name or font size.", KAkaInvalidNum);

        return 1;
    }

    aka::PrintError("Too many parameters.", KAkaTooManyParameters);
    return 1;
}
