#ifndef IFACEFACTORY_H
#define IFACEFACTORY_H

#include "SqliteDatabase.h"
#include "Types.h"
#include "Logger.h"

//TODO: !!!
#undef IS_SAILFISH

#ifdef IS_SAILFISH
#   include <json/json.h>
#else
#   include <jsoncpp/json/json.h>
#endif
#include <experimental/optional>

namespace                         htl   {
namespace                         db    {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        IfaceFactory
{
public:
  /**/                            IfaceFactory    ( const std::string & config_path );
  virtual                       ~ IfaceFactory    () {}

  std::string                     GetConfig       () const;
  bool                            IsValidConfig   () const noexcept;
  virtual bool                    IsValidDatabases() const noexcept;
  htl::db::DatabasesMap::mapped_type GetDb        ( const htl::db::DatabasesMap::key_type & db_mnrmocode );
  db_map_keys_t                   GetDbNames      () const;

protected:
  mutable std::recursive_mutex    m_Mutex;

  virtual void                    SetPath         ( const std::string & config_path ) noexcept;

  template< typename              DBType_ >
  void                            Prepare         () noexcept;

  file_path_t                   & GetConfigRef    ();

protected:
  htl::db::DatabasesMap         & CollectionRef   ( );

private:
  file_path_t                     m_conf_path;
  htl::db::DatabasesMap           m_db_collection;
};

//------------------------------------------------------------------------------

template< typename DBType_ >
void
IfaceFactory::Prepare()
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_db_collection.clear();
  if( IsValidConfig() )
  {
    try
    {
      Json::Value root;
      std::ifstream ( GetConfigRef().string(), std::ifstream::binary) >> root;

      Json::Value val = root.get( root.getMemberNames().at(0), Json::nullValue );
      for( Json::ArrayIndex i(0); val.isArray() and ( i < val.size() ); i++ )
      {
        auto item = val.get(i,Json::objectValue);
        if(item.isNull() )
          continue;

        for( auto str : item.getMemberNames() )
        {
          Json::Value v = item.get( str, Json::nullValue );
          if( not v.isNull() )
          {
            auto iter( m_db_collection.find( str ) );
            if( iter == m_db_collection.end() )
            {
              try
              {
                iter =
                  m_db_collection.emplace(
                                   str,
                                   htl::db::MakeDb<DBType_>( v.asString() ) ).first;
              }
              catch( const sqlite::sqlite_exception & e )
              {
                if( iter != m_db_collection.end() )
                  m_db_collection.erase( iter );
                DUMP_EXCEPTION( e );
              }
            }
          }
        }
      }
    }
    catch( Json::RuntimeError & )
    {
      m_db_collection.clear();
    }
  }
  END_LOCK_SECTION_;
}

}// namespace                     db
}// namespace                     htl

#endif // IFACEFACTORY_H
