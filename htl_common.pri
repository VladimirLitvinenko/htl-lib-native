# ------------------------------------------------------------------------------
#
# Необходимо:
# 1.
# sudo apt install debian-archive-keyring dirmngr
# sudo apt install dirmngr
# sudo apt-key adv --recv-keys --keyserver keys.gnupg.net EF0F382A1A7B6500
#
# sudo -s
# echo "deb https://mirror.yandex.ru/debian/ stretch main contrib non-free" >> /etc/apt/sources.list
# apt update
# exit
#
# 2.
# sudo apt-get install libvsqlitepp-dev libqudev-dev libinotifytools0-dev libjsoncpp-dev libjboost-all-dev
#
# ------------------------------------------------------------------------------
# Установка общих переменных системы
#


MACHENE_NAME = $$system(uname -m)
isEmpty(MACHENE_NAME) : error('Невозможно определить архитектуру операционной системы.')

DEFINES -= IS_SAILFISH
IS_SAILFISH=
equals(MACHENE_NAME,"armv7l"): IS_SAILFISH=1
isEmpty(IS_SAILFISH):
equals(MACHENE_NAME,"i486"): IS_SAILFISH=1

!isEmpty(IS_SAILFISH) {
    !contains(DEFINES, IS_SAILFISH) : DEFINES += IS_SAILFISH
    USER_NAME = mersdk
} else {
    DEFINES -= IS_SAILFISH
    USER_NAME = $$system(id -un)
    isEmpty(USER_NAME) : error('Невозможно определить текущего пользователя системы.')
}

CPU_ARCHITECT = $$find(MACHENE_NAME, 64)
CPU_ARCHITECT = 64

isEmpty(UNAME_INFO) || isEmpty(OS_RELEASE) {
  UNAME_INFO = $$system(uname -v)
#  OS_RELEASE = $$find(UNAME_INFO, astra)
#  !isEmpty (OS_RELEASE): OS_RELEASE = astra
}
#isEmpty (OS_RELEASE):  error('Операционная система не поддерживает сборку проекта.')

USER_PATH = /home/$${USER_NAME}
!exists($$USER_PATH) : error('Домашний Каталог $${USER_PATH} пользователя $${USER_NAME} отсутствует.')

INCLUDE_PATH                = $$PWD/include
INCLUDE_SPDLOG_PATH         = $${INCLUDE_PATH}/spdlog
INCLUDE_COMMON_PATH         = $${INCLUDE_PATH}/common
INCLUDE_BASE_PATH           = $${INCLUDE_PATH}/base
INCLUDE_NMEA_PATH           = $${INCLUDE_PATH}/nmea
INCLUDE_ENVOY_DB_PATH       = $${INCLUDE_PATH}/envoy/db
INCLUDE_APP_PATH            = $${INCLUDE_BASE_PATH}/application
INCLUDE_ASIO_PATH           = $${INCLUDE_BASE_PATH}/asio
INCLUDE_HTTP_PATH           = $${INCLUDE_BASE_PATH}/http
INCLUDE_DOCS_PATH           = $${INCLUDE_BASE_PATH}/documents
INCLUDE_HTTP_PATH           = $${INCLUDE_BASE_PATH}/http
INCLUDE_WATCH_PATH          = $${INCLUDE_BASE_PATH}/watchers
INCLUDE_MBTILES_PATH        = $${INCLUDE_PATH}/mbtiles
INCLUDE_QT_PATH             = $${INCLUDE_PATH}/qt
INCLUDE_QT_APP_PATH         = $${INCLUDE_QT_PATH}/application
INCLUDE_QT_DB_PATH          = $${INCLUDE_QT_PATH}/db
INCLUDE_DB_PATH             = $${INCLUDE_PATH}/db
INCLUDE_SQLITE3_PATH        = $${INCLUDE_DB_PATH}/sqlite3
INCLUDE_QT_APP_PATH         = $${INCLUDE_QT_PATH}/application
INCLUDE_SERVER_PATH         = $${INCLUDE_PATH}/server

GLOBAL_HEADERS = $${INCLUDE_PATH}/HtlGlobal
!contains(HEADERS,$${GLOBAL_HEADERS}) : HEADERS += $${GLOBAL_HEADERS}

!contains(INCLUDEPATH,$${INCLUDE_PATH}) : INCLUDEPATH += $${INCLUDE_PATH}

BUILD_PATH                  = $$USER_PATH/build/$${ORGANISATION_NAME}
BUILD_LIB_PATH              = $${BUILD_PATH}/lib
BUILD_BIN_PATH              = $${BUILD_PATH}/bin
BUILD_INCLUDE_PATH          = $${BUILD_PATH}/include
BUILD_INCLUDE_COMMON_PATH   = $${BUILD_INCLUDE_PATH}/common
BUILD_INCLUDE_BASE_PATH     = $${BUILD_INCLUDE_PATH}/base
BUILD_INCLUDE_NMEA_PATH     = $${BUILD_INCLUDE_PATH}/nmea
BUILD_INCLUDE_SPDLOG_PATH   = $${BUILD_INCLUDE_PATH}/spdlog
BUILD_INCLUDE_APP_PATH      = $${BUILD_INCLUDE_BASE_PATH}/application
BUILD_INCLUDE_ASIO_PATH     = $${BUILD_INCLUDE_BASE_PATH}/asio
BUILD_INCLUDE_DOCS_PATH     = $${BUILD_INCLUDE_BASE_PATH}/documents
BUILD_INCLUDE_HTTP_PATH     = $${BUILD_INCLUDE_BASE_PATH}/http
BUILD_INCLUDE_WATCH_PATH    = $${BUILD_INCLUDE_BASE_PATH}/watchers
BUILD_INCLUDE_MBTILES_PATH  = $${BUILD_INCLUDE_PATH}/mbtiles
BUILD_INCLUDE_QT_PATH       = $${BUILD_INCLUDE_PATH}/qt
BUILD_INCLUDE_QT_APP_PATH   = $${BUILD_INCLUDE_QT_PATH}/application
BUILD_INCLUDE_QT_DB_PATH    = $${BUILD_INCLUDE_QT_PATH}/db
BUILD_INCLUDE_DB_PATH       = $${BUILD_INCLUDE_PATH}/db
BUILD_INCLUDE_SQLITE3_PATH  = $${BUILD_INCLUDE_DB_PATH}/sqlite3
BUILD_INCLUDE_SERVER_PATH   = $${BUILD_INCLUDE_PATH}/server
BUILD_EXAMPLES_DIR          = $${BUILD_PATH}/examples

DEPENDPATH  += $${BUILD_LIB_PATH}
# ------------------------------------------------------------------------------
# Переменные конфигурации файлов проекта
#
ORGANISATION_NAME     = htl_project
INSTALL_PATH          = $$USER_PATH/install
INSTALL_PROJECT_PATH  = $${INSTALL_PATH}/$${ORGANISATION_NAME}
INSTALL_BIN_PATH      = $${INSTALL_PROJECT_PATH}/bin
INSTALL_LIB_PATH      = $${INSTALL_PROJECT_PATH}/lib
INSTALL_CONF_PATH     = $${INSTALL_PROJECT_PATH}/conf
INSTALL_LOG_PATH      = $${INSTALL_PROJECT_PATH}/log
INSTALL_STYLES_PATH   = $${INSTALL_PROJECT_PATH}/styles

# ------------------------------------------------------------------------------
# Установка параметров сборк
#
CONFIG += c++14

!contains(DEFINES,QT_DEPRECATED_WARNINGS ): DEFINES += QT_DEPRECATED_WARNINGS
!contains(DEFINES,QT_DISABLE_DEPRECATED_BEFORE): DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG ( debug, release | debug ) : PROJECT_SUFFIX = -dbg

#CONFIG +=sailfishapp_no_deploy_qml

QMAKE_LFLAGS_RPATH=
isEmpty(QMAKE_LFLAGS) : QMAKE_LFLAGS += "-Wl,-rpath,\'$${INSTALL_LIB_PATH}\'"

!contains(LIBS,-lrt) : LIBS += -lrt
!contains(LIBS,-lpthread) : LIBS += -lpthread
!contains(LIBS,-lz) : LIBS += -lz
#!contains(LIBS,-lcurl) : LIBS += -lcurl
!contains(LIBS,-lsqlite3) : LIBS += -lsqlite3
!contains(LIBS,-ljsoncpp) : LIBS += -ljsoncpp

!contains(LIBS,-lboost_date_time) : LIBS += -lboost_date_time
!contains(LIBS,-lboost_system) : LIBS += -lboost_system
!contains(LIBS,-lboost_regex) : LIBS += -lboost_regex
!contains(LIBS,-lboost_filesystem) : LIBS += -lboost_filesystem
!contains(LIBS,-lboost_container) : LIBS += -lboost_container

#message(LIBS = $$LIBS)

QUAZIP_VER = 5
contains(DEFINES,"IS_SAILFISH") {
message(  contains(DEFINES,"IS_SAILFISH") )
  QUAZIP_VER =
}
!contains(LIBS,-lquazip$${QUAZIP_VER}) : LIBS += -lquazip$${QUAZIP_VER}
!contains(LIBS,-linotifytools) : LIBS += -linotifytools

DISTFILES += \
  $$PWD/README.md
