#include "SqliteDatabase.h"
#include "DBCommon.h"
#include "Logger.h"
#include <sqlite3.h>

namespace                         htl {
namespace                         db  {

SqliteDatabase::SqliteDatabase( const std::string& db_name )
{
  SetName(db_name);
}

SqliteDatabase::~SqliteDatabase()
{
  Close();
}

bool
SqliteDatabase::IsDbValid()
const
{
  return
      m_db_ptr.operator bool()
      and
      m_db_ptr->connection().operator bool();
}

bool
SqliteDatabase::IsValid()
const
{
  SetState( IsDbValid()
            ? htl::db::DBState::Ok
            : htl::db::DBState::NoDatabase );
  return IsOk();
}

bool
SqliteDatabase::IsOk()
const
{
  return GetState() == htl::db::DBState::Ok;
}

DBState
SqliteDatabase::GetState()
const
{
  return m_state;
}

std::string
SqliteDatabase::StateMessage()
const
{
  auto iter( htl::db::db_state_messages.find( GetState() ) );
  return
      std::string("Состояние БД: ") + std::string(
      iter != htl::db::db_state_messages.end()
              ? iter->second
              :"<Описание состояния отсутствует>");
}

std::string
SqliteDatabase::GetDbName()
const
{
  return m_name;
}

void
SqliteDatabase::Close()
noexcept
{
  if( IsDbValid() )
  {
    sqlite3_close_v2( m_db_ptr->connection().get() );
  }
}

void
SqliteDatabase::SetName(const std::string& db_name)
noexcept
{
  if( IsDbValid() )
    Close();

  m_name = db_name;
  try
  {
    m_db_ptr.reset( new htl::db::sqlitedb_ptr::element_type( GetDbName() ) );
    //    auto y = std::atol( htl::utils::StrSplit( name_parts.back(), MBTile_Dot ).front().c_str() );
  }
  catch( const sqlite::sqlite_exception & e )
  {
    SetState( htl::db::DBState::DbNotFound );
    DUMP_EXCEPTION( e );
  }
}

void
SqliteDatabase::SetState(DBState state, const std::string & err_msg )
const
{
//  if( m_state != state )
  {
    m_state = state;
    if( not IsOk() )
    {
      DUMP_CRITICAL( (err_msg.empty() ? StateMessage() : err_msg ) );
    }
  }
}

sqlitedb_ptr&
SqliteDatabase::DbRef()
noexcept
{
  return std::ref( m_db_ptr );
}

}// namespace                     db
}// namespace                     htl
