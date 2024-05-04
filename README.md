# ARPT : ARP toolkit

A cross-platform ARP toolkit written with modern C++.

## Supported Platforms

|       Arch | OS      | Minimum Required Version           |
|-----------:|:--------|:-----------------------------------|
| x86/x86-64 | Windows | Vista (`GetAdaptersAddresses`)     |
|      ARM64 | Windows |                                    |
|         -- | Linux   | 2.2 (`netlink`)                    |
|         -- | FreeBSD | 13.2 (`netlink`)                   |
|         -- | Android | API level 24 (`getifaddrs`)        |

## Compile Requisites

- CMake 3.27 or later
- Clang (17 or later; Cross-platform) or MSVC (latest; Windows only)
- *Android*: NDK version 27 or later (`std::format`)