#ifndef MBTILEINFO_H
#define MBTILEINFO_H

#include "common/Types.h"
#include "mbtiles/vector_tile_config.h"

#include <stdint.h>
#include <vector>
#include <functional>
#include <type_traits>

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        MBTileDatabase;

/**
 * @brief Класс MBTileInfo реализует работу с информацией по тайлу Mapbox
 */
class COMMON_SHARED_EXPORT        MBTileInfo
{
  friend class                    MBTileDatabase;
public:
  /**/                            MBTileInfo  () {}
  /**/                            MBTileInfo  ( const MBTileInfo & ti );
  /**/                            MBTileInfo  ( MBTileInfo && ti );
  virtual                       ~ MBTileInfo  () {}

  virtual MBTileInfo &            operator=   ( const MBTileInfo & ti ) noexcept;
  virtual MBTileInfo &            operator=   ( MBTileInfo && ti ) noexcept;

  bool                            IsEmpty() const;

  vectors_data_t::value_type      GetData     () const;
  std::string                     DataToString() const;
  vectors_data_t::value_type    & GetDataRef  ();
  virtual void                    SetData     ( const vectors_data_t::value_type & data );
  virtual void                    SetData     ( vectors_data_t::value_type && data );
  void                            ClearData     ();
  void                            ResetAll      ();
  uint32_t                        GetZ          () const  { return m_z; }
  uint64_t                        GetX          () const  { return m_x; }
  uint64_t                        GetY          () const  { return m_y; }

  void                            SetCoordinates( uint32_t z, uint64_t x, uint64_t y, bool transform_y = true );

  TileFormat                      GetFormat     () const;
  std::string                     GetFormatStr  () const;
  std::string                     GetFormatMime () const;

  std::pair<bool, std::string>    ToPath        ( const std::string & output_path, bool decompress = false );
  bool                            IsGzip        () const;
  bool                            IsZlib        () const;

protected:
  mutable std::recursive_mutex    m_Mutex;

  virtual void                    DetectFormat  () noexcept;

private:
  uint32_t                        m_z       {0};
  uint64_t                        m_x       {0};
  uint64_t                        m_y       {0};
  vectors_data_t::value_type      m_data;
  TileFormat                      m_format  { TileFormat::UNKNOWN };
};

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILEINFO_H
