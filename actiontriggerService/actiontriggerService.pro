
# Qt uses long long... sigh
QMAKE_CXXFLAGS += -Wall -Werror -Wextra -Wuninitialized -pedantic -Wno-long-long

APP_NAME = actiontriggerService

CONFIG += qt warn_on cascades10 mobility

include(config.pri)

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

QT     += core gui sql

include($$quote($$_PRO_FILE_PWD_)/../actiontriggercommon/static.pri)
