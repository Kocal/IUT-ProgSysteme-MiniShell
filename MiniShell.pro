TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    include/minishell/minishell.cpp \
    include/util.cpp

HEADERS += \
    include/minishell/minishell.h \
    include/util.h

