#include "UploadPathControl.h"
#include "Logger.h"
#include <QFileInfo>
#include <future>

namespace htl {
namespace qt  {

UploadPathControl::UploadPathControl( QObject * parent )
  : UploadPathControl::base_class_t( parent )
  , UploadPathControl::inot_class_t()
{
  SetRecurcive( false );
}

UploadPathControl::UploadPathControl( const QStringList & p, QObject * parent )
  : UploadPathControl::UploadPathControl( parent )
{
  SetPathes( p );
}

UploadPathControl::UploadPathControl( QStringList && p, QObject * parent)
  : UploadPathControl::UploadPathControl( parent )
{
  SetPathes( std::move( p ) );
}

UploadPathControl::~UploadPathControl()
{

}

void
UploadPathControl::SetPathes( const QStringList & p)
noexcept
{
  bool is_active = IsActive();
  Stop();
  GetPathesRef().clear();
  for( auto & p_ref : p )
    GetPathesRef().push_back( p_ref.toStdString() );
  InitHandlers();
  if( is_active )
    Start();
}

void
UploadPathControl::SetPathes( QStringList && p)
noexcept
{
  bool is_active = IsActive();
  Stop();
  GetPathesRef().clear();
  for( auto & p_ref : p )
    GetPathesRef().push_back( p_ref.toStdString() );
  InitHandlers();
  p.clear();
  if( is_active )
    Start();
}

QString
UploadPathControl::GetPath(const std::size_t idx )
const
{
  auto p( GetPathes() );
  return
      p.empty()
      ? QString()
      : QString::fromStdString( p.at( idx < p.size() ? idx  : 0 ) );
}

void
UploadPathControl::InitHandlers(const QStringList & pathes )
noexcept
{
  using namespace htl::watch;
  NotifyFlags usage_flags =
      static_cast<NotifyFlags>(
        static_cast< int >(NotifyFlags::MovedTo) |
        static_cast< int >(NotifyFlags::MovedFrom) |
        static_cast< int >(NotifyFlags::CloseAfterWrite ) |
//        static_cast< int >(NotifyFlags::Close ) |
        static_cast< int >(NotifyFlags::Delete) |
        static_cast< int >(NotifyFlags::Create) |
        static_cast< int >(NotifyFlags::DeleteSelf) );

  SetFlags( usage_flags );

  SetEventHandler(
        [this](const NotifyFlags f, const event_t & ev )
        {
          if( ( not ev.m_name.empty() ) and (ev.m_name.at(0) != '.') )
          {
            if( 0 != (static_cast<int>(ev.m_flags) & static_cast<int>(f)) )
            {
//              std::async( std::launch::async,
//                          [ this, ev ]()
//              {
                QString abs_path( QString::fromStdString( ev.absolute_path() ) );

                auto is_dir( QFileInfo( abs_path ).isDir() );

                if( flag_exists( ev.m_flags, NotifyFlags::MovedFrom )
                    or
                    flag_exists( ev.m_flags, NotifyFlags::Delete )
                    or
                    flag_exists( ev.m_flags, NotifyFlags::DeleteSelf ) )
                {
                  Q_EMIT this->sgnPathDeleted( abs_path );
                }
                else
                {
                  if( flag_exists( ev.m_flags, NotifyFlags::MovedTo )
                         or
                         ( flag_exists( ev.m_flags, NotifyFlags::Create ) and is_dir )
                         or
                         ( flag_exists( ev.m_flags, NotifyFlags::CloseAfterWrite ) and (not is_dir) ) )
                  {
                    Q_EMIT this->sgnPathAdded( UploadNotify::FileAdd, abs_path, is_dir );
                  }
                }
//              } ).wait_for( std::chrono::milliseconds(0));
            }
          }
        }, usage_flags, event_t() );

  for( auto & p_ref : pathes )
    GetPathesRef().push_back( p_ref.toStdString() );
}

}// namespace qt
}// namespace htl
