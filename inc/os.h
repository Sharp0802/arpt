#ifndef OS_H
#define OS_H

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

#endif //OS_H
