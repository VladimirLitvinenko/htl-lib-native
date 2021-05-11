#pragma once

#include <zlib.h>
#include <string>

namespace                         htl   {
namespace                         tiles {

void zlib_compress(const char * data,
                   std::size_t size,
                   std::string & output,
                   bool gzip=true,
                   int level=Z_DEFAULT_COMPRESSION,
                   int strategy=Z_DEFAULT_STRATEGY);
inline
void zlib_compress( std::string const & input,
                    std::string       & output,
                    bool                gzip      = true,
                    int                 level     = Z_DEFAULT_COMPRESSION,
                    int                 strategy  = Z_DEFAULT_STRATEGY )
{
  zlib_compress(input.data(),input.size(),output,gzip,level,strategy);
}

void zlib_decompress(const char * data,
                     std::size_t size,
                     std::string & output);

inline
void zlib_decompress(std::string const& input, std::string & output)
{
  zlib_decompress(input.data(),input.size(),output);
}


}// namespace                     tiles
}// namespace                     htl

//#if !defined(MAPNIK_VECTOR_TILE_LIBRARY)
//#include "vector_tile_compression.ipp"
//#endif
