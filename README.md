# ARPT : ARP toolkit

A cross-platform ARP toolkit written with modern C++.

## Requisites

### Windows

- Windows Vista or above (`GetAdaptersAddresses`)

### Unix-like

- Clang 17 or above
- Linux
  - Linux 2.2 or above (`netlink`)
- FreeBSD
  - FreeBSD 13.2 or above (`netlink`)
- Android
  - API level 24 or above (`getifaddrs`, `freeifaddrs`)
  - NDK version 27 or above (`std::format`)
