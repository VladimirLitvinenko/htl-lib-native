#ifndef MBTILELITERALS_H
#define MBTILELITERALS_H

#include "HTTPLiterals.h"
#include <boost/filesystem.hpp>

//------------------------------------------------------------------------------

const std::string MBTile_Separator  = "/";
const std::string MBTile_Dot        = ".";
const std::string MBTile_Global     = MBTile_Separator + "services";

const std::size_t MBTile_Global_Idx       = 0;
const std::size_t MBTile_DbName_Idx       = 1;
const std::size_t MBTile_Z_Idx            = 3;
const std::size_t MBTile_X_Idx            = 4;
const std::size_t MBTile_Y_Idx            = 5;
const std::size_t MBTile_Max_Idx          = 6;

const std::string MBTile_Detail_Tiles = MBTile_Global + MBTile_Separator + "tiles";
const std::string MBTile_Detail_Map   = MBTile_Global + MBTile_Separator + "map";

const std::string MBTile_Z  = "z";
const std::string MBTile_X  = "x";
const std::string MBTile_Y  = "y";
const std::string MBTile_Proto_Suffix = ".pbf";
const std::string MBTile_Png_Suffix   = ".png";

const std::string MBTile_Format_Png     = "png";
const std::string MBTile_Format_Jpg     = "jpeg";
const std::string MBTile_Format_Pbf     = "pbf";
const std::string MBTile_Format_Mvt     = "mvt";
const std::string MBTile_Format_Deflate = "deflate";
const std::string MBTile_Format_Webp    = "webp";

const std::string MBTile_Mime_Png   = "image/png";
const std::string MBTile_Mime_Jpg   = "image/jpeg";
const std::string MBTile_Mime_Pbf   = "application/x-protobuf";
const std::string MBTile_Mime_Webp  = "image/webp";

const std::string MBTileView                = "tiles";
const std::string MBMetaData                = "metadata";
const std::string MBTile_Validator_Message  = "Отсутствует таблица: '%1'";
const std::string MBTile_Validator_Data     = "SELECT tile_data FROM "+MBTileView+" LIMIT 1";
const std::string MBTile_Validator_Data_Message  = "Данные о тайлах отсутствуют в таблице "+MBTileView+"";
const std::string MBTile_Validator_Zoom_Message  = "Значения минимальных [и | или] максимальных масштабов таблиц "+MBTileView+" и "+MBMetaData+" не совпадают";
const std::string MBTile_Select             = "SELECT tile_data FROM "+MBTileView+" WHERE zoom_level = ? AND tile_column = ? AND tile_row = ? ";
const std::string MBTile_GeoJSON            = "SELECT value AS GeoJSON FROM "+MBMetaData+" WHERE name = \'json\'";
const std::string MBTile_MetaMinMaxZoom     =
  "SELECT "
  "	(Select CAST( value AS INTEGER ) FROM metadata WHERE name = 'minzoom') AS MinZoom, "
  "	(Select CAST( value AS INTEGER ) FROM metadata WHERE name = 'maxzoom') AS MaxZoom";

const std::string MBTile_MinColumn     =
  "SELECT min(t.tile_column) FROM tiles t";
const std::string MBTile_MaxColumn     =
  "SELECT max(t.tile_column) FROM tiles t";

const std::string MBTile_MinRow     =
  "SELECT min(t.tile_row) FROM tiles t";
const std::string MBTile_MaxRow     =
  "SELECT max(t.tile_row) FROM tiles t";

const std::string MBTile_ZoomValidator     =
"SELECT count(*) "
" FROM ( "
" SELECT "
"     min(CAST( zoom_level AS INTEGER )) as MinZoom, "
"     max(CAST( zoom_level AS INTEGER )) as MaxZoom"
"  FROM "
"   ( SELECT zoom_level FROM "+MBTileView+" t GROUP BY t.zoom_level )	"
" UNION   "
" SELECT "
" 	(Select CAST( value AS INTEGER ) FROM "+MBMetaData+" WHERE name = 'minzoom') AS MinZoom, "
" 	(Select CAST( value AS INTEGER ) FROM "+MBMetaData+" WHERE name = 'maxzoom') AS MaxZoom"
" ) ";

const std::string MBTile_MinMaxZoom         =
  "SELECT "
  "   min(CAST( zoom_level AS INTEGER )) as MinZoom, "
  "   max(CAST( zoom_level AS INTEGER )) as MaxZoom"
  "FROM "
  " ( SELECT zoom_level FROM "+MBTileView+" t GROUP BY t.zoom_level )";

const std::string TilesZoomCount_Select   =
  "SELECT "
  "   t.zoom_level, "
  "   count(*) AS tiles_count "
  "FROM     "+MBTileView+" t "
  "GROUP BY t.zoom_level";

const std::string TilesInfo_Select   =
  "SELECT "
"     info.*, "
"     t2.tile_data "
"  FROM ( "
"         SELECT "
"           t1.zoom_level, "
"           count(*) as tiles_count "
"         FROM      ( select t.zoom_level, t.tile_column, t.tile_row FROM "+MBTileView+" t ) AS t1 "
"         GROUP BY  t1.zoom_level "
"       ) AS info "
"  LEFT OUTER JOIN "+MBTileView+" t2 "
"  WHERE info.zoom_level = t2.zoom_level "
"  ORDER BY info.zoom_level ASC";


const std::vector<char> BLANK_PNG
({
    char(0x89), char(0x50), char(0x4e), char(0x47), char(0xd), char(0xa), char(0x1a), char(0xa), char(0x0), char(0x0), char(0x0), char(0xd), char(0x49),
    char(0x48), char(0x44), char(0x52), char(0x0), char(0x0), char(0x1), char(0x0), char(0x0), char(0x0), char(0x1), char(0x0), char(0x1), char(0x3),
    char(0x0), char(0x0), char(0x0), char(0x66), char(0xbc), char(0x3a), char(0x25), char(0x0), char(0x0), char(0x0), char(0x3), char(0x50), char(0x4c),
    char(0x54), char(0x45), char(0x0), char(0x0), char(0x0), char(0xa7), char(0x7a), char(0x3d), char(0xda), char(0x0), char(0x0), char(0x0), char(0x1),
    char(0x74), char(0x52), char(0x4e), char(0x53), char(0x0), char(0x40), char(0xe6), char(0xd8), char(0x66), char(0x0), char(0x0), char(0x0),
    char(0x1f), char(0x49), char(0x44), char(0x41), char(0x54), char(0x68), char(0xde), char(0xed), char(0xc1), char(0x1), char(0xd), char(0x0),
    char(0x0), char(0x0), char(0xc2), char(0x20), char(0xfb), char(0xa7), char(0x36), char(0xc7), char(0x37), char(0x60), char(0x0), char(0x0),
    char(0x0), char(0x0), char(0x0), char(0x0), char(0x0), char(0x0), char(0x71), char(0x7), char(0x21), char(0x0), char(0x0), char(0x1), char(0xa7),
    char(0x57), char(0x29), char(0xd7), char(0x0), char(0x0), char(0x0), char(0x0), char(0x49), char(0x45), char(0x4e), char(0x44), char(0xae),
    char(0x42), char(0x60), char(0x82),
});

const std::string MBTile_Srv_Port_Key  = "port";
const std::string MBTile_Srv_Host_Key  = "host";

const std::string MBTile_JsonFile_Corrupt  = "Неверный файл JSON-конфигурации.";

#endif // MBTILELITERALS_H
