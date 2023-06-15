#pragma once

namespace aer
{

template< typename T, template<typename> typename... interfaces >
struct Interfaces : public virtual interfaces<T>...
{

};

}