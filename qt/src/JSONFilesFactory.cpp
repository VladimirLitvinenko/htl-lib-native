#include "JSONFilesFactory.h"
#include "Logger.h"

#include <QDir>

namespace htl     {
namespace qt      {

static QMap<JSONFilesState, QString > json_state_str
{
  { JSONFilesState::Undefined     , QString("Состояние не определено.%1") },
  { JSONFilesState::Ok            , QString("Файлы в порядке.%1") },
  { JSONFilesState::LayersMissing , QString("Файл \"%1\" набора слоев отсутствует.") },
  { JSONFilesState::SorcesMissing , QString("Файл \"%1\" набора исходников отсутствует.") },
};

QString StateMessage(JSONFilesState state, const QString & file_name )
{
  return json_state_str.value( state, QString("%1") ).arg(file_name);
}

//------------------------------------------------------------------------------

JSONFilesFactory::JSONFilesFactory  ()
  : JSONFilesFactory::base_class_t  ( Q_NULLPTR )
{
  SetConnections();
  SetPath ( QString::fromStdString(htl::GEO_UPLOAD) );
}

JSONFilesFactory::JSONFilesFactory  ( const QString & path )
  : JSONFilesFactory::base_class_t  ( Q_NULLPTR )
{
  SetConnections();
  SetPath ( path );
}

void JSONFilesFactory::SetConnections()
{
  Q_ASSERT( QObject::connect( this, &htl::qt::JSONFilesFactory::sgnStateChanged,
                              [ & ]( htl::qt::JSONFilesState state, const QString & file_name )
                              {
                                DUMP_CRITICAL( StateMessage( state, file_name ).toStdString() );
                              } ) );
  Q_ASSERT( QObject::connect( this, &htl::qt::JSONFilesFactory::sgnPathDeleted,
                              []( const QString & path )
                              {
                                QFileInfo fi( path );
                                if( fi.isDir() )
                                  QDir().mkpath( fi.absolutePath() );
                              } ) );
  Q_ASSERT( QObject::connect( this, &htl::qt::JSONFilesFactory::sgnFileCreated,
                              this, &htl::qt::JSONFilesFactory::Validate ) );
  Q_ASSERT( QObject::connect( this, &htl::qt::JSONFilesFactory::sgnFileDeleted,
                              this, &htl::qt::JSONFilesFactory::Validate ) );
}

void
JSONFilesFactory::SetState( htl::qt::JSONFilesState state, const QString & file_name  ) noexcept
{
  m_state = state;
  std::async( std::launch::async, [this, file_name]()
  {
    Q_EMIT this->sgnStateChanged( this->GetState(), file_name );
  } ).wait_for( std::chrono::milliseconds(0));
}

void
JSONFilesFactory::Start()
{
  Validate();
  JSONFilesFactory::base_class_t::Start();
}

QString
JSONFilesFactory::GetGeoSourcesFile()
const
{
  return m_geo_src_file.absoluteFilePath();
}
QString
JSONFilesFactory::GetGeoLayersFile()
const
{
  return m_geo_lrs_file.absoluteFilePath();
}

JSONFilesState
JSONFilesFactory::GetState()
const noexcept
{
  return m_state;
}

bool
JSONFilesFactory::IsValid()
const noexcept
{
  return GetState() == htl::qt::JSONFilesState::Ok;
}

void
JSONFilesFactory::Validate()
{
  SetState(
        QFile::exists( m_geo_lrs_file.absoluteFilePath() )
        ? htl::qt::JSONFilesState::Ok
        : htl::qt::JSONFilesState::LayersMissing,
        m_geo_lrs_file.absoluteFilePath() );
  if( IsValid() )
  {
    SetState(
          QFile::exists( m_geo_src_file.absoluteFilePath() )
          ? htl::qt::JSONFilesState::Ok
          : htl::qt::JSONFilesState::SorcesMissing,
          m_geo_src_file.absoluteFilePath());
  }
  if( IsValid() )
    std::async( std::launch::async, [this]()
    {
      Q_EMIT this->sgnUpdate();
    } ).wait_for( std::chrono::milliseconds(0));
}

void
JSONFilesFactory::SetPath(const QString & path )
{
  bool is_active{ IsActive() };
  Stop();

  JSONFilesFactory::base_class_t::SetPathes( { path } );
  auto current_path( GetPath( 0 ) );
  bool is_dir{ QFileInfo( current_path ).isDir() };
  m_geo_src_file.setFile( is_dir
                     ? current_path + DIR_SEPARATOR + QString::fromStdString(htl::GEOJSON_SRC_FILE )
                     : current_path );
  m_geo_lrs_file.setFile( is_dir
                    ? current_path + DIR_SEPARATOR + QString::fromStdString(htl::GEOJSON_LRS_FILE )
                    : current_path );
  if( is_active )
    Start();
  else
    Validate();
}

void
JSONFilesFactory::SetPath(QString && path )
{
  bool is_active{ IsActive() };
  Stop();

  JSONFilesFactory::base_class_t::SetPathes( std::move( QStringList() << path ) );
  auto current_path( GetPath( 0 ) );
  bool is_dir{ QFileInfo( current_path ).isDir() };
  m_geo_src_file.setFile( is_dir
                     ? current_path + DIR_SEPARATOR + QString::fromStdString(htl::GEOJSON_SRC_FILE )
                     : current_path );
  m_geo_lrs_file.setFile( is_dir
                    ? current_path + DIR_SEPARATOR + QString::fromStdString(htl::GEOJSON_LRS_FILE )
                    : current_path );
  if( is_active )
    Start();
  else
    Validate();
}

JSONFilesFactory& JSONFilesFactory::Instance()
{
  static JSONFilesFactory retval( QString::fromStdString(htl::GEO_UPLOAD) );
  return std::ref( retval );
}

//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl
