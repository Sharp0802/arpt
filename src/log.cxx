#include "module.h"

namespace arpt
{
    auto getThreadId()
    {
        static std::atomic_uint8_t count{0};
        thread_local uint8_t id = count.fetch_add(1);
        return id;
    }

    auto getHeader()
    {
        auto now = std::chrono::system_clock::now();

        return std::format(
            "[{:03d} {:%FT%T}]",
            getThreadId(),
            now);
    }

    std::ostream& errs()
    {
        return std::cerr << "\x1b[31mERR " << getHeader() << ' ';
    }

    std::ostream& outs()
    {
        return std::cout << "--- " << getHeader() << ' ';
    }

    std::ostream& endl(std::ostream& os)
    {
        os << "\x1b[m";
        return std::flush(std::endl(os));
    }
}
