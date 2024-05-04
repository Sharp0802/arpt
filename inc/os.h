#ifndef ARPT_OS_H
#define ARPT_OS_H

namespace arpt
{
    enum OS
    {
        POSIX,
        Windows,

#ifdef __unix__
        Current = POSIX
#else
        Current = Windows
#endif
    };
}

#endif //ARPT_OS_H
