#include "UploadController.h"
#include <QDir>

namespace htl {
namespace qt  {

UploadController::UploadController( QObject * parent )
  : UploadController::base_class_t( parent )
{
}

UploadController::~UploadController()
{
  Stop();
}

bool
UploadController::IsActive()
const
{
  return m_upc.IsActive();
}

QStringList
UploadController::GetPathes()
const
{
  QStringList retval;
  retval.reserve( m_upc.PathesSize() );
  for( auto p_ref : m_upc.GetPathes() )
    retval.push_back( p_ref.c_str() );
  return retval;
}

QString
UploadController::GetPath(const std::size_t idx)
const
{
  return m_upc.GetPath( idx );
}

void
UploadController::AddPath(const QString& path)
{
  m_upc.AddPath( path.toStdString() );
  m_upc.InitHandlers();
}

void
UploadController::AddPathes(const QStringList& pathes)
{
  auto list = m_upc.GetPathes();
  for( auto & p : pathes )
  {
    if( list.end() == std::find_if( list.begin(),
                              list.end(),
                              [ &p ]( const htl::strings_t::value_type & pth )
                              {
                                return pth == p.toStdString();
                              }))
      AddPath( p );
  }
}

void
UploadController::SetPathes(const QStringList& pathes)
{
  bool is_active{ IsActive() };
  Stop();
  for( auto & p : pathes )
    AddPath( p );
  if( is_active )
    Start();
}

void UploadController::SetPathes( QStringList && pathes)
{
  m_upc.GetPathesRef().clear();
  for( auto & p : pathes )
    AddPath( p );
  pathes.clear();
}

void
UploadController::OnPathAdded( htl::watch::UploadNotify in, const QString& path, bool is_dir)
{
  QString abs_file  ( path );
  if( in == htl::watch::UploadNotify::FileAdd )
  {
    std::async( std::launch::async,
                [ this, is_dir, abs_file ]()
                {
                  is_dir
                      ? Q_EMIT this->sgnDirCreated  ( abs_file )
                      : Q_EMIT this->sgnFileCreated ( abs_file );
                } ).wait_for( std::chrono::milliseconds(0));
  }
//  else if( in == htl::watch::UploadNotify::FileRemove )
//  {
//    std::async( std::launch::async,
//                [ this, is_dir, abs_file ]()
//                {
//                  is_dir
//                      ? Q_EMIT this->sgnPathDeleted( abs_file )
//                      : Q_EMIT this->sgnFileDeleted( abs_file );
//                } ).wait_for( std::chrono::milliseconds(0));
//  }
}

void UploadController::Start()
{
  SetConnections();
  m_upc.Start();
}

void UploadController::Stop()
{
  DropConnections();
  m_upc.Stop();
}

void UploadController::SetConnections()
{
  Q_ASSERT( QObject::connect( &m_upc, &UploadPathControl::sgnPathAdded,
                              this  , &UploadController::OnPathAdded,
                              Qt::DirectConnection ) );
  Q_ASSERT( QObject::connect( &m_upc, &UploadPathControl::sgnPathDeleted,
                              this  , &UploadController::sgnPathDeleted,
                              Qt::DirectConnection ) );
}

void UploadController::DropConnections()
{
  m_upc.disconnect();
}

}// namespace qt
}// namespace htl
