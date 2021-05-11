#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/cfg/env.h"
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/fmt/ostr.h" // must be included
#include "spdlog/sinks/syslog_sink.h"
#include "spdlog/pattern_formatter.h"
#include "spdlog/cfg/env.h"

#include <boost/filesystem.hpp>

namespace htl   {
namespace log   {

Logger::Logger()
{
    spdlog::cfg::load_env_levels();
    m_loggers["console"] = spdlog::stdout_color_mt("console");
    m_loggers["console"]->info("\n*** Start CONCOLE logger. Version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    m_loggers["console"]->flush();
}

Logger::Logger(const std::string& file_name)
  : Logger::Logger()
{
  if( not file_name.empty() )
    SetFile( file_name );
}

std::string
Logger::FileName() const
{
  boost::filesystem::path p( m_file_name );
  return p.stem().string() + p.extension().string();
}

std::string
Logger::AbsoluteFileName() const
{
  return MAIN_LOG + "/" + FileName();
}

void
Logger::SetFormapPattern(const std::string& pattern)
{
  auto formatter = std::make_unique< spdlog::pattern_formatter>();
  formatter->add_flag<Logger::formatter_flag>('*').set_pattern( pattern );
  spdlog::set_formatter(std::move(formatter));
}

bool
Logger::IsValid()
const
{
  return not m_loggers.empty();
}

Logger& Logger::Instance(const std::string& file_name)
{
  static Logger retval(file_name);
  return std::ref(retval);
}

void Logger::dump_throw(const std::string& msg)
{
  spdlog::throw_spdlog_ex( msg );
  DUMP_CRITICAL( msg );
}

void
Logger::SetFile(const std::string& file_name)
noexcept
{
  m_file_name = file_name;
  m_loggers["file"] = spdlog::basic_logger_mt<spdlog::async_factory>("file", AbsoluteFileName() );
  m_loggers["file"]->info("\n*** Start FILE logger. Version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  m_loggers["file"]->flush();
}

}// namespace log
}// namespace htl

//namespace {
//void
//DUMP_ASIO_ERROR(boost::system::error_code error_code)
//{
//  std::string msg = std::string("ASIO Error ") + std::to_string(error_code.value())+ ". Мessagе: "+ error_code.message();
//  htl::log::Logger::Instance().Info("console", msg );
//  htl::log::Logger::Instance().Info("file", msg );
//}
//}// namespace
