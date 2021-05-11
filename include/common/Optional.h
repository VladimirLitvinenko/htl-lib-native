#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <bits/c++config.h>

#if __cplusplus >= 201703L

#include <optional>
namespace htl   {
    template< typename T_ >
    using optional = std::optional< T_ >;
}// namespace                     htl

#else

#include <experimental/optional>
namespace htl   {
    template< typename T_ >
    using optional = std::experimental::optional<T_>;
}// namespace                     htl
#endif

#endif // OPTIONAL_H
