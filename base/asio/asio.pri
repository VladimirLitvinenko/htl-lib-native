include($$PWD/../documents/documents.pri)

#DEFINES+=BOOST_COROUTINES_NO_DEPRECATION_WARNING
!contains(QT,network)): QT += network

ASIO_HEADERS = $$files($${INCLUDE_ASIO_PATH}/*.h)
ASIO_SOURCES = $$files($$PWD/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_ASIO_PATH}) : INCLUDEPATH += $${INCLUDE_ASIO_PATH}

#contains(ANDROID_TARGET_ARCH,x86) {
#    ANDROID_EXTRA_LIBS += \
#    -L/usr/include/boost \
#    -lboost_context \
#    -lboost_thread \
#    -lboost_date_time \
#    -lboost_system \
#    -lboost_regex \
#    -lboost_filesystem \
#    -lboost_coroutine \
#        ..........
#} else {
#    }

#QMAKE_CLEAN += \
#  $${target_asio_include.path}/*.h \

OTHER_FILES += \
  $$PWD/README.md
