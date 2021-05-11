#include "IfaceFactory.h"
#include "Logger.h"

namespace                         htl   {
namespace                         db    {

IfaceFactory::IfaceFactory(const std::string& config_path)
{
  IfaceFactory::SetPath( config_path );
}

std::string
IfaceFactory::GetConfig()
const
{
  return m_conf_path.string();
}

bool
IfaceFactory::IsValidConfig()
const noexcept
{
  boost::system::error_code ec;
  return
    boost::filesystem::exists( m_conf_path, ec )
    and
    ( not boost::filesystem::is_empty( m_conf_path, ec ) )
    and
    ( ec == boost::system::errc::success );
}

bool
IfaceFactory::IsValidDatabases()
const noexcept
{
  return not m_db_collection.empty();
}

void
IfaceFactory::SetPath(const std::string& config_path)
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_conf_path = config_path;
  END_LOCK_SECTION_;
}


file_path_t&
IfaceFactory::GetConfigRef()
{
  return std::ref( m_conf_path );
}

htl::db::DatabasesMap::mapped_type
IfaceFactory::GetDb(const htl::db::DatabasesMap::key_type& db_mnrmocode )
{
  auto iter( m_db_collection.find( db_mnrmocode ) );
  return
    iter == m_db_collection.end()
      ? htl::db::DatabasesMap::mapped_type()
      : iter->second;
}

db_map_keys_t
IfaceFactory::GetDbNames() const
{
  db_map_keys_t retval( m_db_collection.size() );
  for( auto & db_ref : m_db_collection)
  {
    retval.push_back( db_ref.first );
  }
  return retval;
}

db::DatabasesMap&
IfaceFactory::CollectionRef()
{
  return std::ref( m_db_collection )  ;
}

}// namespace                     db
}// namespace                     htl
