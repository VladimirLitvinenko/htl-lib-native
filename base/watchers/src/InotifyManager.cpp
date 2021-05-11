#include <InotifyManager.h>
#include <Logger.h>
#include <Utils.h>
#include <linux/limits.h>
#include <type_traits>
#include <boost/filesystem.hpp>
#include <boost/bind/bind.hpp>
#include <future>

namespace                         htl   {
namespace                         watch {

//------------------------------------------------------------------------------
/**
 * @brief InotifyManager::InotifyManager
 */
InotifyManager::~InotifyManager()
{
  Stop();
}

/**
 * @brief InotifyManager::IsValid
 * @return
 */
//bool
//InotifyManager::IsValid()
//const
//{
//  return m_Handlers.m_OnEvent.operator bool();
//}

bool
InotifyManager::IsActive()
const
{
  return m_inotify_descriptor > 0;
}

bool
InotifyManager::IsRecurcive()
const
{
  return m_recurcive.load();
}

void
InotifyManager::SetRecurcive(bool recursive)
{
  bool is_active = IsActive();
  Stop();
  m_recurcive.store( recursive );
  if( is_active )
    Start();
}

/**
 * @brief InotifyManager::Stop
 */
void
InotifyManager::Stop()
noexcept
{
  if( m_Handlers.m_OnStop )
  {
    m_Handlers.m_OnStop->Run();
  }
  m_inotify_descriptor = 0;
  inotifytools_cleanup();
}

/**
 * @brief InotifyManager::DumpStdString
 * @param flags
 * @return
 */
std::string
InotifyManager::DumpStdString( NotifyFlags flags )
{
  std::string retval;
  for( auto pair : std::vector< std::pair<NotifyFlags, std::string > >
       ({
          { NotifyFlags::Access           , "ACCESS " },
          { NotifyFlags::Modify           , "MODIFY "},
          { NotifyFlags::Attributes       , "ATTRIB "},
          { NotifyFlags::CloseAfterWrite  , "CLOSE_WRITE "},
          { NotifyFlags::CloseAfterNoWrite, "CLOSE_NOWRITE "},
          { NotifyFlags::Open             , "OPEN "},
          { NotifyFlags::Close            , "CLOSE "},
          { NotifyFlags::MovedFrom        , "MOVED_FROM_PLACE "},
          { NotifyFlags::MovedTo          , "MOVED_TO_NEW_PLACE "},
          { NotifyFlags::Create           , "CREATE "},
          { NotifyFlags::Delete           , "DELETE "},
          { NotifyFlags::DeleteSelf       , "DELETE_SELF "},
          { NotifyFlags::MoveSelf         , "MOVE_SELF "},
          { NotifyFlags::NonBlock         , "NONBLOCK "},
          { NotifyFlags::FileWasIgnored   , "FILE_IGNORED "},
      }) )
  {
    if( ( static_cast<int>(flags) & static_cast<int>( pair.first ) ) != 0 )
      retval += std::string("\t") + pair.second + "\n";
  }
  return retval;
}

/**
 * @brief InotifyManager::Flags
 * @return
 */
NotifyFlags
InotifyManager::Flags()
const noexcept
{
  return m_Flags;
}

/**
 * @brief InotifyManager::SetError
 * @param error
 */
void
InotifyManager::SetError( int error )
{
  if( m_Handlers.m_OnError )
  {
    ( * m_Handlers.m_OnError )( error, strerror( error ) ).Run();
  }
}

/**
 * @brief InotifyManager::SetFlags
 * @param flags
 */
void
InotifyManager::SetFlags( NotifyFlags flags )
noexcept
{
  m_Flags = flags;
}

/**
 * @brief InotifyManager::AddFlag
 * @param flag
 */
void
InotifyManager::AddFlag( NotifyFlags flag )
noexcept
{
  bool is_active = IsActive();
  Stop();
  m_Flags =
      static_cast< NotifyFlags >(
        static_cast< int >(m_Flags) |
        static_cast< int >(flag) );
  if( is_active )
    Start();
}

void
InotifyManager::AddPath(const std::string & p) noexcept
{
  if( PathExists( p ) )
    return;

  bool is_active = IsActive();
  Stop();
  m_pathes.push_back( p ) ;
  if( is_active )
    Start();
}

strings_t
InotifyManager::GetPathes() const
{
  return m_pathes;
}

std::size_t
InotifyManager::PathesSize()
const
{
  return m_pathes.size();
}

bool
InotifyManager::PathExists(const std::string& path)
const
{
  return m_pathes.cend() != std::find( m_pathes.cbegin(), m_pathes.cend(), path );
}

void
InotifyManager::SetExcluded(const strings_t& excluded)
noexcept
{
  bool is_active = IsActive();
  Stop();
  m_excluded = excluded;
  if( is_active )
    Start();
}

void
InotifyManager::SetIgnore(const std::string& ignore_by_regex)
noexcept
{
  bool is_active = IsActive();
  Stop();
  m_ignore_by_regex = ignore_by_regex;
  if( is_active )
    Start();
}

/**
 * @brief InotifyManager::DumpFlags
 * @param flags
 */
void
InotifyManager::DumpFlags( NotifyFlags flags )
{
  DUMP_INFO( std::string("\nINotify Flags:\n") + DumpStdString(flags) );
}

strings_t&
InotifyManager::GetPathesRef()
{
  return std::ref( m_pathes );
}

/**
 * @brief InotifyManager::Start
 * @param excluded
 * @param ignore_by_regex
 */
void
InotifyManager::Start(const htl::strings_t& pathes)
{
  if( not pathes.empty() )
    m_pathes = pathes;

  if( not Init( IsRecurcive() ) )
  {
    SetError(EUCLEAN);
    return;
  }
  SetRunHandler(
        []( InotifyManager & inm )
        {
          for(
              auto event( inotifytools_next_event( -1 ) );

              inm.IsActive()
              and
              inm.m_Handlers.m_OnEvent.operator bool()
              and ( nullptr != event );

              event = inotifytools_next_event( 0 ) )
          {
            if( 0 == inotifytools_error() )
            {
              event_t e( event );
              if( not e.m_name.empty() )
              ( * inm.m_Handlers.m_OnEvent )( e ).Run();
            }
          }
    }, std::ref( *this ) );
  if( not m_thread_runner )
  {
    SetError(ENOEXEC);
    return;
  }
  m_thread_runner->detach ();
}

/**
 * @brief InotifyManager::Init
 * @param excluded
 * @param ignore_by_regex
 * @return
 */
bool
InotifyManager::Init( bool recursive )
noexcept
{
  if( IsActive() )
    inotifytools_cleanup();

  bool retval( 0 != static_cast<int>(Flags()) );
  if( retval )
  {
    m_inotify_descriptor = inotifytools_initialize();
    retval =  IsActive();
  }

  if( retval )
  {
    inotifytools_initialize_stats();

    auto exc_size( m_excluded.size() );
    std::vector<const char*> excl_array(exc_size + 1, nullptr);
    try
    {
      for( std::size_t idx(0); idx < exc_size; idx++ )
      {
        excl_array[idx] = const_cast< char* >( const_cast<strings_t &>(m_excluded)[ idx ].data() );
      }

      for( auto iter( m_pathes.begin()); retval and (iter != m_pathes.end()); ++iter )
      {
        if( retval )
        {
          auto str_ptr = iter->c_str();
          retval = nullptr != str_ptr;
          if( not retval )
            break;

          int events = static_cast<int>(m_Flags);
          retval =
              1 == ( m_excluded.empty()
                     ? recursive
                       ? inotifytools_watch_recursively( str_ptr, events )
                       : inotifytools_watch_file( str_ptr, events )
                     : inotifytools_watch_recursively_with_exclude( str_ptr,
                                                                    events,
                                                                    excl_array.data() ) );
          if( retval
              and
              ( not m_ignore_by_regex.empty() )
              and
              ( nullptr != m_ignore_by_regex.data() )
          )
          {
            retval = 1 == inotifytools_ignore_events_by_regex(
                       m_ignore_by_regex.c_str(),
                       IN_ALL_EVENTS );
          }
        }
      }
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION( e );
      Stop();
    }
  }

  if( not retval )
    SetError( errno );

  return retval;
}

}// namespace                     watch
}// namespace                     htl

