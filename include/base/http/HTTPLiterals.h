#ifndef HTTPLITERALS_H
#define HTTPLITERALS_H

#include <string>
#include <unordered_map>
#include <map>

const std::string HTTP_KEY_RET              = "\r\n";
const std::string HTTP_DELIMITER            = ": ";

const std::string HTTP_KEY_GET              = "GET";
const std::string HTTP_KEY_VERSION          = "HTTP/";
const std::string HTTP_KEY_HOST             = "Host";
const std::string HTTP_KEY_USER_AGENT       = "User-Agent";
const std::string HTTP_KEY_CONNECTION       = "Connection";
const std::string HTTP_VAL_CONNECTION_TRUE  = "Keep-Alive";
const std::string HTTP_KEY_A_ENCODING       = "Accept-Encoding";
const std::string HTTP_VAL_A_ENCODING_GZ    = "gzip";
const std::string HTTP_VAL_A_ENCODING_DF    = "deflate";
const std::string HTTP_KEY_A_LANGUAGE       = "Accept-Language";

const std::string HTTP_KEY_ACC_CTRL         = "Access-Control-Allow-Origin";
const std::string HTTP_VAL_ACC_CTRL         = "*";

const std::string HTTP_KEY_CNT_TYPE         = "Content-Type";
const std::string HTTP_KEY_CNT_ENCODING     = "Content-Encoding";

const std::string HTTP_KEY_CACHE_CTRL       = "Cache-Control";
const std::string HTTP_VAL_CACHE_CTRL       = "public";

enum class                HttpStatus : std::uint16_t
{
  Continue           = 100, // RFC 7231, 6.2.1
  SwitchingProtocols = 101, // RFC 7231, 6.2.2
  Processing         = 102, // RFC 2518, 10.1
  EarlyHints         = 103, // RFC 8297

  OK                   = 200, // RFC 7231, 6.3.1
  Created              = 201, // RFC 7231, 6.3.2
  Accepted             = 202, // RFC 7231, 6.3.3
  NonAuthoritativeInfo = 203, // RFC 7231, 6.3.4
  NoContent            = 204, // RFC 7231, 6.3.5
  ResetContent         = 205, // RFC 7231, 6.3.6
  PartialContent       = 206, // RFC 7233, 4.1
  MultiStatus          = 207, // RFC 4918, 11.1
  AlreadyReported      = 208, // RFC 5842, 7.1
  IMUsed               = 226, // RFC 3229, 10.4.1

  MultipleChoices   = 300, // RFC 7231, 6.4.1
  MovedPermanently  = 301, // RFC 7231, 6.4.2
  Found             = 302, // RFC 7231, 6.4.3
  SeeOther          = 303, // RFC 7231, 6.4.4
  NotModified       = 304, // RFC 7232, 4.1
  UseProxy          = 305, // RFC 7231, 6.4.5
  Unused            = 306, // RFC 7231, 6.4.6 (Unused)
  TemporaryRedirect = 307, // RFC 7231, 6.4.7
  PermanentRedirect = 308, // RFC 7538, 3

  BadRequest                   = 400, // RFC 7231, 6.5.1
  Unauthorized                 = 401, // RFC 7235, 3.1
  PaymentRequired              = 402, // RFC 7231, 6.5.2
  Forbidden                    = 403, // RFC 7231, 6.5.3
  NotFound                     = 404, // RFC 7231, 6.5.4
  MethodNotAllowed             = 405, // RFC 7231, 6.5.5
  NotAcceptable                = 406, // RFC 7231, 6.5.6
  ProxyAuthRequired            = 407, // RFC 7235, 3.2
  RequestTimeout               = 408, // RFC 7231, 6.5.7
  Conflict                     = 409, // RFC 7231, 6.5.8
  Gone                         = 410, // RFC 7231, 6.5.9
  LengthRequired               = 411, // RFC 7231, 6.5.10
  PreconditionFailed           = 412, // RFC 7232, 4.2
  RequestEntityTooLarge        = 413, // RFC 7231, 6.5.11
  RequestURITooLong            = 414, // RFC 7231, 6.5.12
  UnsupportedMediaType         = 415, // RFC 7231, 6.5.13
  RequestedRangeNotSatisfiable = 416, // RFC 7233, 4.4
  ExpectationFailed            = 417, // RFC 7231, 6.5.14
  Teapot                       = 418, // RFC 7168, 2.3.3
  MisdirectedRequest           = 421, // RFC 7540, 9.1.2
  UnprocessableEntity          = 422, // RFC 4918, 11.2
  Locked                       = 423, // RFC 4918, 11.3
  FailedDependency             = 424, // RFC 4918, 11.4
  TooEarly                     = 425, // RFC 8470, 5.2.
  UpgradeRequired              = 426, // RFC 7231, 6.5.15
  PreconditionRequired         = 428, // RFC 6585, 3
  TooManyRequests              = 429, // RFC 6585, 4
  RequestHeaderFieldsTooLarge  = 431, // RFC 6585, 5
  UnavailableForLegalReasons   = 451, // RFC 7725, 3

  InternalServerError           = 500, // RFC 7231, 6.6.1
  NotImplemented                = 501, // RFC 7231, 6.6.2
  BadGateway                    = 502, // RFC 7231, 6.6.3
  ServiceUnavailable            = 503, // RFC 7231, 6.6.4
  GatewayTimeout                = 504, // RFC 7231, 6.6.5
  HTTPVersionNotSupported       = 505, // RFC 7231, 6.6.6
  VariantAlsoNegotiates         = 506, // RFC 2295, 8.1
  InsufficientStorage           = 507, // RFC 4918, 11.5
  LoopDetected                  = 508, // RFC 5842, 7.2
  NotExtended                   = 510, // RFC 2774, 7
  NetworkAuthenticationRequired = 511, // RFC 6585, 6
};

const std::unordered_map< std::uint16_t, std::string> HTTP_STATUS_MAP
{
  { static_cast<std::uint16_t>(HttpStatus::Continue)            , "Continue" },
  { static_cast<std::uint16_t>(HttpStatus::SwitchingProtocols)  , "Switching Protocols" },
  { static_cast<std::uint16_t>(HttpStatus::Processing)          , "Processing" },
  { static_cast<std::uint16_t>(HttpStatus::EarlyHints)          , "Early Hints" },

  { static_cast<std::uint16_t>(HttpStatus::OK)                  , "OK" },
  { static_cast<std::uint16_t>(HttpStatus::Created)             , "Created" },
  { static_cast<std::uint16_t>(HttpStatus::Accepted)            , "Accepted" },
  { static_cast<std::uint16_t>(HttpStatus::NonAuthoritativeInfo), "Non-Authoritative Information" },
  { static_cast<std::uint16_t>(HttpStatus::NoContent)           , "No Content" },
  { static_cast<std::uint16_t>(HttpStatus::ResetContent)        , "Reset Content" },
  { static_cast<std::uint16_t>(HttpStatus::PartialContent)      , "Partial Content" },
  { static_cast<std::uint16_t>(HttpStatus::MultiStatus)         , "Multi-Status" },
  { static_cast<std::uint16_t>(HttpStatus::AlreadyReported)     , "Already Reported" },
  { static_cast<std::uint16_t>(HttpStatus::IMUsed)              , "IM Used" },

  { static_cast<std::uint16_t>(HttpStatus::MultipleChoices)     , "Multiple Choices" },
  { static_cast<std::uint16_t>(HttpStatus::MovedPermanently)    , "Moved Permanently" },
  { static_cast<std::uint16_t>(HttpStatus::Found)               , "Found" },
  { static_cast<std::uint16_t>(HttpStatus::SeeOther)            , "See Other" },
  { static_cast<std::uint16_t>(HttpStatus::NotModified)         , "Not Modified" },
  { static_cast<std::uint16_t>(HttpStatus::UseProxy)            , "Use Proxy" },
  { static_cast<std::uint16_t>(HttpStatus::Unused)              , "-" },
  { static_cast<std::uint16_t>(HttpStatus::TemporaryRedirect)   , "Temporary Redirect" },
  { static_cast<std::uint16_t>(HttpStatus::PermanentRedirect)   , "Permanent Redirect" },

  { static_cast<std::uint16_t>(HttpStatus::BadRequest)          , "Bad Request" },
  { static_cast<std::uint16_t>(HttpStatus::Unauthorized)        , "Unauthorized" },
  { static_cast<std::uint16_t>(HttpStatus::PaymentRequired)     , "Payment Required" },
  { static_cast<std::uint16_t>(HttpStatus::Forbidden)           , "Forbidden" },
  { static_cast<std::uint16_t>(HttpStatus::NotFound)            , "Not Found" },
  { static_cast<std::uint16_t>(HttpStatus::MethodNotAllowed)    , "Method Not Allowed" },
  { static_cast<std::uint16_t>(HttpStatus::NotAcceptable)       , "Not Acceptable" },
  { static_cast<std::uint16_t>(HttpStatus::ProxyAuthRequired)   , "Proxy Authentication Required" },
  { static_cast<std::uint16_t>(HttpStatus::RequestTimeout)      , "Request Timeout" },
  { static_cast<std::uint16_t>(HttpStatus::Conflict)            , "Conflict" },
  { static_cast<std::uint16_t>(HttpStatus::Gone)                , "Gone" },
  { static_cast<std::uint16_t>(HttpStatus::LengthRequired)      , "Length Required" },
  { static_cast<std::uint16_t>(HttpStatus::PreconditionFailed)  , "Precondition Failed" },
  { static_cast<std::uint16_t>(HttpStatus::RequestEntityTooLarge), "Request Entity Too Large" },
  { static_cast<std::uint16_t>(HttpStatus::RequestURITooLong)   , "Request URI Too Long" },
  { static_cast<std::uint16_t>(HttpStatus::UnsupportedMediaType), "Unsupported Media Type" },
  { static_cast<std::uint16_t>(HttpStatus::RequestedRangeNotSatisfiable), "Requested Range Not Satisfiable" },
  { static_cast<std::uint16_t>(HttpStatus::ExpectationFailed)   , "Expectation Failed" },
  { static_cast<std::uint16_t>(HttpStatus::Teapot)              , "I'm a teapot" },
  { static_cast<std::uint16_t>(HttpStatus::MisdirectedRequest)  , "Misdirected Request" },
  { static_cast<std::uint16_t>(HttpStatus::UnprocessableEntity) , "Unprocessable Entity" },
  { static_cast<std::uint16_t>(HttpStatus::Locked)              , "Locked" },
  { static_cast<std::uint16_t>(HttpStatus::FailedDependency)    , "Failed Dependency" },
  { static_cast<std::uint16_t>(HttpStatus::TooEarly)            , "Too Early" },
  { static_cast<std::uint16_t>(HttpStatus::UpgradeRequired)     , "Upgrade Required" },
  { static_cast<std::uint16_t>(HttpStatus::PreconditionRequired), "Precondition Required" },
  { static_cast<std::uint16_t>(HttpStatus::TooManyRequests)     , "Too Many Requests" },
  { static_cast<std::uint16_t>(HttpStatus::RequestHeaderFieldsTooLarge), "Request Header Fields Too Large" },
  { static_cast<std::uint16_t>(HttpStatus::UnavailableForLegalReasons), "Unavailable For Legal Reasons" },

  { static_cast<std::uint16_t>(HttpStatus::InternalServerError) , "Internal Server Error" },
  { static_cast<std::uint16_t>(HttpStatus::NotImplemented)      , "Not Implemented" },
  { static_cast<std::uint16_t>(HttpStatus::BadGateway)          , "Bad Gateway" },
  { static_cast<std::uint16_t>(HttpStatus::ServiceUnavailable)  , "Service Unavailable" },
  { static_cast<std::uint16_t>(HttpStatus::GatewayTimeout)      , "Gateway Timeout" },
  { static_cast<std::uint16_t>(HttpStatus::HTTPVersionNotSupported), "HTTP Version Not Supported" },
  { static_cast<std::uint16_t>(HttpStatus::VariantAlsoNegotiates), "Variant Also Negotiates" },
  { static_cast<std::uint16_t>(HttpStatus::InsufficientStorage) , "Insufficient Storage" },
  { static_cast<std::uint16_t>(HttpStatus::LoopDetected)        , "Loop Detected" },
  { static_cast<std::uint16_t>(HttpStatus::NotExtended)         , "Not Extended" },
  { static_cast<std::uint16_t>(HttpStatus::NetworkAuthenticationRequired), "Network Authentication Required" },
};

//------------------------------------------------------------------------------
using http_header_t             = std::map< std::uint16_t, std::pair< std::string, std::string > >;
using http_pair_t               = http_header_t::mapped_type;

#endif // HTTPLITERALS_H
