#ifndef QTDBTYPES_H
#define QTDBTYPES_H

#include <QStringList>
#include <QString>

namespace                         htl   {
namespace                         qt    {
namespace                         db   {

//------------------------------------------------------------------------------
/**
 * @brief Request_INSERT
 * @arg %1 - table name
 * @arg %2 - fields name
 * @arg %2 - values : value == "?", count == fields count
 *
 * @example
 * auto req_insert( Request_INSERT.
 *                  arg( Table_RoutePointTable ).
 *                  arg( "Position, Name, Latitude, RouteId, Longitude, Type" ).
 *                  arg( "?, ?, ?, ?, ?, ?" ) );
 */
const QString Request_INSERT("INSERT INTO %1 (%2) VALUES(%3)");

QString   BuildRequestInsert
(
    const QString     & table_name,
    const QStringList & fields_names
);

//------------------------------------------------------------------------------
/**
 * @brief Request_DELETE
 * @arg %1 - table name
 * @arg %2 -- WHERE (default empty)
 *
 * @example
 * auto req_del_1( Request_DELETE.
 *                  arg( Table_NightMode ).
 *                  arg( QString() ) );
 * auto req_del_2( Request_DELETE.
 *                  arg( Table_RouteTable ).
 *                  arg( QString("WHERE RouteId = ?") ) );
 */
const QString Request_DELETE("DELETE FROM %1 %2");

QString   BuildRequestDelete
(
    const QString     & table_name,
    const QStringList & where_fields_names = QStringList()
);

//------------------------------------------------------------------------------
/**
 * @brief Request_SELECT
 * @arg %2 - fields (default "*" )
 * @arg %1 - table name
 * @arg %3 - WHERE    (default empty)
 * @arg %4 - ORDER BY (default empty)
 */
const QString Request_SELECT("SELECT %2 FROM %1 %3 %4 %5");

QString   BuildRequestSelect
(
    const QString     & table_name,
    const QStringList & fields_names        = QStringList(), /* empty list equal "*" */
    quint64             limit               = 0,
    const QStringList & where_fields_names  = QStringList(),
    const QStringList & order_fields_names  = QStringList(),
    bool                is_order_asc        = true
);

//------------------------------------------------------------------------------
/**
 * @brief Request_UPDATE
 * @arg %1 - table name (required)
 * @arg %2 - fields     (required)
 * @arg %3 -- WHERE     (default empty)
 */
const QString Request_UPDATE("UPDATE %1 %2 %3");

QString   BuildRequestUpdate
(
    const QString     & table_name,
    const QStringList & fields_names,
    const QStringList & where_fields_names  = QStringList()
);

}// namespace                     db
}// namespace                     qt
}// namespace                     htl


#endif // QTDBTYPES_H
