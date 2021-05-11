#include <AppService.h>
#include <iostream>

using namespace htl::app;

//TODO: Необходимо помнить, что передача экземпляра AppSkeleton по ссылке НЕЖЕЛАТЕЛЬНА:

/**
 * @brief method
 * @param name
 * @warning Передача экземпляра по ссылке приведет к тому, что завершение работы
 * родительского приложения после ::fork() удалит экземпляр, и будет передана пустая ссылка
 */
void method( const std::string & name )
{
  std::cout << "Execute " << name.c_str () << " as service." << std::endl;
  std::flush(std::cout);
};

int main( int argc, char ** argv )
{
  htl::app::AppService<AppSkeleton> app( argc, argv );

  //TODO: не рекомендуется передавать в сервис ссылки!
  /**
   * @note  После вызова системного ::fork() объекты родительского процесса могут
   *        исчезуть, и поведение функции станет непредсказуемым.
   * @see man fork
   */
  app.SetExistsCallback (
  [ name = app.Name () ]()
  {
    std::cout << "Service  " << name.c_str () << "   exists. Terminate." << std::endl;
    std::flush(std::cout);
  });
  app.SetUnique( true );
  app.RunService();
  return app.GetCode();
}
