QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Kernel/Basic/aka_basic_cmd.cpp \
    Kernel/ExceptionSystem/aka_exception_system.cpp \
    Kernel/FileSystem/aka_file_cmd.cpp \
    Kernel/FileSystem/aka_file_system.cpp \
    Kernel/FileSystem/base_file.cpp \
    Kernel/FileSystem/directory.cpp \
    Kernel/FileSystem/file.cpp \
    Kernel/abstract_kernel_system.cpp \
    Kernel/kernel_manager.cpp \
    Terminal/Input/aka_input_system.cpp \
    Terminal/Display/aka_display_cmd.cpp \
    Terminal/UI/aka_plain_text_edit.cpp \
    Terminal/abstract_terminal_system.cpp \
    Terminal/terminal_manager.cpp \
    abstract_system.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Kernel/Basic/aka_basic_cmd.h \
    Kernel/ExceptionSystem/aka_exception_system.h \
    Kernel/ExceptionSystem/aka_status_code.h \
    Kernel/FileSystem/aka_file_cmd.h \
    Kernel/FileSystem/aka_file_system.h \
    Kernel/FileSystem/base_file.h \
    Kernel/FileSystem/directory.h \
    Kernel/FileSystem/file.h \
    Kernel/abstract_kernel_system.h \
    Kernel/kernel_manager.h \
    Terminal/Display/aka_display_cmd.h \
    Terminal/Info/aka_commands.h \
    Terminal/Input/aka_input_system.h \
    Terminal/Display/aka_display_cmd.h \
    Terminal/UI/aka_plain_text_edit.h \
    Terminal/abstract_terminal_system.h \
    Terminal/terminal_manager.h \
    abstract_system.h \
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
