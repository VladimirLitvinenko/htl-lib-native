#ifndef APPSKELETON_H
#define APPSKELETON_H

#include "HtlGlobal"
#include "Utils.h"
#include "AppProperty.h"
#include "SysSignalsController.h"
#include "Types.h"
#include "Logger.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <boost/bind/bind.hpp>

#include <mutex>
#include <condition_variable>
#include <string>
#include <functional>

namespace                         htl   {
namespace                         app   {

//----------------------------------------------------------------------------

using appexists_action_t        = std::function< void() >;

template< typename ResultT_,  typename ... ArgsT_ >
using main_method_t             = std::function< ResultT_ ( ArgsT_ ... ) > ;

//template< typename SkeletonT_ >
//using app_validator_t           = std::function< bool( SkeletonT_ & ) >;

//------------------------------------------------------------------------------
inline std::pair<pid_t, bool>
ApplicationExists ( const std::string & appName = std::string() )
{
  assert ( not appName.empty() );
  return htl::utils::ProcessIsActive( appName );
}

//------------------------------------------------------------------------------
/**
 * @brief Класс AppSkeleton реализует методы и свойства исполняемого приложения.
 */
class COMMON_SHARED_EXPORT        AppSkeleton
: public                          htl::sys::SysSignalsController
{
public:
  using base_class_t            = htl::sys::SysSignalsController;

  /**/                            AppSkeleton( int argc, char ** argv);
  virtual                       ~ AppSkeleton () {}

  /**
   * @brief Метод SetAction устанавливает метод (функцию) как главный метод исполняемого приложения
   * @param action - указатель, ссылка на метод
   * @param params - набор значений параметров
   * @return признак успешной (true) или неудачной (false) установки главного метода
   */
  template< typename Action_, typename ... ArgsT_ >
  bool                            SetAction   ( Action_ action, ArgsT_... params ) noexcept;

  /**
   * @brief Метод IsValid возвращает признак готовности объекта класса к дальнейшим действиям над ним.
   * @return Булево значение:
   * @value true  объекта класса находится в корректном состоянии
   * @value false объекта класса необходимо проверить и скорректировать статусы.
   */
  virtual bool                    IsValid     () const;
  /**
   * @brief Метод IsActive возвращает признак состояния главного метода приложения как не прерванного
   * @return Булево значение:
   * @value true  метод выполняется
   * @value false метод не выполняется
   */
  bool                            IsActive    () const;
  /**
   * @brief Метод IsComplete возвращает признак состояния главного метода приложения как успешно завершенного
   * @return Булево значение:
   * @value true  метод успешно завершен
   * @value false метод не успешно завершен
   */
  bool                            IsComplete  () const;

  /**
   * @brief Метод InitSignalsDefault инициализирует правила обработки сигналов экземпляром класса по умолчанию.
   * Сигналы:
   *    SIGTRAP,
   *    SIGCLD,
   *    SIGCHLD,
   *    SIGSTOP,
   *    SIGTSTP,
   *    SIGPROF,
   *    SIGWINCH
   * игнорируются.
   * Сигналам присваиваются обработчики:
   *    SIGINT  -> [this]( int sn ){ this->Quit   ( sn ); }
   *    SIGHUP  -> [this]( int sn ){ this->Reload ( sn ); }
   *    SIGQUIT -> [this]( int sn ){ this->Quit   ( sn ); }
   *    SIGTERM -> [this]( int sn ){ this->Quit   ( sn ); }
   *    SIGABRT -> [this]( int sn ){ this->Abort  ( sn ); }
   */
  void                            InitSignalsDefault  ();
  /**
   * @brief Метод IsUnique возвращает признак уникальности приложения в списке
   * мсполняемых операционной системой.
   * Если приложение помечено как уникальное, невозможно запустить второй экземпляр.
   * @return Булево значение:
   * @value true  приложение помечено как уникальное
   * @value false приложение НЕ помечено как уникальное
   */
  bool                            IsUnique    () const;
  /**
   * @brief Метод BlockMode возвращает признак блокируемости доальнейших действий приложения
   * до завершения исполнения головного метода.
   * @return Булево значение:
   * @value true  режим блокировки включен
   * @value false режим блокировки отключен
   * @return
   */
  virtual bool                    BlockMode   () const;
  /**
   * @brief Метод SetBlockMode устанавливает признак блокируемости доальнейших действий приложения
   * до завершения исполнения головного метода.
   * @param mode Булево значение:
   * @value true  режим блокировки включен
   * @value false режим блокировки отключен
   */
  virtual void                    SetBlockMode( bool mode );
  /**
   * @brief Метод SetUnique устанавливает признак уникальности приложения в списке
   * мсполняемых операционной системой.
   * @param uniqueMode Булево значение:
   * @value true  включен
   * @value false отключен
   */
  void                            SetUnique   ( bool uniqueMode );

  /**
   * @brief Метод Name возвращает название приложения без пути запуска
   * @return название приложения
   */
  std::string                     Name              () const;
  /**
   * @brief Метод Path возвращает каталог запуска приложения
   * @return каталог запуска приложения
   */
  std::string                     Path              () const;
  /**
   * @brief Метод Start запускакт метод @a Execute правил выполнения головной
   * программы после проверки и установки обработчика сигналов.
   */
  virtual void                    Start             ();
  /**
   * @brief Метод Stop останавливает выполнения головной программы
   */
  virtual void                    Stop              ();
  /**
   * @brief Метод Restart перезапускает выполнения головной программы
   * @param code
   */
  virtual void                    Restart           ( int code );
  /**
   * @brief Метод Reload перечитывает возможную конфигурацию и
   * перезапускает выполнения головной программы
   * @param code
   */
  virtual void                    Reload            ( int code );
  /**
   * @brief Метод Quit корректно завершает выполнение приложения и назначает
   * код выполнения.
   * @param code код выполнения.
   */
  virtual void                    Quit              ( int code );
  /**
   * @brief Метод Abort экстренно завершает выполнение приложения и назначает
   * @param code код выполнения.
   */
  virtual void                    Abort             ( int );
  /**
   * @brief Метод Delete удаляет экземпляр класса
   */
  virtual void                    Delete            ();
  /**
   * @brief Метод Exec блокирует дальнейшее выполнение приложения до
   * завершения головного метода.
   * @return код результата выполнения.
   */
  virtual int                     Exec              ();

  /**
   * @brief Метод GetPid возвращает значение PID выполняемого приложения
   * @return значение PID выполняемого приложения
   */
  pid_t                           GetPid            () const  { return  m_pid; }
  /**
   * @brief Метод NewPid проверяет значение PID выполняемого приложения
   * @return значение PID выполняемого приложения
   */
  pid_t                           NewPid            () { m_pid = ::getpid (); return  GetPid (); }
  /**
   * @brief Метод GetCode возвращает код статуса приложения.
   * @return код статуса приложения
   */
  int                             GetCode           () const  { return  m_code; }
  /**
   * @brief Метод IsCodeSuccess возвращает признак коррекного кода статуса приложения
   * @param mode Булево значение:
   * @value true  код статуса корректен
   * @value false код статуса ошибочен
   */
  bool                            IsCodeSuccess     () const  { return  GetCode() == EXIT_SUCCESS; }
  /**
   * @brief Метод SetCode принудительно назначает код статуса
   * @param code значение кода статуса
   */
  void                            SetCode           ( const decltype (EXIT_SUCCESS) code );
  /**
   * @brief Метод GetProperty возвращает слабо звязный указатель на экземпляр свойств приложения
   * @return слабо звязный указатель на экземпляр свойств приложения
   */
  appproperty_week_t              GetProperty       () const;
  /**
   * @brief Метод SetExistsCallback устанавливает метод обработки ситуации, если приложение
   * запускается повторно при уникальности экземпляра класса.
   * @param callback метод
   * @see IsUnique
   */
  void                            SetExistsCallback( appexists_action_t callback );
  /**
   * @brief Метод ExistsCallback вызывает метод обработки ситуации, если приложение
   * запускается повторно при уникальности экземпляра класса.
   * @return пара значений:
   * @value pid_t PID ранее запущенного приложения
   * @value bool приложение запущено ранее
   */
  std::pair<pid_t, bool>          ExistsCallback   ();
  /**
   * @brief Метод MakeArguments формирует свойства приложения согласно аргументам его запуска
   * @param argc количество аргументов
   * @param argv указатели на массив char * аргументов
   * @return
   */
  AppSkeleton                   & MakeArguments     ( int argc, char ** argv ) noexcept;

protected:
  virtual AppSkeleton           & operator=         ( appproperty_shared_t && app_property );
  /**
   * @brief Метод SetPid  принудительно назначает новый PID приложению
   * @param pid новое значение PID
   */
  void                            SetPid            ( pid_t pid );

  /// Блок управления приложением
  struct app_state
  {
    enum class status
    {
      Idle,       ///< простаивает
      Run,        ///< выполняется
      Complete,   ///< успешно завершена работа
      Terminate,  ///< прерван принудительно
    };

    std::atomic<status>             m_status    { status::Idle };
    std::atomic_bool                m_block_mode{ false };
    mutable std::mutex              m_mutex;
    mutable std::condition_variable m_condition;

    bool is_run       () const { return m_status == status::Run; }
    bool is_complete  () const { return m_status == status::Complete; }
    bool is_idle      () const { return m_status == status::Idle; }
    bool is_terminate () const { return m_status == status::Terminate; }
    bool is_block     () const { return m_block_mode; }

    /**
     * @brief Метод starter производит попытку блокировки исполнения до наступления услови отмены
     * @param app ссылка на экземпляр данного класса
     */
    void  starter( AppSkeleton & app );

    /**
     * @brief Метод executor производит выполнения головного метода
     * @param app ссылка на экземпляр данного класса
     */
    void executor( AppSkeleton & app );
  };

  pid_t                           m_pid           { -1 };
  app_state                       m_app_state;
  /**
   * @brief Аргумент m_action_ptr содержит общий указатель главный метод исполняемого приложения.
   */
  utils::action_ptr               m_action_ptr;

  /**
   * @brief GetCodeRef
   * @return
   */
  decltype(EXIT_SUCCESS)        & GetCodeRef        () { return std::ref( m_code); }
  /**
   * @brief Execute
   * @return
   */
  virtual bool                    Execute           () noexcept;

private:
  appproperty_shared_t            m_propertyPtr;
  /**
   * @brief Атрибут m_unique_mode содержит признак уникальности сервиса.
   * При значении true сервис не может запуститься повторно при наличии процесса
   * ранее запущенного сервиса.
   */
  std::atomic_bool                m_unique_mode { false };
  appexists_action_t              m_exists_callback;
  int                             m_code          = EXIT_SUCCESS;
  htl::threadptr_t                m_starter;
  htl::threadptr_t                m_executer;

protected:
  void SetStatus                  ( app_state::status s );
};


template< typename Action_, typename ... ArgsT_ >
bool
AppSkeleton::SetAction   ( Action_ action, ArgsT_... params )
noexcept
{
//  DUMP_INFO_CONSOLE( __PRETTY_FUNCTION__ );
  BEGIN_LOCK_SECTION_SELF_;
  try
  {
    m_action_ptr =
        std::make_shared<utils::action_p_t<Action_, ArgsT_... > >
        ( std::forward< Action_ >( action ),
          std::forward< ArgsT_  >( params ) ... );
  }
  catch ( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    m_action_ptr.reset();
    return false;
  }
  return true;
  END_LOCK_SECTION_;
}

//------------------------------------------------------------------------------

}// namespace                     app
}// namespace                     htl

#endif // APPSKELETON_H
