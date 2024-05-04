#ifndef PCH_H
#define PCH_H

#include <array>
#include <atomic>
#include <cstdint>
#include <chrono>
#include <cstring>
#include <format>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <ostream>
#include <optional>
#include <ranges>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#if _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <ipmib.h>
#include <ws2tcpip.h>

#elif __unix__

#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <sys/types.h>
#include <ifaddrs.h>

#if __linux__
#include <linux/if_packet.h> // sockaddr_ll
#include <linux/rtnetlink.h>
#else
#include <net/if_dl.h> // sockaddr_dl
#include <netlink/netlink.h>
#include <netlink/netlink_route.h>
#endif

#endif

#endif //PCH_H
