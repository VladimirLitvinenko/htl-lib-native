#ifndef JSONFILESFACTORY_H
#define JSONFILESFACTORY_H

#include "UploadController.h"
#include <QFileInfo>

namespace htl     {
namespace qt      {

//------------------------------------------------------------------------------
enum class                        JSONFilesState
{
  Undefined                     = 0,
  Ok,
  SorcesMissing,
  LayersMissing,
};

QString  StateMessage( htl::qt::JSONFilesState state, const QString & file_name = QString() );

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        JSONFilesFactory
: public                          htl::qt::UploadController
{
  Q_OBJECT
  Q_PROPERTY( QString geo_sources_file      READ GetGeoSourcesFile  NOTIFY sgnGeoSourcesFileChanged )
  Q_PROPERTY( QString geo_layers_file       READ GetGeoLayersFile   NOTIFY sgnGeoLayersFileChanged )
  Q_PROPERTY( htl::qt::JSONFilesState state READ GetState           NOTIFY sgnStateChanged )

public:
  using base_class_t            = htl::qt::UploadController;

  static JSONFilesFactory &       Instance          ();

  QString                         GetGeoSourcesFile () const;
  QString                         GetGeoLayersFile  () const;
  JSONFilesState                  GetState          () const noexcept ;
  QString                         GetStateStr       () const
  {
    return StateMessage( GetState() );
  }

  bool                            IsValid           () const noexcept;

  Q_SLOT    void                  Validate          ();
  Q_SLOT    void                  SetPath           ( const QString & path );
  Q_SLOT    void                  SetPath           ( QString && path );
  Q_SLOT    void                  SetState          ( htl::qt::JSONFilesState state, const QString & file_name = QString() ) noexcept ;
  Q_SLOT    void                  Start             () override;

  Q_SIGNAL  void                  sgnGeoSourcesFileChanged( const QString & );
  Q_SIGNAL  void                  sgnGeoLayersFileChanged ( const QString & );
  Q_SIGNAL  void                  sgnStateChanged         ( htl::qt::JSONFilesState, const QString & = QString() );
  Q_SIGNAL  void                  sgnUpdate               ();

protected:
  explicit                        JSONFilesFactory    ();
  explicit                        JSONFilesFactory    ( const QString & path );

  void                            SetConnections      ();
  void                            SetState            () noexcept;

private:
  htl::qt::JSONFilesState         m_state             { htl::qt::JSONFilesState::Undefined };
  QFileInfo                       m_geo_src_file;
  QFileInfo                       m_geo_lrs_file;
};

//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl

Q_DECLARE_METATYPE( htl::qt::JSONFilesState )

#endif // JSONFILESFACTORY_H
