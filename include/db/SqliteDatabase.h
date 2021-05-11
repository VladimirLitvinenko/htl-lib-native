#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include "HtlGlobal"
#include "common/Types.h"
#include "common/ErrLiterals.h"
#include "common/Utils.h"
#include "db/DBCommon.h"

#include <memory>
#include <string>
#include <type_traits>

namespace                         htl {
namespace                         db  {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        SqliteDatabase
{
public:
  /**/                            SqliteDatabase() = default;
  /**/                            SqliteDatabase( const std::string & db_name );
  virtual                       ~ SqliteDatabase();

  bool                            IsDbValid     () const;
  bool                            IsValid       () const;
  bool                            IsOk          () const;
  void                            SetState      ( htl::db::DBState state, const std::string & err_msg = "" ) const;
  htl::db::DBState                GetState      () const;
  std::string                     StateMessage  () const;
  std::string                     GetDbName     () const;

  void                            Close         () noexcept;
  htl::db::sqlitedb_ptr         & DbRef         () noexcept;


protected :
  virtual void                    SetName       ( const std::string & db_name ) noexcept;

private:
  std::string                     m_name;
  htl::db::sqlitedb_ptr           m_db_ptr;
  mutable htl::db::DBState        m_state       { htl::db::DBState::NoDatabase };
};

//------------------------------------------------------------------------------
using SharedSqliteDb            = std::shared_ptr< htl::db::SqliteDatabase >;

template< typename DBType_ >
htl::db::SharedSqliteDb           MakeDb (  const std::string & db_name )
{
  static_assert ( std::is_base_of< htl::db::SqliteDatabase,
                  typename std::decay<DBType_>::type >::value,
                  ST_ASSERT_TEMPLATE_VALIDATOR );

  return
      std::static_pointer_cast<htl::db::SharedSqliteDb::element_type >(
        std::make_shared< DBType_ >( db_name ) );
}

//------------------------------------------------------------------------------

/**
 * @typedef DatabasesMap
 * @brief Упорядоченный по ключу контейнер SQLite баз данных
 */
using DatabasesMap            = std::map< std::string/* db name */, htl::db::SharedSqliteDb >;
using db_map_keys_t           = std::vector<htl::db::DatabasesMap::key_type>;
using db_map_values_t         = std::vector<htl::db::DatabasesMap::value_type>;
using db_map_mapped_t         = std::vector<htl::db::DatabasesMap::mapped_type>;

//------------------------------------------------------------------------------

}// namespace                     db
}// namespace                     htl

#endif // SQLITEDATABASE_H
