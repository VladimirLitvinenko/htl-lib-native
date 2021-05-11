
/**
 * @file
 */
#ifndef DBFACTORY_H
#define DBFACTORY_H

#include "IfaceFactory.h"

#include <tuple>
#include <experimental/optional>

namespace                         htl   {
namespace                         db    {

//------------------------------------------------------------------------------
/**
 * @brief The DbFactory class
 */
class COMMON_SHARED_EXPORT        DbFactory
: public                          htl::db::IfaceFactory
{
public:
  using base_class_t            = htl::db::IfaceFactory;

  /**/                            DbFactory() = delete;
  explicit                        DbFactory( const std::string & config_path );

  bool                            IsValid         () const noexcept;

  template< typename DbFactoryT_ >
  static DbFactoryT_ &            Instance        ( const std::string & config_path = std::string() )
  {
    static_assert ( std::is_base_of<htl::db::DbFactory, typename std::decay<DbFactoryT_>::type >::value,
                    ST_ASSERT_TEMPLATE_VALIDATOR );
    static DbFactoryT_ retval( config_path );
    if( ( retval.GetConfig() != config_path ) and ( not config_path.empty() ) )
    {
      retval.SetPath( config_path );
    }
    return retval;
  }
};

//------------------------------------------------------------------------------
enum class DBOrderType
{
  None,
  Asc,
  Desc,
};

extern std::map< DBOrderType, std::string > order_literal;

bool DBTableValidation
(
    SqliteDatabase    & dbRef,
    const std::string & tableName,
    const std::string & err_msg     = ""
) noexcept;

std::string FieldsFromList ( const htl::strings_t  & fields );

std::experimental::optional< std::string > PrepareDBSelect
(
    const std::string                   & tableName,
    const htl::strings_t                & fields          = { {"*"}, },
    const std::string                   & whereExpression = std::string(),
    std::uint64_t                         limit           = 0,
    const htl::strings_t                & orderFields     = htl::strings_t(),
    DBOrderType                           order           = DBOrderType::None
);

//------------------------------------------------------------------------------

}// namespace                     db
}// namespace                     htl

#endif // DBFACTORY_H
