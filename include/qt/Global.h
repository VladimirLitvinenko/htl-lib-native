#ifndef QT_COMMON_GLOBAL_H
#define QT_COMMON_GLOBAL_H

#include <QtGlobal>

#if defined(COMMON_LIBRARY)
#  define COMMON_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMON_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QT_COMMON_GLOBAL_H
