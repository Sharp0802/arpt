#ifndef LOG_H
#define LOG_H

#include <ostream>

namespace arpt
{
    std::ostream& errs();

    std::ostream& outs();

    std::ostream& endl(std::ostream& os);
}

#endif //LOG_H
