include($$PWD/../documents/documents.pri)

WATCH_SOURCES = $$files($$PWD/src/*.cpp)
WATCH_HEADERS = $$files($${INCLUDE_WATCH_PATH}/*.h)

!contains(INCLUDEPATH,$${INCLUDE_WATCH_PATH}) : INCLUDEPATH += $${INCLUDE_WATCH_PATH}

OTHER_FILES += $$PWD/README.md

#QMAKE_CLEAN += \
#  $${target_watch_include}/*.h \

#isEmpty(INOTIFY_COMPLETE) : {
#  INOTIFY_COMPLETE=$$system('/usr/bin/sudo sysctl -w fs.inotify.max_user_watches=6553')
##  message (INOTIFY_COMPLETE=$$INOTIFY_COMPLETE)
#}

!contains(LIBS,-linotifytools) : LIBS += -linotifytools

