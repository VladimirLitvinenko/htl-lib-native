#include "DbFactory.h"
#include "Utils.h"

#include "Utils.h"

namespace                         htl   {
namespace                         db    {

//------------------------------------------------------------------------------
std::map< DBOrderType, std::string > order_literal
{
  { DBOrderType::None , std::string() },
  { DBOrderType::Asc  , std::string("ASC") },
  { DBOrderType::Desc , std::string("DESC") },
};

//------------------------------------------------------------------------------
DbFactory::DbFactory(const std::string& config_path)
  : DbFactory::base_class_t(config_path)
{

}

bool
DbFactory::IsValid()
const noexcept
{
  return IsValidConfig() and IsValidDatabases();
}

//------------------------------------------------------------------------------

bool
DBTableValidation
(
    SqliteDatabase    & dbRef,
    const std::string & tableName,
    const std::string & err_msg
)
noexcept
{
  bool retval{ not tableName.empty() };
  if( retval and dbRef.IsValid() )
  {
    try
    {
      (* dbRef.DbRef() )
          << htl::utils::StrReplace( htl::db::Table_Validator_Select,"%1", tableName )
          >> [&]( std::unique_ptr<int> tbl_count)
          {
            dbRef.SetState(
                  tbl_count.operator bool()
                  ? *tbl_count > 0
                    ? htl::db::DBState::Ok
                    : htl::db::DBState::MissingRequiredTable
                  : htl::db::DBState::ArgumentError,
              err_msg );
          };
    }
    catch( const std::exception & e )
    {
      retval = false;
      dbRef.SetState( htl::db::DBState::OutOfRangeError, err_msg );
      DUMP_EXCEPTION( e );
    }
  }
  return retval and dbRef.IsValid();
}

std::string
FieldsFromList ( const htl::strings_t  & fields )
{
  std::string retval;
  if( fields.empty() )
    retval = "*";
  else
  {
    for( auto & field : fields )
    {
      retval += field + ",";
    }
    retval = htl::utils::StrChop( retval, 1 );
  }
  return retval;
}

std::experimental::optional< std::string > PrepareDBSelect
(
    const std::string   & tableName,
    const strings_t     & fields,
    const std::string   & whereExpression,
    std::uint64_t         limit,
    const strings_t     & orderFields,
    DBOrderType         order
)
{
  try
  {
    if( not fields.empty() )
    {
      auto select_field = FieldsFromList( fields );
      auto order_fields = FieldsFromList( orderFields );
      std::string select_expr("SELECT ");
      select_expr += select_field;
      select_expr += " FROM ";
      select_expr += tableName;
      select_expr += ( whereExpression.empty() ? "" : ( std::string(" WHERE ") + whereExpression ) );
      select_expr += ( limit > 0 ? ( std::string(" LIMIT ") + std::to_string(limit) ) : "");
      bool is_order{order_fields != "*"};
      select_expr += ( is_order ? ( std::string(" ORDER BY ") + order_fields ) : "" );
      select_expr += ( is_order ? ( std::string(" ") + order_literal.at(order) ) : "" );
      return select_expr;
    }
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return nullptr;
}

}// namespace                     db
}// namespace                     htl

