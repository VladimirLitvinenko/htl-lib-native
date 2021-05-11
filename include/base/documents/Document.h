#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <memory>

#include "HtlGlobal"
#include "IDocument.h"
#include "ErrLiterals.h"

namespace                       htl   {
namespace                       docs  {

//------------------------------------------------------------------------------
template< typename              ContentT_ >
/**
 * @brief Шаблонный класс Document
 */
class COMMON_SHARED_EXPORT      Document :
public                          htl::docs::IDocument
{
  /*
   * Тип ContentT_ должен соответствовать условиям:
   * 1. Не быть типом void
   * 2. Содержать оператор копирования.
   * 3. Содержать оператор перемещения (хотя бы пустой ( [[optional]] ) ).
   */
  static_assert ( ( not std::is_void< typename std::decay<ContentT_>::type >::value )
                  and
                  (
                  ( std::is_copy_assignable< typename std::decay<ContentT_>::type >::value )
                  or
                  ( std::is_move_assignable< typename std::decay<ContentT_>::type >::value )
                  ) ,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using   base_class_t        = htl::docs::IDocument;
  using   element_t           =  typename std::decay<ContentT_>::type ;

protected:
  mutable element_t   m_content;

public:
                      Document();
                      Document( doc_id_t identificator );
                      Document( const Document & document );
                      Document( Document && document );
                      Document( const Document::element_t & cnt );
                      Document( doc_id_t identificator, const Document::element_t & cnt );
                      Document( Document::element_t && cnt );
                      Document( doc_id_t identificator, Document::element_t && cnt );
  virtual           ~ Document(){}

  typename Document::element_t    Content   () const { return m_content; }
  typename Document::element_t  & ContentRef()       { return std::ref( m_content ); }

  virtual void        SetContent ( const typename Document::element_t & content ) noexcept;
  virtual void        MoveContent( typename Document::element_t && content ) noexcept;

  virtual Document  & operator ()();
  virtual Document  & operator ()( const typename Document::element_t & content );

  virtual Document  & operator =  ( const typename Document::element_t & content ) noexcept;
  virtual Document  & operator =  ( const Document & document ) noexcept;
  virtual Document  & operator =  ( Document && document ) noexcept;
  virtual Document  & operator =  ( typename Document::element_t && content ) noexcept;
};

//------------------------------------------------------------------------------
template< typename ContentT_ >
/**
 * @brief Document<ContentT_>::Document
 */
Document< ContentT_ >::Document()
  : Document< ContentT_ >::base_class_t()
{}

template< typename ContentT_ >
/**
 * @brief Document<ContentT_>::Document
 * @param identificator
 */
Document< ContentT_ >::Document( doc_id_t identificator )
: Document< ContentT_ >::base_class_t( identificator )
{}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::Document
 * @param document
 */
Document< ContentT_ >::Document( Document && document )
{
  *this = std::move( document );
}

template< typename ContentT_>
/**
 * @brief Document<ContentT_>::Document
 * @param document
 */
Document< ContentT_ >::Document( const Document<ContentT_> & document )
{
  * this = document;
}

template< typename ContentT_ >
/**
 * @brief Document<ContentT_>::Document
 * @param cnt
 */
Document< ContentT_ >::Document( const Document<ContentT_>::element_t & cnt )
  : Document< ContentT_ >::base_class_t()
{
  SetContent ( cnt );
}

template<typename ContentT_>
Document< ContentT_ >::Document(doc_id_t identificator, const Document<ContentT_>::element_t & cnt)
  : Document< ContentT_ >::base_class_t( identificator )
{
  SetContent ( cnt );
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::Document
 * @param identificator
 * @param cnt
 */
Document< ContentT_ >::Document(doc_id_t identificator, Document< ContentT_ >::element_t && cnt)
  : Document< ContentT_ >::base_class_t( identificator )
{
  MoveContent ( cnt );
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::Document
 * @param cnt
 */
Document< ContentT_ >::Document( Document< ContentT_ >::element_t && cnt )
  : Document< ContentT_ >::base_class_t()
{
  MoveContent ( cnt );
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::SetContent
 * @param content
 */
void
Document< ContentT_ >::SetContent( const Document< ContentT_ >::element_t & content )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_content = std::forward< const typename Document< ContentT_ >::element_t & >( content );
  END_LOCK_SECTION_
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::MoveContent
 * @param content
 */
void
Document< ContentT_ >::MoveContent( typename Document< ContentT_ >::element_t && content )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_content = std::move( std::forward< typename Document< ContentT_ >::element_t && >( content ) );
  END_LOCK_SECTION_
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::operator ()
 * @return
 */
Document< ContentT_ > &
Document< ContentT_ >::operator ()()
{
  return std::ref( * this );
}

template<typename ContentT_>
/**
 * @brief Document<ContentT_>::operator ()
 * @param content
 * @return
 */
Document< ContentT_ > &
Document< ContentT_ >::operator ()( const typename Document< ContentT_ >::element_t& content )
{
  SetContent ( content );
  return std::ref( * this );
}

template<typename ContentT_>
Document< ContentT_ > &
Document< ContentT_ >::operator =( const typename Document< ContentT_ >::element_t & content)
noexcept
{
  return this->operator() ( content );
}

template<typename ContentT_>
Document< ContentT_ > &
Document< ContentT_ >::operator =( const Document< ContentT_ > & document)
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  Document< ContentT_ >::base_class_t::operator =( static_cast< const IDocument & >( document ) );
  SetContent( const_cast<Document &>(document).ContentRef() );
  return std::ref( * this );
  END_LOCK_SECTION_
}

template<typename ContentT_>
Document< ContentT_ > &
Document< ContentT_ >::operator =( Document< ContentT_ > && document )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  {
    Document< ContentT_ >::base_class_t::operator =(
          std::move(
            static_cast< Document< ContentT_ >::base_class_t&& >(
              std::move( document ) ) ) );
    MoveContent( std::move( document.m_content ) );
    document.GetIDRef() = htl::docs::DOC_UNDEFINED_ID;
  }
  END_LOCK_SECTION_
  return std::ref( * this );
}

template<typename ContentT_>
Document< ContentT_ > &
Document< ContentT_ >::operator =( typename Document< ContentT_ >::element_t && content )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  MoveContent ( std::move( content ) );
  return std::ref( * this );
  END_LOCK_SECTION_
}

//------------------------------------------------------------------------------

}// namespace                   docs
}// namespace                   htl

#endif // DOCUMENT_H

