
LIBS           += -lbbcascades
QT             += declarative
QT             -= gui

QMAKE_CFLAGS   += -Wno-psabi
QMAKE_CXXFLAGS += -Wno-psabi

TEMPLATE = lib

staticlib {
    QNX_VARIANT_DIR = a
} else {
    QNX_VARIANT_DIR = so
}

device {
    QNX_VARIANT_DIR = $${QNX_VARIANT_DIR}.le-v7
}

simulator {
}

CONFIG(debug, debug|release) {
    QNX_VARIANT_DIR  = $${QNX_VARIANT_DIR}-g
    DEFINES         += QT_DECLARATIVE_DEBUG
}

CONFIG(release, debug|release) {
    QMAKE_CFLAGS   += -fstack-protector-strong -mthumb -Os
    QMAKE_CXXFLAGS += -fstack-protector-strong -mthumb -Os
    QMAKE_LFLAGS   += -Wl,-z,relro
    DEFINES        += _FORTIFY_SOURCE=2
}

DESTDIR = $${QNX_VARIANT_DIR}

profile {
    QMAKE_CXXFLAGS += -g -finstrument-functions
    LIBS           += -lprofilingS
}

QMAKE_CFLAGS   += -fvisibility=hidden
QMAKE_CXXFLAGS += -fvisibility=hidden

DEFINES += ACTIONTRIGGERCOMMON_LIBRARY
staticlib {
    DEFINES += ACTIONTRIGGERCOMMON_STATIC_LINK
}

OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR     = $${DESTDIR}/moc
RCC_DIR     = $${DESTDIR}/rcc
UI_DIR      = $${DESTDIR}/ui
