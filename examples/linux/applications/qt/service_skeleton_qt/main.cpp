#include <application/QtAppSkeleton.h>

#include <QCoreApplication>

#include <iostream>

using namespace htl::app::qt;

int main( int argc, char ** argv )
{
  QtAppSkeleton<QCoreApplication> app( argc, argv );
  auto name( app.Name () );

  //TODO: не рекомендуется передавать в сервис ссылки!
  /**
   * @note  После вызова системного ::fork() объекты родительского процесса могут
   *        исчезуть, и поведение функции станет непредсказуемым.
   * @see man fork
   */
  app.SetEexistsCallback (
  [ name = app.Name () ]()
  {
    std::cout << "Service  " << name.c_str () << " exists. Terminate." << std::endl;
    std::flush(std::cout);
  });

  app.Fork(
        true,
        true,
        [ name = app.Name () ]()
        {
          std::cout << "Execute " << name.c_str () << " as service." << std::endl;
          std::flush(std::cout);
        }
  );
  return  app.GetCode();
}
