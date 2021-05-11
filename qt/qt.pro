include ($$PWD/qt.pri)

TEMPLATE = lib
#CONFIG += staticlib
TARGET = $${LIB_QT_NAME}$${PROJECT_SUFFIX}

!contains(HEADERS,$${QT_APP_HEADERS}) : HEADERS += $${QT_APP_HEADERS}
!contains(HEADERS,$${QT_DB_HEADERS}) : HEADERS += $${QT_DB_HEADERS}
!contains(HEADERS,$${QT_HEADERS}) : HEADERS += $${QT_HEADERS}

!contains(SOURCES,$${QT_SOURCES}) : SOURCES += $${QT_SOURCES}
!contains(SOURCES,$${QT_DB_SOURCES}) : SOURCES += $${QT_DB_SOURCES}

!contains(LIBS,-L$${INSTALL_LIB_PATH}) : LIBS += -L$${INSTALL_LIB_PATH}
!contains(LIBS,-l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_DB_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_DB_NAME}$${PROJECT_SUFFIX}

target_qt_include.path  = $${BUILD_INCLUDE_QT_PATH}
target_qt_include.files = $${QT_HEADERS}
!contains(INSTALLS,target_qt_include) : INSTALLS += target_qt_include

target_qt_app_include.path  = $${BUILD_INCLUDE_QT_APP_PATH}
target_qt_app_include.files = $${QT_APP_HEADERS}
!contains(INSTALLS,target_qt_app_include) : INSTALLS += target_qt_app_include

target_qt_db_include.path  = $${BUILD_INCLUDE_QT_DB_PATH}
target_qt_db_include.files = $${QT_DB_HEADERS}
!contains(INSTALLS,target_qt_db_include) : INSTALLS += target_qt_db_include

target.path = $${INSTALL_LIB_PATH}
!contains(INSTALLS,target) : INSTALLS += target

OTHER_FILES += \
  $$PWD/README.md \
  $$files($$PWD/rpm/*) \
  $$files($$PWD/files/*) \
