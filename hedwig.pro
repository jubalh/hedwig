TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.c \
    src/users.c \
    src/parser.c \
    src/xmpp.c

LIBS += -lstrophe -lssl

HEADERS += \
    src/users.h \
    src/parser.h \
    src/xmpp.h
