include($$PWD/base.pri)

TEMPLATE = lib
TARGET = $${LIB_BASE_NAME}$${PROJECT_SUFFIX}

!contains(HEADERS,$${DOCUMENT_HEADERS}) : HEADERS += $${DOCUMENT_HEADERS}
target_docs_include.path  = $${BUILD_INCLUDE_DOCS_PATH}
target_docs_include.files = $${DOCUMENT_HEADERS}

!contains(HEADERS,$${APP_HEADERS}) : HEADERS += $${APP_HEADERS}
target_app_include.path  = $${BUILD_INCLUDE_APP_PATH}
target_app_include.files = $${APP_HEADERS}

!contains(HEADERS,$${ASIO_HEADERS}) : HEADERS += $${ASIO_HEADERS}
target_asio_include.path  = $${BUILD_INCLUDE_ASIO_PATH}
target_asio_include.files = $${ASIO_HEADERS}

!contains(HEADERS,$${HTTP_HEADERS}) : HEADERS += $${HTTP_HEADERS}
target_http_include.path  = $${BUILD_INCLUDE_BASE_PATH}
target_http_include.files = $${ASIO_HEADERS}

!contains(HEADERS,$${WATCH_HEADERS}) : HEADERS += $${WATCH_HEADERS}
target_watch_include.path  = $${BUILD_INCLUDE_WATCH_PATH}
target_watch_include.files = $${WATCH_HEADERS}

!contains(SOURCES,$$ASIO_SOURCES) : SOURCES += $$ASIO_SOURCES
!contains(SOURCES,$$APP_SOURCES) : SOURCES += $$APP_SOURCES
!contains(SOURCES,$$HTTP_SOURCES) : SOURCES += $$HTTP_SOURCES
!contains(SOURCES,$$WATCH_SOURCES) : SOURCES += $$WATCH_SOURCES

!contains(LIBS,-linotifytools) : LIBS += -linotifytools
!contains(LIBS,-L$${INSTALL_LIB_PATH}) : LIBS += -L$${INSTALL_LIB_PATH}
!contains(LIBS,-l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}


OTHER_FILES += \
  $$PWD/README.md \

target_global_include.path  = $${BUILD_INCLUDE_PATH}
target_global_include.files = $${GLOBAL_HEADERS}

target.path = $${INSTALL_LIB_PATH}

INSTALLS += \
  target \
  target_global_include \
  target_docs_include \
  target_asio_include \
  target_app_include \
  target_watch_include \
  target_http_include \

