#include <Base/memory/manager.h>

namespace aer::mem
{
std::unique_ptr<Manager>& Manager::instance() noexcept
{
    static auto manager = std::make_unique<Manager>();
    return manager;
}

} // namespace aer::mem