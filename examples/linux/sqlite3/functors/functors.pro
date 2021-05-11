APP_NAME = functors

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
