#include <application/QtAppSkeleton.h>
#include <QApplication>
#include <QMessageBox>

using namespace htl::app::qt;

void show_message( QtAppSkeleton<QApplication> & )
{
  if( QApplication::instance () )
  QMessageBox::information (nullptr,"GUI Service example.", "enter kill -s 3 <app pid>.");
}

int main( int argc, char ** argv )
{
  QtAppSkeleton<QApplication> app( true, argc, argv );

  app.SetExistsCallback(
  [ & ]()
  {
    QMessageBox::information (nullptr,"GUI Service WARNING.",
                              QString(" GUI Service \"%1\"  exists.").
                              arg (app.Name ().c_str ()));
  });

//  app.Fork( true, true, show_message, app );

  return app.GetCode ();
}
