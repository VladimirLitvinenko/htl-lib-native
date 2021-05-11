#include <MBTileServer.h>
#include <HTTPLiterals.h>
#include <Logger.h>

namespace                         htl   {
namespace                         tiles {
static std::string Str500 =
    "HTTP/1.1 500 InternalServerError\r\n"
    "\r\n"
    ;
static std::string Str200 =
    "HTTP/1.1 200 OK\r\n"
    "Vary: Origin\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Type: application/x-protobuf\r\n"
    "Content-Encoding: gzip\r\n"
    "Cache-Control: public\r\n"
    "\r\n"
    ;

MBTileServer::MBTileServer( uint16_t port, int64_t timeoutMs )
  : MBTileServer::base_class_t( port, timeoutMs )
{
}

bool
MBTileServer::IsValid()
const noexcept
{
  return m_status == HttpStatus::Unused;
}

void MBTileServer::SetStatus(const HttpStatus status)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_status = status;
//  DUMP_INFO( std::string("New MBTilesServger status: ") + HTTP_STATUS_MAP.at( static_cast<std::uint16_t>(status)) );
  END_LOCK_SECTION_;
}

HttpStatus
MBTileServer::GetStatus() const
{
  return m_status;
}

bool
MBTileServer::StatusOk()
const
{
  return GetStatus() == HttpStatus::OK;
}

std::string
MBTileServer::GetStatusStr()
const
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( HTTP_STATUS_MAP.find( static_cast<std::uint16_t>(m_status) ));
  return
      HTTP_KEY_VERSION + "1.1 " +

      ( iter == HTTP_STATUS_MAP.end()
      ? std::to_string(static_cast<std::uint16_t>(HttpStatus::InternalServerError)) +
        " " +
        HTTP_STATUS_MAP.at( static_cast<std::uint16_t>(HttpStatus::InternalServerError) )
      : std::to_string( iter->first ) +
        " ") +
        iter->second;
  END_LOCK_SECTION_;
}

bool
MBTileServer::Start( bool asThread )
noexcept
{
  try
  {
    SetTransformation( boost::bind( &MBTileServer::Tramsformation, this, boost::placeholders::_1 ) );
    SetStatus( MBTileServer::base_class_t::Start( asThread )
               ? HttpStatus::OK
               : HttpStatus::InternalServerError );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    SetStatus( HttpStatus::InternalServerError );
  }
  return StatusOk();
}

vectors_data_t
MBTileServer::Tramsformation( const vectors_data_t & in )
{
  BEGIN_LOCK_SECTION_SELF_;

  vectors_data_t              retval;
  htl::tiles::MBTileInfo      ti;
  vectors_data_t::value_type  h;
  HTTPMBTileHeader            th;

  SetStatus( in.empty() ? HttpStatus::InternalServerError : HttpStatus::OK );
  if( StatusOk() )
  {
    th.SetRequest( in.at(0).data() );
    ti = th.GetTileInfo();
  }

  auto & d = ti.GetDataRef();
  SetStatus( d.empty() ? HttpStatus::InternalServerError : HttpStatus::OK );
  StatusOk()
      ? h.assign( Str200.begin(), Str200.end() )
      : h.assign( Str500.begin(), Str500.end() );

  retval.push_back( h );
  retval.push_back( ti.GetData() );

  return retval;
  END_LOCK_SECTION_;
}

}// namespace                     tiles
}// namespace                     htl
