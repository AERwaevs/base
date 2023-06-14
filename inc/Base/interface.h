#pragma once

#ifdef interface
#undef interface
#endif
#define interface template<typename> typename

namespace aer
{

template< typename T, interface... interfaces >
struct Interfaces : public virtual interfaces<T>...
{

};

}