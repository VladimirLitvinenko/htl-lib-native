#pragma once

#include "base/watchers/InotifyManager.h"
#include "Types.h"

#include <QObject>
#include <QMetaType>

namespace htl {
namespace qt  {

class COMMON_SHARED_EXPORT        UploadController;

class COMMON_SHARED_EXPORT        UploadPathControl
: public                          QObject
, public                          htl::watch::InotifyManager
{
  Q_OBJECT
  friend class                    UploadController;

public:
  using base_class_t            = QObject;
  using inot_class_t            = htl::watch::InotifyManager;

  explicit                        UploadPathControl( QObject * parent = nullptr );
  explicit                        UploadPathControl( const QStringList & p, QObject * parent = nullptr );
  explicit                        UploadPathControl( QStringList && p, QObject * parent = nullptr );
  virtual                       ~ UploadPathControl();

  virtual void                    SetPathes         ( const QStringList & p ) noexcept;
  virtual void                    SetPathes         ( QStringList && p ) noexcept;
  QString                         GetPath           ( const std::size_t idx = 0 ) const;

  Q_SIGNAL  void                  sgnPathAdded      ( htl::watch::UploadNotify, const QString & path, bool is_dir ) const;
  Q_SIGNAL  void                  sgnPathDeleted    ( const QString & path ) const;

  Q_SLOT void                     InitHandlers      ( const QStringList & pathes = QStringList() ) noexcept;
};

}// namespace qt
}// namespace htl

