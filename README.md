# ARPT : ARP toolkit

A cross-platform ARP toolkit written with modern C++.

## Supported Platforms

| OS      | Minimum Required Version           |
|:--------|:-----------------------------------|
| Windows | Vista (`GetAdaptersAddresses`)     |
| Linux   | 2.2 (`netlink`)                    |
| FreeBSD | 13.2 (`netlink`)                   |
| Android | API level 24 (`getifaddrs`)        |

## Compile Requisites

- CMake 3.27 or later
- Clang 19 or later
- *Android*: NDK version 27 or later

## How to build?

```shell
$ cmake -B build -G ninja
$ cd build
$ ninja
```

For Android, CMake toolchain files are provided.
To use these toolchain files:

```shell
$ cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/Android.<archtechture>.cmake
$ cd build
$ make -j
```

## CMake Variables

- `USE_NETLINK:BOOL` : Whether Netlink should be used as default gateway resolver (only *nix)
  - Default is `OFF` on Android, otherwise `ON`
