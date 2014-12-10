APP_NAME = actiontriggercommon

CONFIG += staticlib qt warn_on mobility

MOBILITY += sensors

LIBS   += -laudio_manager \
          -lc \
          -lbps \
          -lbtapi \
          -lbb \
          -lbbpim \
          -lbbmultimedia \
          -lbbdevice \
          -lbbsystem \
          -lbbplatform \
          -lbbdata \
          -lclipboard \
          -lcamapi \
          -lids \
          -lnfc \
          -lnfc_bps \
          -lsensor \
          -lwifi
QT     += core sql gui

# uncomment for generating separate debug info
# CONFIG += separate_debug_info

include(cascades10_library.pri)

include(config.pri)

# specify a list of .ts files for supported languages
# TRANSLATIONS += $${TARGET}_en_US.ts

# uncomment following line to include translations to binary
# RESOURCES += translations/qm/actiontriggercommon_translations.qrc
