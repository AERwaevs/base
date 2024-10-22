#pragma once

#include <concepts>
#include <memory>
namespace aer
{

template< typename P >
concept pointer = requires( P ptr )
{
    std::is_pointer_v<P>;
    { P::element_type };
    { ptr.operator *() };
    { ptr.operator ->() };
};

template< typename P >
concept shared_pointer = pointer<P> && std::copyable<P>;

template< typename P >
concept owned_pointer = pointer<P> && std::movable<P> && !std::copyable<P>;

template< typename P, typename T >
concept pointer_to = pointer<P> && std::same_as<typename P::element_type, T>;

} // namespace aer
