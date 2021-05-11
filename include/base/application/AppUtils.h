#ifndef APPUTILS_H
#define APPUTILS_H

#include <AppSkeleton.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace                         htl   {
namespace                         app   {

//----------------------------------------------------------------------------
//TODO: Зарезервировано к разработке
/**
 * @brief Метод Create создает экземпляр приложения наследника
 *        класса @a QCoreApplication.
 * @param argc количество параметров приложения
 * @param argv массив значений параметров ( массив строк в правилах языка "С" )
 * @param params список параметров конструктора экземпляра приложения
 *        типа @a ApplicationT_.
 * @return Признак успешного выполнения метода:
 * @value true экземпляр приложения успешно создан;
 * @value false экземпляр приложения не создан.
 *
 * Метод устанавливает значение указателя @a m_AppPtr на область памяти вновь
 * созанного экземпляра приложения типа @a ApplicationT_.
 *
 * @note Экземпляр не создаётся, если значение указателя @a m_AppPtr занято.
 * Для освобождения значения указателя @a m_AppPtr необходимо применить методы:
 * @a AppFactory::Release, @a AppFactory::Exit или @a AppFactory::Abort
 *
 * @see AppFactory::Release, AppFactory::Exit, AppFactory::Abort
 *
 */
//template< typename SkeletonT_, typename ... Args_ >
//bool Create
//(
//    int                               argc,
//    char                          **  argv,
//    const htl::app::app_validator_t< SkeletonT_ >  & validator,
//    bool                              reallocApp,
//    bool                              parentExit,
//    Args_                         ... params
//) noexcept
//{
//  using skeleton_t    = typename std::decay<SkeletonT_ >::type;
//  using skeleton_ptr  = skeleton_t *;
//  static_assert ( std::is_base_of< AppSkeleton, skeleton_t >::value,
//                  ST_ASSERT_BASE_CLASS_VALIDATOR );

//  skeleton_ptr ptr =
//      & skeleton_t::Instance(
//        argc, argv,
//        reallocApp, parentExit,
//        std::forward<Args_>(params)... );

//  bool retval = nullptr != ptr;
//  if( retval and ptr->IsValid() and ptr->GetProperty().lock()->IsReallocApp() )
//  { // пересоздание приложения
//    ptr->Delete();
//    ptr = & skeleton_t::Instance( argc, argv, reallocApp, parentExit );
//  }

//  try
//  {
//    retval = ( nullptr != ptr ) and std::__invoke( validator, *ptr );
//  }
//  catch( const std::exception & e )
//  {
//    retval = false;
//    DUMP_EXCEPTION( e )
//  }
//  return retval;
//}

//----------------------------------------------------------------------------
/**
 * @brief Статический шаблонный метод Fork "демонизирует" экземпляр приложения
 *        типа @a ApplicationT_.
 * @param argc количество параметров приложения
 * @param argv массив значений параметров ( массив строк в правилах языка "С" )
 * @param recreate признак пересодания (значение treue) экземпляра приложения
 *        вместо существующего.
 * @param method метод реализации алгоритма приложения
 * @param params список параметров конструктора экземпляра приложения
 *        типа @a ApplicationT_.
 * @value EXIT_SUCCESS признак успешного выполнения;
 * @value EXIT_FAILURE признак выполнения с ошибкой.
 * @value другой - признак выполнения метода @a method.
 *
 * При пересоздании экземпляра приложения, тип нового может обеспечивать
 * графическое представление (на основе QApplication, например) в отличие,
 * скажем, существующегона основе консольного (на основе QCoreApplication)
 * и наоборот.
 */
//template< typename SkeletonT_, typename ... Args_ >
//int Fork
//(
//    const htl::app::app_validator_t< SkeletonT_ >  & validator
//) noexcept
//{
//  using skeleton_t    = typename std::decay<SkeletonT_ >::type;
//  using skeleton_ptr  = skeleton_t *;
//  static_assert ( std::is_base_of< AppSkeleton, skeleton_t >::value,
//                  ST_ASSERT_BASE_CLASS_VALIDATOR );

//  skeleton_ptr ptr = & skeleton_t::Instance();

//  if( (nullptr == ptr) or ( not ptr->IsValid() ) )
//    return EXIT_FAILURE;

//  auto weak_prop = ptr->GetProperty().lock();
//  auto arguments = weak_prop->Arguments();
////  m_app_ptr = new QtAppSkeleton<ApplicationT_, Args_...>::type( args.first, * args.second );

//  auto pid( ::fork() );

//  if( pid == -1 )
//  {
//    // ошибка fork()
////    AppFactory::SetState( AppState::ErrorFork );
//    return EXIT_FAILURE;
//  }

//  if( ( pid > 0 ) and arguments.IsParentExit() )
//  {
//    // в родительском процессе
//    ptr->Delete();
//  }

//  // в дочернем процессе
//  if( method.operator bool ()
//      /*and
//      htl::app::Create< skeleton_t >(
//        argc, argv,
//        validator,
//        recreate, false,
//        std::forward< Args_&& >( params )... )*/ )
//  {
//    try
//    {
//      if( htl::utils::IsSuperuser() )
//      {
//        ::umask( 0 );
//        ::setsid();
//        ::chdir( DIR_SEPARATOR );
//      }
//      ptr->Exec( true, method );
//      return ptr->GetCode ();
//    }
//    catch( const std::exception & e )
//    {
//      DUMP_EXCEPTION( e );
////      self_t::SetState( AppState::Exception );
//    }
//  }
////  self_t::SetState( AppState::ErrorNoMemory );
//  return EXIT_FAILURE;
//}

//  //----------------------------------------------------------------------------
//  template< typename ... Args_ >
//  static int ApplicationService
//  (
//      int                               argc,
//      char                           ** argv,
//      const app_validator_t< AppT_ >  & startValidator,
//      const std::function< int() >    & method,
//      bool                              recreate,
//      bool                              parentExit,
//      Args_                      && ... params
//  ) noexcept
//  {
//    if( AppFactory::Exists( true, ( argc > 0 ? argv[0] : std::string() ) ) )
//    {
//      if( m_ExistsCallback )
//        m_ExistsCallback();
//      self_t::Abort();
//    }

//    return
//        AppFactory::Fork< Args_... >( argc, argv,
//           startValidator,
//           method,
//           recreate,
//           parentExit,
//           std::forward< Args_&& >( params ) ... );
//  }

//------------------------------------------------------------------------------

}// namespace                     app
}// namespace                     htl


#endif // APPUTILS_H
