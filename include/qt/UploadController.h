#pragma once

#include "UploadPathControl.h"

namespace htl {
namespace qt  {

//------------------------------------------------------------------------------
/**
 * @brief The UploadController class
 */
class COMMON_SHARED_EXPORT        UploadController
: public                          QObject
{
  Q_OBJECT
public:
  using base_class_t            = QObject;

  explicit                        UploadController  ( QObject * parent = nullptr );
  virtual                       ~ UploadController  ();

  bool                            IsActive          () const;
  QStringList                     GetPathes         () const;
  QString                         GetPath           ( const std::size_t idx = 0 ) const;

  Q_SLOT    void                  AddPath           ( const QString & path );
  Q_SLOT    void                  AddPathes         ( const QStringList & pathes );
  Q_SLOT    virtual void          SetPathes         ( const QStringList & pathes );
  Q_SLOT    virtual void          SetPathes         ( QStringList && pathes );
  Q_SLOT    void                  SetConnections    ();
  Q_SLOT    void                  DropConnections   ();
  Q_SLOT    virtual void          Start             ();
  Q_SLOT    virtual void          Stop              ();

  Q_SIGNAL  void                  sgnDirCreated     ( const QString & ) const;
  Q_SIGNAL  void                  sgnFileCreated    ( const QString & ) const;
  Q_SIGNAL  void                  sgnPathDeleted    ( const QString & ) const;
  Q_SIGNAL  void                  sgnFileDeleted    ( const QString & ) const;

protected:
  Q_SLOT void                     OnPathAdded
  (
      htl::watch::UploadNotify    in,
      const QString             & path,
      bool                        is_dir
  );

private:
  UploadPathControl               m_upc;
};
//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl
