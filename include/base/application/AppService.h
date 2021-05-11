#ifndef APP_SERVICE_H
#define APP_SERVICE_H

#include "Types.h"
#include "Utils.h"
#include "AppSkeleton.h"
#include "ArgumentLiterals.h"
#include "AsioCommon.h"

namespace                         htl   {
namespace                         app   {

//------------------------------------------------------------------------------
template< typename                SkeletonT_  = htl::app::AppSkeleton >
/**
 * @brief Шаблонный класс AppService реализует оболочку подготовки сервиса для
 *        приложений, основанных на классе @a htl::app::AppSkeleton.
 */
class COMMON_SHARED_EXPORT          AppService
: public                            SkeletonT_
{
  static_assert ( std::is_base_of<htl::app::AppSkeleton, typename std::decay<SkeletonT_>::type >::value,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using app_type    = SkeletonT_;

  template< typename ... AppArguments_ >
  /**
   * @brief Конструктор AppService<SkeletonT_>::AppService создаеь экземпляр сервиса
   * для приложения на основе класса @a SkeletonT_.
   * @param argc количество параметров командной строки;
   * @param argv значения параметров командной строки;
   * @param params возможный список дополнительных параметров для конструктора
   *        клвсса приложения на основе класса @a SkeletonT_.
   */
  /**/                AppService
  (
      int               argc,
      char           ** argv,
      AppArguments_ ... params
  );

  /**/              ~ AppService()    {}

  virtual bool        MakePidFile     ();

  std::string         PidFile         () const;

  /**
   * @brief Метод Fork производит разветвление процессов как самостоятельных с
   * завершением выполнения родительского и перевода дочернего как сервиса под
   * управдение init unix
   * @param blockMode признак блокировки приложения до завершения выполнения
   * головной программы
   * @param uniqueMode признак уникальности приложения (выполнения в единичном экземпляре)
   * @return признак выполнения
   * @value 3 - невалидный экземпляр приложения;
   * @value EXIT_FAILURE - ошибка запуска
   * @value EXIT_SUCCESS - успешное разветвление
   */
  virtual int         Fork              () noexcept;

  /**
   * @brief Метод RunService переводит приложение в разряд сервиса (демона)
   *
   * Постоянная попытка запуска @a Fork при ошибке.
   */
  void                RunService      ();

protected:
  file_path_t         MakeFile        ( const std::string & path_name, const std::string & file_ext );

private:
  /**
   * @brief Атрибут m_file_pid содержит описание файла  со значениями PID
   * запущенных сервисов.
   */
  boost::filesystem::path         m_file_pid;
};


//------------------------------------------------------------------------------
template<typename SkeletonT_>
template< typename ... AppArguments_ >
AppService<SkeletonT_>::AppService
(
    int                   argc,
    char               ** argv,
    AppArguments_     ... params
) : SkeletonT_ ( argc, argv, std::forward<AppArguments_>(params)... )
  , m_file_pid ( MAIN_CONF )
{
}

template<typename SkeletonT_>
/**
 * @brief Метод AppService<SkeletonT_>::MakeFile
 * @param file
 * @param file_ext
 * @return
 */
file_path_t
AppService<SkeletonT_>::MakeFile( const std::string & path_name, const std::string & file_ext )
{
  file_path_t retval( path_name + std::string(DIR_SEPARATOR) + SkeletonT_::Name () + file_ext );
  std::pair< std::string, bool > file_info(
        htl::utils::MakeFile ( retval.generic_string () ) );
  if( file_info.second )
    retval  = file_info.first;
  else
    retval.clear();

  return retval;
}

template<typename SkeletonT_>
/**
 * @brief Метод PidFile возвращает название файла, хранящего значение PID
 * текущего экземпляра приложения.
 * @return название файла.
 */
std::string
AppService<SkeletonT_>::  PidFile() const
{
  return  m_file_pid.string ();
}

template<typename SkeletonT_>
int
AppService<SkeletonT_>::Fork()
noexcept
{
  if( not SkeletonT_::IsValid() )
  {
    DUMP_INFO(std::string(__PRETTY_FUNCTION__) + ": SkeletonT_ is not Valid.");
    return 3;
  }

  try
  {
    switch ( ::fork() )
    {
      case -1 : // ошибка fork()
        DUMP_INFO("FORK error. Abort.");
        SkeletonT_::Abort( EXIT_FAILURE );
        return EXIT_FAILURE;
      case 0 :
      {// дочерний процесс. Сервис.
        break;
      }
      default:// родительском процессе
        DUMP_INFO("Parent QUIT.");
        SkeletonT_::Quit ( EXIT_SUCCESS ) ;
        return EXIT_SUCCESS;
    }

    SkeletonT_::Start();
    MakePidFile();

    if( htl::utils::IsSuperuser() )
    {
      DUMP_INFO("Run superuser fork-settings.");
      ::umask( 0 );
      ::setsid();
      ::chdir("/");
    }
    ::close(STDIN_FILENO);
    ::close(STDOUT_FILENO);
    ::close(STDERR_FILENO);

    return SkeletonT_::Exec();
  }
  catch ( const std::exception & e )
  {
    DUMP_EXCEPTION ( e );
    SkeletonT_::Abort( EXIT_FAILURE );
  }
  return EXIT_FAILURE;
}

template<typename SkeletonT_>
/**
 * @brief AppService<SkeletonT_>::MakePidFile
 * @return
 */
bool
AppService<SkeletonT_>::MakePidFile()
{
  bool retval( SkeletonT_::IsValid () );
  if( retval )
  {
    SkeletonT_::NewPid ();

    file_path_t pid_file = MakeFile ( m_file_pid.string(), htl::app::EXT_FILE_PID );
    retval = not pid_file.empty();
    if( retval )
    {
      std::ofstream w_stream( pid_file.generic_string(), std::ios::out | std::ios::trunc );
      retval = w_stream.operator bool ();
      if( retval )
      {
        w_stream << SkeletonT_::GetPid () << "\n";
      }
    }
  }
  return retval;
}

template<typename SkeletonT_>
/**
 * @brief AppService<SkeletonT_>::Start
 * @param uniqueMode
 * @param action
 * @param params
 */
void
AppService<SkeletonT_>::RunService()
{
  SkeletonT_::SetBlockMode( true );
  SkeletonT_::SetCode( EXIT_SUCCESS );
  uint8_t cnt{5};
  while( (EXIT_SUCCESS != Fork()) and (cnt > 0) )
  {
    --cnt;
    std::this_thread::sleep_for( std::chrono::milliseconds(100) );
  }
}

//------------------------------------------------------------------------------

}// namespace                     app
}// namespace                     htl

#endif // APP_SERVICE_H
