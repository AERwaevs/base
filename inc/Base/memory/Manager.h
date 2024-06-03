#pragma once

#include <memory>

namespace aer
{
namespace mem
{

class Manager
{
public:
    Manager();
    static std::unique_ptr<Manager>& instance() noexcept;
    
    template< typename T >
    bool ref( const Object*, std::memory_order = std::memory_order_relaxed ) noexcept;
    template< typename T >
    bool unref( const Object*, std::memory_order = std::memory_order_seq_cst ) noexcept;
}

template< typename T >
static inline bool ref( const T const* obj, std::memory_order order = std::memory_order_relaxed ) noexcept
{
    return Manager::instance()->ref( obj );
}

template< typename T >
static inline bool unref( const T const* obj, std::memory_order order = std::memory_order_seq_cst ) noexcept
{
    return Manager::instance()->unref( obj );
}

} // namespace aer::mem
} // namespace aer
