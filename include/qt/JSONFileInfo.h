#ifndef JSONFILEINFO_H
#define JSONFILEINFO_H

#include "JSONFilesFactory.h"
#include "Document.h"

#include <QVariantMap>

namespace htl     {
namespace qt      {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        JSONFileInfo
: public                          QObject
{
  Q_OBJECT

public:
  using base_class_t            = QObject;

  explicit                        JSONFileInfo  ( QObject * parent = Q_NULLPTR );
  virtual                       ~ JSONFileInfo  ()  {}

  Q_SLOT virtual void             OnUpdateFile  () = 0;

protected:
  Q_SLOT virtual bool             SetConnections();

private:
  void                            Init          () noexcept;
};

//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl

#endif // JSONFILEINFO_H
