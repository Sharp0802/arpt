/*
 * lilac (LILAC)
 * Copyright (C) Yeong-won Seo  2024
 *
 * This file is part of LILAC
 *
 * LILAC is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3, or (at your option) any later
 * version.
 *
 * LILAC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LILAC; see the file LICENSE.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include "module.h"

using Subcommand = int(std::vector<std::string>);

int Help()
{
    std::cout << "arpt (ARPT) " VERSION " " TIMESTAMP R"(
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBCOMMAND

  list      List network-interface informations
  block     Attempts to block specific users to use network by poisioning
            ARP table
  help      Prints this message

)";
    return 0;
}

int ListDevs()
{
    std::stringstream ss;
    ss << "\n                                  NAME LINK                           IP/MASK GATEWAY         BROADCAST";
    for (const auto& dev: arpt::QueryNetworkInterfaceList({ true })->Get())
    {
        ss << std::format("\n{:>38} {:17} {:>15}/{:<3}  {:15} {:15}",
            dev->Name,
            dev->Link.ToString(),
            dev->Address.ToString(),
            dev->Mask,
            dev->Gateway.has_value() ? dev->Gateway.value().ToString() : "",
            dev->Broadcast.has_value() ? dev->Broadcast.value().ToString() : "");
    }
    arpt::outs() << ss.str() << arpt::endl;

    return 0;
}

int main(int argc, char* argv[])
{
#if _WIN32
    /*********************************
     * Enable SGR terminal sequences *
     *********************************/
    {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;

        if (out == INVALID_HANDLE_VALUE)
            goto CONSOLE_INIT_ERROR;
        if (!GetConsoleMode(out, &mode))
            goto CONSOLE_INIT_ERROR;
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(out, mode))
            goto CONSOLE_INIT_ERROR;

        if (false)
        {
            CONSOLE_INIT_ERROR:
            arpt::errs() << std::format(
                "Couldn't enable virtual sequence: {}",
                std::system_category().message(static_cast<int>(GetLastError()))) << arpt::endl;
        }
    }
#endif

    static std::map<std::string, std::function<Subcommand>> s_Handler = {
        {
            "help", [](auto)
            {
                Help();
                return 0;
            }
        },
        {
            "list", [](auto)
            {
                ListDevs();
                return 0;
            }
        }
    };

    if (argc < 2)
    {
        arpt::errs() << "subcommand is missing" << arpt::endl;
        Help();
        return -1;
    }

    if (!s_Handler.contains(argv[1]))
    {
        arpt::errs() << "undefined subcommand '" << argv[1] << "'." << arpt::endl;
        Help();
        return -1;
    }

    std::vector<std::string> args;
    args.reserve(argc - 2);
    for (auto i = 2; i < argc; ++i)
        args.emplace_back(argv[i]);
    return s_Handler[argv[1]](args);
}
