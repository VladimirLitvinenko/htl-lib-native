#ifndef MBTILESWATCHER_H
#define MBTILESWATCHER_H

#include "InotifyManager.h"
#include "MBTileDatabase.h"

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
//TODO: Зарезервировано к разработке

/**
 * @brief Класс MBTilesWatcher реализует механизм слежения за изменениями набора
 * тайловых баз данных.
 *
 * Базируется на @a htl::watch::InotifyManager
 */
class COMMON_SHARED_EXPORT        MBTilesWatcher
: public                          htl::watch::InotifyManager
{
public:
  using base_class_t            = htl::watch::InotifyManager;

  /**/                            MBTilesWatcher  () = delete;
  explicit                        MBTilesWatcher  ( const std::string & path );
  virtual                       ~ MBTilesWatcher  ();

//  bool                            IsValid         () const override;

  void                            SetPath         ( const std::string & path ) noexcept;
  file_path_t                     GetDir          () const;
  file_pathes_t                   GetFiles        () const;

protected:
  mutable std::recursive_mutex    m_Mutex;

private:
  boost::filesystem::path         m_dir;
  htl::db::DatabasesMap           m_tiles;
  void                            PrepareFiles    ();
};

//------------------------------------------------------------------------------

}// namespace                     watch
}// namespace                     htl

#endif // MBTILESWATCHER_H
