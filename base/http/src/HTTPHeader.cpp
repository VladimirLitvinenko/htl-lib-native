#include "HTTPHeader.h"
#include <DocumentLiterals.h>
#include <Utils.h>

namespace                         htl   {
namespace                         http {

HTTPHeader::HTTPHeader()
  : HTTPHeader::base_class_t     ( htl::docs::DOC_HTTP_HEADER_ID )
{

}

HTTPHeader::HTTPHeader( const std::string& req )
  : HTTPHeader::HTTPHeader()
{
  Parce( req );
}

bool
HTTPHeader::IsValid()
const noexcept
{
  return
      HTTPHeader::base_class_t::IsValid()
      and
      IsCodeValid()
      and
      ( not m_content.empty() );
}

bool
HTTPHeader::IsCodeValid()
const
{
  return m_status == HttpStatus::OK;
}

bool
HTTPHeader::IsKeepAlive()
{
  return Get( HTTP_KEY_CONNECTION ) == HTTP_VAL_CONNECTION_TRUE;
}

void HTTPHeader::SetRequest(const std::string& request)
{
  Parce( request );
}

std::string
HTTPHeader::Get( const std::string & key )
{
//  BEGIN_LOCK_SECTION_SELF_;
  auto iter( std::find_if( ContentRef().begin(), ContentRef().end(),
                           [ & ]( const http_header_t::value_type & p )
  {
    return p.second.first == key;
  }) );
  return
      iter != ContentRef().end()
              ? iter->second.first + HTTP_DELIMITER + iter->second.second
              : "";
//  END_LOCK_SECTION_;
}

void
HTTPHeader::Set( const HttpStatus status )
{
//  BEGIN_LOCK_SECTION_SELF_;
  m_status = status;
  std::uint16_t key( static_cast<std::uint16_t>(m_status) );
  ContentRef().emplace( key , std::make_pair( std::to_string(key), HTTP_STATUS_MAP.at(key) ) );
//  END_LOCK_SECTION_;
}

void
HTTPHeader::Set( const std::string& key, const std::string& value)
{
//  BEGIN_LOCK_SECTION_SELF_;
  ContentRef().emplace( std::atoi(key.c_str()), std::make_pair( key, value ) );
//  END_LOCK_SECTION_;
}

std::string
HTTPHeader::GetHeader()
{
  std::string retval;
  for( auto & cnt : std::move( Content() ) )
  {
    retval += cnt.second.first + HTTP_DELIMITER + cnt.second.second + HTTP_KEY_RET;
  }
  if( not retval.empty() )
    retval += HTTP_KEY_RET;

  return retval;
}

void
HTTPHeader::ClearContent()
{
  BEGIN_LOCK_SECTION_SELF_;
  m_content.clear();
  END_LOCK_SECTION_;
}

std::string
HTTPHeader::GetDestinition()
const
{
  return m_destinition;
}

std::string
HTTPHeader::Dump()
{
  std::string retval;
  for( auto iter( ContentRef().begin() ); iter != ContentRef().end(); ++iter )
    retval += iter->second.first + HTTP_DELIMITER + " " + iter->second.second + HTTP_KEY_RET;
  if( not retval.empty())
    retval += "DESTINITION: " + m_destinition  + HTTP_KEY_RET;
  return retval;
}

void
HTTPHeader::Parce(const std::string& request)
{
//  BEGIN_LOCK_SECTION_SELF_;
  ClearContent();
  if( not request.empty() )
  {
    auto strings = htl::utils::StrSplit( request, HTTP_KEY_RET );
    if( strings.size() < 2 )
      return;

    if( strings.front().find( HTTP_KEY_GET ) == std::string::npos )
      return;

    auto get_valuses = htl::utils::StrSplit( strings.front(), " " );
    if( get_valuses.size() < 2 )
      return;

    m_destinition = get_valuses.at(1);

    for( auto item : strings )
    {
      for( auto key : {
           HTTP_KEY_HOST,
           HTTP_KEY_USER_AGENT,
           HTTP_KEY_CONNECTION,
           HTTP_KEY_A_ENCODING,
           HTTP_KEY_A_LANGUAGE,
    })
      if( not item.empty() )
      {
        auto pos( item.find( key ) );
        if( pos != std::string::npos )
        {
          std::string res;
          res.assign(item.begin() + pos + key.size() + HTTP_DELIMITER.size(), item.end());
          Set( key, res );
        }
      }
    }
  }
//  END_LOCK_SECTION_;
}

}// namespace                     watch
}// namespace                     htl
