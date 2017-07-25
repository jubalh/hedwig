TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    users.c \
    xmpp.c
LIBS += -lstrophe -lssl

HEADERS += \
    users.h \
    xmpp.h
