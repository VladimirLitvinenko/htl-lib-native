#ifndef BASE_COMMON_GLOBAL_H
#define BASE_COMMON_GLOBAL_H

#if defined(QT_GUI_LIB) or defined(QT_CORE_LIB)
//#if defined(QT_CORE_LIB)
# include <QtCore/QtGlobal>
#else
# define Q_DECL_EXPORT     __attribute__((visibility("default")))
# define Q_DECL_IMPORT     __attribute__((visibility("default")))
# define Q_DECL_HIDDEN     __attribute__((visibility("hidden")))
#endif

#if defined(COMMON_LIBRARY)
#  define COMMON_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMON_SHARED_EXPORT Q_DECL_IMPORT
#endif

#if __cplusplus > 201703L
# define EXPORT_MODULE(module_name) export module module_name
# define IMPORT_MODULE(module_name) import module module_name
#else
# define EXPORT_MODULE(module_name)
# define IMPORT_MODULE(module_name)
#endif

#endif // BASE_COMMON_GLOBAL_H
