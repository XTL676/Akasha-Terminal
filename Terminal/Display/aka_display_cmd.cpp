#include "aka_display_cmd.h"
#include <QException>
#include <QMainWindow>
#include "Kernel/ExceptionSystem/aka_exception_system.h"
#include "Kernel/ExceptionSystem/aka_status_code.h"
#include "Terminal/Input/akainputsystem.h"
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
