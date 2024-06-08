/*
 * arpt (ARPT)
 * Copyright (C) Yeong-won Seo  2024
 *
 * This file is part of ARPT
 *
 * ARPT is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3, or (at your option) any later version.
 *
 * ARPT is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARPT; see the file LICENSE.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PCH_H
#define PCH_H

#include <array>
#include <atomic>
#include <cstdint>
#include <chrono>
#include <csignal>
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
#include <conio.h>

#elif __unix__

#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <termios.h>
#include <sys/ioctl.h>

#if __linux__
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>
#include <linux/rtnetlink.h>
#else
#include <net/if_dl.h> // sockaddr_dl
#include <netlink/netlink.h>
#include <netlink/netlink_route.h>
#endif

#endif

#define __packed [[gnu::packed, gnu::aligned(1)]]

#endif //PCH_H
