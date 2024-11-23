#pragma once

#include <memory>
#include <type_traits>

#define BASE_PTR_VERSION 1

namespace aer
{
template< typename T >
concept pointer = std::is_pointer_v<T>;

template< typename T, typename To >
concept pointer_to = pointer<T> && std::convertible_to<T, std::add_pointer_t<To>>;

template< typename T >
concept dereferenceable = requires( T* ptr )
{
    { *ptr } -> std::convertible_to<std::add_lvalue_reference_t<std::remove_pointer_t<T>>>;
};

template< typename T >
concept protected_pointer = pointer<T> && !dereferenceable<T>;

namespace mem
{
    
#if BASE_PTR_VERSION == 1

template< typename T >
struct base_ptr
{
    using type = std::remove_extent_t<T>;

             base_ptr()                      noexcept : ptr( nullptr ) {}
             base_ptr( std::nullptr_t )      noexcept : ptr( nullptr ) {}
             base_ptr( const base_ptr& rhs ) noexcept : ptr( rhs.ptr ) {}
    explicit base_ptr( type* rhs )           noexcept : ptr( rhs )     {}
            ~base_ptr()                                                {}
    
    void     operator []    ( int ) const = delete;
    auto     operator *     ( this auto& self ) noexcept { return *self.ptr; }
    auto     operator ->    ( this auto& self ) noexcept { return self.ptr; }
    explicit operator bool  ( this auto& self ) noexcept { return self.valid(); }
    auto     operator <=>   ( const base_ptr& ) const = default;
    auto     operator <=>   ( const pointer auto rhs ) const noexcept { return (ptr <=> rhs); };
    auto     operator <=>   ( const std::derived_from<base_ptr> auto rhs ) const noexcept { return (ptr <=> rhs.ptr); };

    auto     get()                              const noexcept { return ptr; }
    bool     valid()                            const noexcept { return (ptr != nullptr); }
    void     swap( base_ptr& other )            const noexcept { std::swap( ptr, other.ptr ); }

protected:
    std::add_pointer_t<type> ptr = nullptr;
};

#elif BASE_PTR_VERSION == 2

template< typename T >
struct base_ptr
{
    using element_t = std::remove_extent_t<T>;
    using pointer_t = std::add_pointer_t<T>;

    //template< typename... Args > requires( std::constructible_from<T, Args...> )
    //explicit    base_ptr( Args&&... args )      noexcept { new( &ptr ) T( std::forward<Args>( args )... ); }
    explicit    base_ptr( pointer_t rhs )       noexcept : ptr( rhs )     {}
                base_ptr( const base_ptr& rhs ) noexcept : ptr( rhs.ptr ) {}
                base_ptr( base_ptr&& rhs )      noexcept : ptr( rhs.ptr ) { rhs.ptr = nullptr; }
                base_ptr()                      noexcept : ptr( nullptr ) {}
                ~base_ptr() = default;
  
    inline void swap( base_ptr& other ) const noexcept { std::swap( ptr, other.ptr ); }
    inline void clear()                       noexcept { ptr = nullptr; }
    inline bool valid()                 const noexcept { return ( ptr != nullptr ); }
    operator    bool()                  const noexcept { return valid(); }

    template< typename Self > auto get          ( this Self&& s ) noexcept { return s.valid() ?  s.ptr   : nullptr; }
    template< typename Self > auto operator ->  ( this Self&& s ) noexcept { return s.valid() ?  s.get() : nullptr; }
    template< typename Self > auto operator *   ( this Self&& s ) noexcept { return s.valid() ? *s.get() : nullptr; }
    template< typename R >    auto operator <=> ( const R* rhs )    const noexcept { return ( ptr <=> rhs ); };
                              auto operator <=> ( const base_ptr& ) const = default;
                              void operator []  ( int )             const = delete;
protected:
    pointer_t   ptr = nullptr;
};

#endif

} // namespace aer::mem
} // namespace aer
