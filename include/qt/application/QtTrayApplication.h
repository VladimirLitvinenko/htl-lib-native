#ifndef QTTRAYAPPLICATION_H
#define QTTRAYAPPLICATION_H

#include <Application.h>

#include <QSystemTrayIcon>
#include <QApplication>

namespace                         spo   {
namespace                         app   {

//----------------------------------------------------------------------------
#ifndef QT_NO_SYSTEMTRAYICON
/**
 * @brief Статический метод CreateTray создает экземпляр приложения наследника
 *        класса @a QApplication. Приложение располагается в трее Рабочего стола
 *        моконного менеджера.
 * @param argc количество параметров приложения
 * @param argv массив значений параметров ( массив строк в правилах языка "С" )
 * @param params список параметров конструктора экземпляра приложения
 *        типа @a ApplicationT_.
 * @return Признак успешного выполнения метода:
 * @value true экземпляр приложения успешно создан;
 * @value false экземпляр приложения не создан.
 *
 * Метод основан на @a AppFactory::Create.
 * При создании проверяется наличие ранее созданного экземпляра приложения
 * типа AppT_. При наличии ранее созданного экземпляра приложения, новый
 * экземпляр не сосздаётся. При наличии обработчика события проверки ранее
 * созданного экземпляра (callback @a m_ExistsHandler) производится попытка
 * его выполнения.
 *
 * @note Экземпляр не создаётся, если значение указателя @a m_Instance занято.
 * Для освобождения значения указателя @a m_Instance необходимо применить методы:
 * @a AppFactory::Release, @a AppFactory::Exit или @a AppFactory::Abort
 *
 * @see AppFactory::Release, AppFactory::Exit, AppFactory::Abort
 *
 */
template
<
    typename                    AppT_,
    typename                ... Args_,
    typename                  = typename std::enable_if< std::is_base_of< QApplication, AppT_ >::value >::type
>
static bool                     CreateTray
(
    int                         argc,
    char                     ** argv
) noexcept
{
  bool retval( not AppFactory<AppT_>::IsValid() );
  if( retval )
  {
    retval =
        AppFactory<AppT_>::Create(
          argc, argv,
          []( AppT_ * )
          {
            if( AppFactory<AppT_>::Exists() )
            {
              return AppFactory<AppT_>::ExistsCallback();
            }
            return true;
          },
          true,
          std::forward< Args_ >( params ) ... );

    if( retval  )
    {
      retval = QSystemTrayIcon::isSystemTrayAvailable();
    }

    if( retval  )
    {
      QApplication::setQuitOnLastWindowClosed( false );
    }
    else
    {
      AppFactory<AppT_>::SetState( AppState::ErrorValidator );
    }
  }
  return retval;
}

//----------------------------------------------------------------------------
/**
 * \brief СТАТИЧЕСКИЙ метод ApplicationTrayMain представляет обвёртку наз
 *        методом @a AppFactory::CreateTray c выполнением алгоритма после
 *        успешного создания экземпляра приложения.
 * @param argc количество параметров приложения
 * @param argv массив значений параметров ( массив строк в правилах языка "С" )
 * @param appBody метод реализации алгоритма приложения
 * @param params список параметров конструктора экземпляра приложения
 *        типа @a ApplicationT_.
 * @return Признак выполнения приложения:
 * @value EXIT_SUCCESS признак успешного выполнения;
 * @value EXIT_FAILURE признак выполнения с ошибкой.
 * @value другой - признак выполнения метода @a appBody.
 *
 * Например:
 * @code language="cpp"
 *
 * #include "include/MyApplication.h"
 * #include "include/MyMainWindow.h"
 * #include <QMessageBox>
 *
 * int main(int argc, char *argv[])
 * {
 *   try
 *   {
 *     using namespace spo::app;
 *     AppFactory::AssignExistsCallback(
 *           []()
 *           {
 *             QMessageBox::information( nullptr,
 *                                       "Предупреждение!",
 *                                       QString("Экземпляр приложения %1 создан ранее.").arg( AppFactory::Name() ),
 *                                       "Закрыть сообщение" );
 *           } );
 *     return
 *         AppFactory::ApplicationTrayMain< MyApplication >(
 *           argc, argv, []()
 *           {
 *             if( nullptr != dynamic_cast< MyApplication * >( AppFactory::Application() ) )
 *             {
 *               MyMainWindow w;
 *               w.setVisible( false );
 *               return AppFactory::AppExec();
 *             }
 *             return EXIT_FAILURE;
 *           } );
 *   }
 *   catch( const std::exception & e )
 *   {
 *     DUMP_EXCEPTION( e );
 *   }
 *   return EXIT_FAILURE;
 * }
 * @endcode
 *
 */
template
<
    typename                    AppT_,
    typename                ... Args_,
    typename                  = typename std::enable_if< std::is_base_of< QApplication, AppT_ >::value >::type
>
static int                      ApplicationTrayMain
(
    int                             argc,
    char                         ** argv,
    spo::simple_fnc_t< int >        appBody,
    Args_                       ... params
) noexcept
{
  if( spo::app::CreateTray< AppT_, Args_...>( argc, argv, std::forward< Args_ >(params)... ) )
  {
    try
    {
      return
          appBody
          ? appBody()
          : EXIT_FAILURE ;
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION( e );
      AppFactory<AppT_>::SetState( AppState::Exception );
    }
  }
  return EXIT_FAILURE;
}
#endif

//------------------------------------------------------------------------------

}// namespace                   app
}// namespace                   spo

#endif // QTTRAYAPPLICATION_H
