LIBTARGET = actiontriggercommon

profile {
    LIBTARGETDIR_PROFILE = -p
}
CONFIG(debug, debug|release) {
    LIBTARGETDIR_DEBUG = -g
}
device {
    LIBTARGETDIR = arm$${LIBTARGETDIR_PROFILE}/so.le-v7$${LIBTARGETDIR_DEBUG}
}
simulator {
    LIBTARGETDIR = x86$${LIBTARGETDIR_PROFILE}/so$${LIBTARGETDIR_DEBUG}
}

BASEDIR      = $${PWD}
INCLUDEPATH *= $$quote($${BASEDIR}/include)
DEPENDPATH  *= $$quote($${BASEDIR}/include)
LIBS        += -L$${BASEDIR}/$${LIBTARGETDIR} -l$${LIBTARGET}
