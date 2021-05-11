#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "HtlGlobal"
#include "ErrLiterals.h"
#include "DocumentLiterals.h"

#include <mutex>
#include <memory>
#include <map>
#include <functional>
#include <fstream>
#include <assert.h>

#include <boost/filesystem.hpp>

namespace htl   {
namespace docs  {

//------------------------------------------------------------------------------
/**
 * @brief Класс IDocument опреднляет интерфейс для понятия "Документ".
 *
 * Документы могут содержать различный контент, поэтому для хранения документов
 * в контейнере, например, удобно использовать ссылки/указатели на данный класс,
 * предварительно приведя экземпляры к htl::docs::IDocument.
 * Тспользуя значение аттрибута @a htl::docs::IDocument::m_id можно понять, с
 * каким документом приходится иметь дело.
 */
class COMMON_SHARED_EXPORT        IDocument
{
public:
  /**
   * @brief Конструктор IDocument::IDocument определяет неопределенное состояние документа.
   * Метод @a IDocument::IsValid вернет false - экземпляр не готов к работе.
   */
  /**/                IDocument () = default;
  /**
   * @brief Конструктор IDocument назначения идентификатора документу.
   * @param id
   */
  /**/                IDocument ( doc_id_t id ) : m_id( id ) {}
  /**
   * @brief Конструктор IDocument копирования
   * @param d
   */
  /**/                IDocument ( const IDocument & d ) : IDocument::IDocument() { * this = d; }
  /**
   * @brief Конструктор перемещения объекта IDocument.
   * @param в r-value ссылка на перемещаемый экземпляр класса.
   * @see IDocument::operator =
   */
  /**/                IDocument ( IDocument && d ) : IDocument::IDocument() { * this = std::move( d ); }
  virtual           ~ IDocument () {}

  /**
   * @brief Виртуальный метод IsValid возвращает признак отовности экземпляра класса к работе.
   * @return Булево значение:
   * @value true экземпляр класса не содержит ошибок и готов к работе;
   * @value false состояние содержит ощибки.
   */
  virtual bool        IsValid () const noexcept
  {
    return m_id > DOC_UNDEFINED_ID;
  }

  virtual IDocument & operator ()()
  {
    return std::ref( * this );
  }
  virtual IDocument & operator ()( doc_id_t type )
  {
    m_id = type;
    return std::ref( * this );
  }

  virtual IDocument & operator =( const IDocument & d ) noexcept
  {
    BEGIN_LOCK_SECTION_SELF_;
    m_id = d.GetID();
    return *this;
    END_LOCK_SECTION_
  }
  virtual IDocument & operator =( IDocument && d ) noexcept
  {
    BEGIN_LOCK_SECTION_SELF_;
    m_id = d.GetID();
    d.GetIDRef() = DOC_UNDEFINED_ID;
    return *this;
    END_LOCK_SECTION_
  }
  /**
   * @brief Метод GetID
   * @return
   */
  doc_id_t GetID () const { return m_id; }

  /**
   * @brief Метод ClearContent
   */
  virtual void ClearContent (){}

protected:
  /**
   * @brief Атрибут m_Mutex
   */
  mutable std::recursive_mutex  m_Mutex;

  /**
   * @brief Метод GetIDRef
   * @return
   */
  doc_id_t & GetIDRef ()
  {
    return std::ref( m_id );
  }

private:
  /**
   * @brief Атрибут mID содержит идентификатор типа документа
   */
  doc_id_t m_id = DOC_UNDEFINED_ID;
};

//------------------------------------------------------------------------------
using document_shared_t      = std::shared_ptr< htl::docs::IDocument >;
using document_unique_t      = std::unique_ptr< htl::docs::IDocument >;

//------------------------------------------------------------------------------
template
<
    typename      DocumentChildT_,
    typename  ... Args_
>
/**
 * @brief MakeIDocumentShared
 * @param params
 * @return
 */
document_shared_t MakeIDocumentShared( Args_ && ... params )
{
  static_assert ( std::is_base_of<IDocument, DocumentChildT_>::value,
                  ST_ASSERT_BASE_CLASS_VALIDATOR );
  return
      std::dynamic_pointer_cast<htl::docs::IDocument>(
        std::make_shared< DocumentChildT_ >(
          std::forward<Args_&&>(params)...));
}

template< typename DocumentT_ >
std::shared_ptr<DocumentT_> IDocumentSharedCast( const document_shared_t & ptr )
{
  static_assert ( std::is_base_of<IDocument, DocumentT_>::value,
                  ST_ASSERT_BASE_CLASS_VALIDATOR );
  return
      std::dynamic_pointer_cast<DocumentT_ >( ptr );
}

template
<
    typename      DocumentChildT_,
    typename  ... Args_
>
/**
 * @brief MakeIDocumentUnique
 * @param params
 * @return
 */
document_unique_t   MakeIDocumentUnique( Args_ && ... params )
{
  static_assert ( std::is_base_of<IDocument, DocumentChildT_>::value,
                  ST_ASSERT_BASE_CLASS_VALIDATOR );
  return
      std::dynamic_pointer_cast<htl::docs::IDocument>(
        std::make_unique< DocumentChildT_ >(
          std::forward<Args_&&>(params)...));
}

//------------------------------------------------------------------------------

} //namespace                     docs
} //namespace                     htl

#endif // IDOCUMENT_H
