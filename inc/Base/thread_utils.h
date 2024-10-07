#pragma once

#include <utility>
#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include "log.h"

namespace aer { namespace utils
{   

static size_t num_threads()
{
    static size_t num_threads = []() -> size_t
    {
        if( const auto num = std::getenv("NUM_THREADS") ) return std::stoi( num ) + 1;
        else return std::thread::hardware_concurrency() + 1;
    }();
    return num_threads;
}

struct thread_id_t
{
    static std::vector<std::atomic_bool> in_use;

    const int id;
    const int operator ()() { return id; }

    thread_id_t() :
    id([]{
        for( int i = 0; i < num_threads(); i++ )
        {
            bool expected = false;
            if( !in_use[i] && in_use[i].compare_exchange_strong( expected, true ) ) return i;
        }
        AE_FATAL( "More than %d threads created", num_threads() );
    }()){}

    ~thread_id_t() { in_use[id].store( false ); }
};

std::vector<std::atomic_bool>   thread_id_t::in_use( num_threads() );
const thread_local thread_id_t  thread_id{};

} } // namespace aer::utils