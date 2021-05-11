include($$PWD/common.pri)

TEMPLATE = lib
TARGET = $${LIB_COMMON_NAME}$${PROJECT_SUFFIX}

!contains(HEADERS,$${COMMON_HEADERS}) : HEADERS += $${COMMON_HEADERS}
!contains(SOURCES,$${COMMON_SOURCES}) : SOURCES += $${COMMON_SOURCES}
!contains(LIBS,-L$${INSTALL_LIB_PATH}) : LIBS += -L$${INSTALL_LIB_PATH}
!contains(LIBS,-l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}


target_common_include.path  = $${BUILD_INCLUDE_COMMON_PATH}
target_common_include.files = $${COMMON_HEADERS}

target.path = $${INSTALL_LIB_PATH}

OTHER_FILES += \
    $$PWD/README.md \

INSTALLS += \
  target_common_include \
  target

#QMAKE_CLEAN += \
#    $${target_common_include}/*.h \

