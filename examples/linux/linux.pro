TEMPLATE = subdirs

SUBDIRS += \
#if use mapbox :
#  mapbox \
  asio \
#  mapbox \
#  db \
#if use mapbox :
#  qt \
  watchers \
  applications \
#  sqlite3 \
  nmea \
#if use mapbox :
#  mbtiles \
  servers \
