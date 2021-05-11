#pragma once

#include "sqlite_modern_cpp.h"
#include "Types.h"
#include <map>
#include <memory>

namespace                         htl {
namespace                         db  {

//------------------------------------------------------------------------------
using sqlitedb_ptr              = std::unique_ptr< sqlite::database >;

//------------------------------------------------------------------------------
enum class                        DBState
{
  NoDatabase                    =   -1,
  Ok                            =    0,

  ArgumentError                 =   10,
  OutOfRangeError               =   11,
  MissingRequiredTable          =  100,
  MissingTiles                  =  110,
  DbNotFound                    =  111,
  TileNotFound                  =  112,
  SelectError                   =  113,
};

const std::map< DBState, std::string > db_state_messages
{
  { DBState::NoDatabase           , "База данных отсутствует." },
  { DBState::Ok                   , "Успех" },
  { DBState::ArgumentError        , "Ошибка в параметрах метода вызова запроса" },
  { DBState::OutOfRangeError      , "НГарушение границы диапазона значения" },
  { DBState::MissingRequiredTable , "Отсутствуют необходимые таблицы 'tiles' и/или 'metadata'" },
  { DBState::MissingTiles         , "Отсутствует набор тайлов" },
  { DBState::TileNotFound         , "Запрашиваемый тайл отсутствует" },
  { DBState::SelectError          , "Некорректный запрос к БД" },
};

const std::string Table_Validator_Select   = "SELECT count(*) FROM sqlite_master WHERE name IN ('%1')";

//------------------------------------------------------------------------------

}// namespace                     db
}// namespace                     htl
