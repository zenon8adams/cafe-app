#ifndef CAFEAPPLIB_GLOBAL_H
#define CAFEAPPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CAFEAPPLIB_LIBRARY)
#  define CAFEAPPLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CAFEAPPLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CAFEAPPLIB_GLOBAL_H