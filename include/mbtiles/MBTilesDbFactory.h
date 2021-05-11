#ifndef MBTILESDBFACTORY_H
#define MBTILESDBFACTORY_H

#include "MBTileDatabase.h"
#include "DbFactory.h"

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
/**
 * @brief Класс MBTilesDbFactory реализует фабрику работы с тайлами как файлами
 */
class COMMON_SHARED_EXPORT        MBTilesDbFactory
: public                          htl::db::DbFactory
{
public:
  using base_class_t            = htl::db::DbFactory;

  /**/                            MBTilesDbFactory();
  explicit                        MBTilesDbFactory( const std::string & config_path );

  /**
   * @brief Метод Instance возвращает ссылку на статический экземпляр класса
   * @param config_path путь с файлом конфигурации баз тайлов
   * @return ссылка на статический экземпляр класса
   */
  static MBTilesDbFactory &       Instance        ( const std::string & config_path = htl::MAIN_CONF_DB )
  {
    return MBTilesDbFactory::base_class_t::Instance< htl::tiles::MBTilesDbFactory >(config_path);
  }

  /**
   * @brief Метод PrepareTile готовит информацию о тайле (чтение) из базы данных (файла) согласно параметрам
   * @param db_name файл с тайлами
   * @param ti информационные данные по тайлу
   * @return Ьулево значение:
   * @value true успешное извлечение тайла
   * @value false при извлечении возникли ошибки
   */
  bool                            PrepareTile     ( const std::string & db_name, MBTileInfo & ti );
  bool                            PrepareGeoJSON  ( const std::string & db_name, std::string & geo );
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILESDBFACTORY_H
