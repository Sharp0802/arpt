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

- `NPCAP_VERSION:VERSION` : NPcap version to use (only Windows)
  - Default is `1.13`
- `USE_NETLINK:BOOL` : Whether Netlink should be used as default gateway resolver (only *nix)
  - Default is 'OFF' on Android, otherwise 'ON'
