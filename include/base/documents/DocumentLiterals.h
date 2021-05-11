#ifndef DOCUMENT_LITERALS_H
#define DOCUMENT_LITERALS_H 1

#include <typeinfo>

namespace                                 htl   {
namespace                                 docs  {

using doc_id_t                          = std::size_t;
//------------------------------------------------------------------------------

const doc_id_t DOC_UNDEFINED_ID      = 0;

const doc_id_t DOC_VALUE_ID          = 1;

const doc_id_t DOC_DEVICE_ID         = 10;
const doc_id_t DOC_DEVICES_ID        = DOC_DEVICE_ID + 1;
const doc_id_t DOC_NETIFACE_ID       = DOC_DEVICE_ID + 2;
const doc_id_t DOC_NETIFACES_ID      = DOC_DEVICE_ID + 3;
const doc_id_t DOC_NETECHO_ID        = DOC_DEVICE_ID + 4;

const doc_id_t DOC_USER_ID           = 20;
const doc_id_t DOC_USERS_ID          = DOC_USER_ID + 1;
const doc_id_t DOC_GROUP_ID          = DOC_USERS_ID + 1;
const doc_id_t DOC_GROUPS_ID         = DOC_GROUP_ID + 1;

const doc_id_t DOC_DOCPKG_ID          = 30;

const doc_id_t DOC_BYTEARRAY_ID      = 40;
const doc_id_t DOC_BYTESTUFFING_ID   = 50;

const doc_id_t DOC_ICMP_ID           = 400;
const doc_id_t DOC_IP4_ID            = DOC_ICMP_ID + 10;
const doc_id_t DOC_PING_INFO_ID      = DOC_ICMP_ID + 20;
const doc_id_t DOC_PING_CTRL_ID      = DOC_ICMP_ID + 21;

const doc_id_t DOC_MBGL_HASHTABLE_ID = 500;
const doc_id_t DOC_MBGL_SRC_CONTAINER_ID = 501;

const doc_id_t DOC_MBGL_LAYER_ID = 1100;
const doc_id_t DOC_MBGL_LAYER_DATA_ID = 600;


const doc_id_t DOC_APP_PROP_ID        = 700;
const doc_id_t DOC_QT_APP_ID          = 701;

const doc_id_t DOC_HTTP_ID            = 800;
const doc_id_t DOC_HTTP_HEADER_ID     = DOC_HTTP_ID + 1;

const doc_id_t DOC_AIRHELI_ID         = 900;
const doc_id_t DOC_AIRHELI_DISTR_ID   = 901;
const doc_id_t DOC_AIRHELI_CHART_ID   = 902;

const doc_id_t DOC_RADIO_ID           = 1000;
const doc_id_t DOC_RADIO_COLLECT_ID   = 1001;

const doc_id_t DOC_VERIFY_ID          = 1100;
const doc_id_t DOC_VERIFY_FACTORY_ID  = 1101;

//------------------------------------------------------------------------------

} //namespace                     docs
} //namespace                     htl

#endif // DOCUMENT_LITERALS_H
