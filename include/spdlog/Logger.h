#ifndef HTL_LOGGER_H
#define HTL_LOGGER_H

#include "HtlGlobal"
#include <map>
#include <string>
#include <boost/asio.hpp>
#include "spdlog/spdlog.h"

namespace htl   {
namespace log   {

using loggers_map               = std::map< std::string, std::shared_ptr<spdlog::logger> >;

class COMMON_SHARED_EXPORT        Logger
{
public:
  /**/                            Logger();
  /**/                            Logger( const std::string & file_name );
  std::string                     FileName() const;
  std::string                     AbsoluteFileName() const;
//  void                            SetFormapPattern( const std::string & pattern = "[%n] [%*] [%^%l%$] %v" );
  void                            SetFormapPattern( const std::string & pattern = "[%*] [%^%l%$] %v" );

  bool                            IsValid() const;

  static Logger                 & Instance( const std::string & file_name = "" );

  template<typename T>
  static void                     Info( const std::string & logger_key,  const T & msg )
  {
    Logger & i_ref = Logger::Instance();
    if( i_ref.IsValid() and( i_ref.m_loggers.find(logger_key) != i_ref.m_loggers.end() ) )
    {
      i_ref.m_loggers[logger_key]->info( msg );
      i_ref.m_loggers[logger_key]->flush();
    }
  }

  template<typename T>
  static void                     Warning( const std::string & logger_key,  const T & msg )
  {
    Logger & i_ref = Logger::Instance();
    if( i_ref.IsValid() and( i_ref.m_loggers.find(logger_key) != i_ref.m_loggers.end() ) )
    {
      i_ref.m_loggers[logger_key]->warn( msg );
      i_ref.m_loggers[logger_key]->flush();
    }
  }
  template<typename T>
  static void                     Critical( const std::string & logger_key,  const T & msg )
  {
    Logger & i_ref = Logger::Instance();
    if( i_ref.IsValid() and( i_ref.m_loggers.find(logger_key) != i_ref.m_loggers.end() ) )
    {
      i_ref.m_loggers[logger_key]->critical( msg );
      i_ref.m_loggers[logger_key]->flush();
    }
  }
  template<typename T>
  static void                     Debug( const std::string & logger_key,  const T & msg )
  {
    Logger & i_ref = Logger::Instance();
    if( i_ref.IsValid() and( i_ref.m_loggers.find(logger_key) != i_ref.m_loggers.end() ) )
    {
      i_ref.m_loggers[logger_key]->debug( msg );
      i_ref.m_loggers[logger_key]->flush();
    }
  }
  template<typename T>
  static void                     Trace( const std::string & logger_key,  const T & msg )
  {
    Logger & i_ref = Logger::Instance();
    if( i_ref.IsValid() and( i_ref.m_loggers.find(logger_key) != i_ref.m_loggers.end() ) )
    {
      i_ref.m_loggers[logger_key]->trace( msg );
      i_ref.m_loggers[logger_key]->flush();
    }
  }

  void                            dump_throw( const std::string & msg );

protected:
  class                           formatter_flag
  : public spdlog::custom_flag_formatter
  {
  public:
      void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
      {
          std::string some_txt = "custom-flag";
          dest.append(some_txt.data(), some_txt.data() + some_txt.size());
      }

      std::unique_ptr<custom_flag_formatter> clone() const override
      {
          return spdlog::details::make_unique<Logger::formatter_flag>();
      }
  };

private:
  std::string                     m_file_name;
  loggers_map                     m_loggers;

  void                            SetFile( const std::string & file_name ) noexcept;
};

}// namespace log
}// namespace htl

//-----------------------------------------------------------------------------

namespace {

#ifndef DUMP_INFO_CONSOLE
#define DUMP_INFO_CONSOLE(msg) \
  htl::log::Logger::Instance().Info("console", std::string(msg) );
#endif

#ifndef DUMP_WARNING_CONSOLE
#define DUMP_WARNING_CONSOLE(msg) \
  htl::log::Logger::Instance().Warning("console", std::string(msg) );
#endif

#ifndef DUMP_INFO_FILE
#define DUMP_INFO_FILE(msg) \
  htl::log::Logger::Instance().Info("file", std::string(msg) );
#endif

#ifndef DUMP_WARNING_FILE
#define DUMP_WARNING_FILE(msg) \
  htl::log::Logger::Instance().Warning("file", std::string(msg) );
#endif

#ifndef DUMP_INFO
#define DUMP_INFO(msg) \
  DUMP_INFO_CONSOLE(msg ) \
  DUMP_INFO_FILE(msg )
#endif

#ifndef DUMP_WARNING
#define DUMP_WARNING(msg) \
  DUMP_WARNING_CONSOLE(msg ) \
  DUMP_WARNING_FILE(msg )
#endif

#ifndef ERROR_ABSTRACT_OVERLOAD
# define ERROR_ABSTRACT_OVERLOAD \
  assert( false/*, Q_FUNC_INFO, spo::ABSTRACT_OVERLOAD_MESSAGE.toStdString().data()*/ );
#endif

#ifndef DUMP_EXCEPTION
#define DUMP_EXCEPTION(ExceptValue) \
  std::string msg = std::string( __PRETTY_FUNCTION__) + ":" + __FILE__ + ", " + std::to_string(__LINE__) + ": EXCEPTION :" + ExceptValue.what(); \
  htl::log::Logger::Instance().Info("console", std::string(msg) ); \
  htl::log::Logger::Instance().Info("file", std::string(msg) );
#endif

#ifndef SET_EXCEPTION
#define SET_EXCEPTION(Msg) \
  htl::log::Logger::Instance().dump_throw(std::string(Msg) );
#endif

#ifndef DUMP_EXCEPTION_BOOST
#define DUMP_EXCEPTION_BOOST(Error) \
  std::string msg = std::string( __PRETTY_FUNCTION__) + "\n\tEXCEPTION : code " + std::to_string(Error.code().value()) + " - " + __LINE__ + Error.what(); \
  htl::log::Logger::Instance().Info("console", std::string(msg) ); \
  htl::log::Logger::Instance().Info("file", std::string(msg) );
#endif

#ifndef DUMP_ERRNO
#define DUMP_ERRNO \
  std::string msg = std::string( __PRETTY_FUNCTION__ ) + ". errno: " + std::to_string(errno); \
  htl::log::Logger::Instance().Info("console", std::string(msg) ); \
  htl::log::Logger::Instance().Info("file", std::string(msg) );
#endif

#ifndef DUMP_CRITICAL_CONSOLE
#define DUMP_CRITICAL_CONSOLE(msg) \
{ \
  std::string m = std::string( __PRETTY_FUNCTION__ ) + ":\n" + msg; \
  htl::log::Logger::Instance().Critical("console", std::string(m ) ); \
}
#endif

#ifndef DUMP_CRITICAL_FILE
#define DUMP_CRITICAL_FILE(msg) \
{ \
  std::string m = std::string( __PRETTY_FUNCTION__ ) + ":\n" + msg; \
  htl::log::Logger::Instance().Critical("file", std::string(m ) ); \
}
#endif

#ifndef DUMP_CRITICAL
#define DUMP_CRITICAL(msg) \
{ \
  std::string caption("КРИТИЧЕСКАЯ ОШИБКА: "); \
  DUMP_CRITICAL_CONSOLE( caption + msg ); \
  DUMP_CRITICAL_FILE( caption + msg ); \
}
#endif

#ifndef DUMP_DEBUG
#define DUMP_DEBUG(msg) \
  std::string msg = std::string( __PRETTY_FUNCTION__ ) + ":\n" + msg; \
  htl::log::Logger::Instance().Debug("console", msg ); \
  htl::log::Logger::Instance().Debug("file", msg );
#endif

//void DUMP_ASIO_ERROR( boost::system::error_code error_code );
#ifndef DUMP_ASIO_ERROR
#define DUMP_ASIO_ERROR(error_code) \
{ \
  std::string msg = std::string("ASIO Error ") + std::to_string(error_code.value())+ ". Мessagе: "+ error_code.message(); \
  htl::log::Logger::Instance().Info("console", msg ); \
  htl::log::Logger::Instance().Info("file", msg ); \
}
#endif

}// namespace

#endif // HTL_LOGGER_H
