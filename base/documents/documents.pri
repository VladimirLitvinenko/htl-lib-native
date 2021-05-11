include($$PWD/../../htl_common.pri)

DOCUMENT_HEADERS = $$files($${INCLUDE_DOCS_PATH}/*.h)

!contains(INCLUDEPATH,$${INCLUDE_DOCS_PATH}) : INCLUDEPATH += $${INCLUDE_DOCS_PATH}

OTHER_FILES += \
  $$PWD/README.md

#QMAKE_CLEAN += \
#  $${target_docs_include.path}/*.h \


