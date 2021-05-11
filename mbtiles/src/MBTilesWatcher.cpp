#include <MBTilesWatcher.h>
#include <AsioError.h>
#include <Types.h>
#include <iostream>

namespace                         htl   {
namespace                         tiles {

MBTilesWatcher::~MBTilesWatcher() = default;

MBTilesWatcher::MBTilesWatcher(const std::string& path)
  : MBTilesWatcher::base_class_t()
{
  SetPath( path );
}

void
MBTilesWatcher::SetPath( const std::string & path )
noexcept
{
  if( GetDir() == path )
    return;

  BEGIN_LOCK_SECTION_SELF_;

  Stop();
  m_dir.clear();

  boost::system::error_code ec;
  file_path_t p( path );

  auto validate = ( boost::filesystem::is_directory( boost::filesystem::status( p, ec ) ) )
      or
      ( boost::filesystem::exists( p, ec ) );
  if( validate )
  {
    m_dir = path;
  }
  SetError( validate ? ec.value() : boost::asio::error::invalid_argument );

  END_LOCK_SECTION_;
}

file_path_t
MBTilesWatcher::GetDir()
const
{
  return m_dir;
}

file_pathes_t MBTilesWatcher::GetFiles() const
{
  BEGIN_LOCK_SECTION_SELF_;
  file_pathes_t retval;
  for( boost::filesystem::directory_iterator iter( GetDir() );
       iter != boost::filesystem::directory_iterator();
       iter++ )
  {
    if( not boost::filesystem::is_directory( boost::filesystem::status( *iter ) ) )
    {
      retval.push_back( boost::filesystem::path(*iter) );
    }
  }
  return retval;
  END_LOCK_SECTION_;
}

void
MBTilesWatcher::PrepareFiles()
{

}

}// namespace                     watch
}// namespace                     htl
