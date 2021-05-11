#include "QtDbTypes.h"

namespace                         htl   {
namespace                         qt    {
namespace                         db   {

QString
BuildRequestInsert(const QString& table_name, const QStringList& fields_names)
{
  if( fields_names.isEmpty() or table_name.isEmpty() )
    return QString();

  QString fields;
  QString values;
  for( auto & fld : fields_names )
  {
    fields += QString("%1,").arg(fld);
    values += "?,";
  }
  for( auto & str : QStringList() << fields << values )
    if( not str.isEmpty() )
      str.chop(1);

  return Request_INSERT.arg(table_name).arg(fields).arg(values);
}

QString
BuildRequestDelete(const QString& table_name, const QStringList& where_fields_names)
{
  if( table_name.isEmpty() )
    return QString();

  QString where( where_fields_names.isEmpty() ? QString() : "WHERE " );
  for( auto & fld : where_fields_names )
  {
    where += QString("(%1 = ?) AND").arg(fld);
  }
  if( not where.isEmpty() )
    where.chop(4);

  return Request_DELETE.arg(table_name).arg(where);
}

QString
BuildRequestSelect
(
    const QString     & table_name,
    const QStringList & fields_names,
    quint64             limit,
    const QStringList & where_fields_names,
    const QStringList & order_fields_names,
    bool                is_order_asc
)
{
  if( table_name.isEmpty() )
    return QString();

  QString fields;
  for( auto & f : fields_names)
  {
    fields += QString("%1,").arg(f);
  }

  if( fields.isEmpty() )
    fields = "*";
  else
    fields.chop(1);

  QString limit_str( limit > 0 ? QString("LIMIT %1").arg(limit) : QString() );
  QString where( where_fields_names.isEmpty() ? QString() : "WHERE " );
  for( auto & fld : where_fields_names )
  {
    where += QString("(%1 = ?) AND").arg(fld);
  }
  if( not where.isEmpty() )
    where.chop(4);

  QString order( order_fields_names.isEmpty() ? QString() : "ORDER BY " );
  for( auto & fld : order_fields_names )
  {
    order += QString("%1,").arg(fld);
  }
  if( not order.isEmpty() )
  {
    order.chop(1);
    order += QString(is_order_asc ? "ASC" : "DESC" );
  }

  return Request_SELECT.arg(table_name).arg(fields).arg(limit_str).arg(where).arg(order);
}

QString
BuildRequestUpdate
(
    const QString     & table_name,
    const QStringList & fields_names,
    const QStringList & where_fields_names
)
{
  if( table_name.isEmpty() or fields_names.isEmpty() )
    return QString();

  QString fields;
  for( auto & f : fields_names )
  {
    fields += QString("SET %1 = ?,").arg(f);
  }

  if( not fields.isEmpty() )
    fields.chop(1);

  QString where( where_fields_names.isEmpty() ? QString() : "WHERE " );
  for( auto & fld : where_fields_names )
  {
    where += QString("(%1 = ?) AND").arg(fld);
  }
  if( not where.isEmpty() )
    where.chop(4/* last " AND" literal*/);

  return Request_UPDATE.arg(table_name).arg(fields).arg(where);
}


}// namespace                     db
}// namespace                     qt
}// namespace                     htl
