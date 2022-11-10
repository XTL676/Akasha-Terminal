QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Kernel/Basic/aka_basic_cmd.cpp \
    Kernel/ExceptionSystem/aka_exception_system.cpp \
    Terminal/Input/aka_input_system.cpp \
    Terminal/Display/aka_display_cmd.cpp \
    Terminal/UI/aka_plain_text_edit.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Kernel/Basic/aka_basic_cmd.h \
    Kernel/ExceptionSystem/aka_exception_system.h \
    Kernel/ExceptionSystem/aka_status_code.h \
    Terminal/Display/aka_display_cmd.h \
    Terminal/Info/aka_commands.h \
    Terminal/Input/aka_input_system.h \
    Terminal/Display/aka_display_cmd.h \
    Terminal/UI/aka_plain_text_edit.h \
    aka_global.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore
