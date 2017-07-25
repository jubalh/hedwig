TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    users.c \
    parser.c \
    xmpp.c

LIBS += -lstrophe -lssl

HEADERS += \
    users.h \
    parser.h \
    xmpp.h
