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

#pragma once

#include "endians.h"
#include "mac.h"

namespace arpt
{
    enum class EthernetProtocol
    {
        LOOP       = 0x0060,
        PUP        = 0x0200,
        PUPAT      = 0x0201,
        TSN        = 0x22F0,
        ERSPAN2    = 0x22EB,
        IP         = 0x0800,
        X25        = 0x0805,
        ARP        = 0x0806,
        BPQ        = 0x08FF,
        IEEEPUP    = 0x0a00,
        IEEEPUPAT  = 0x0a01,
        BATMAN     = 0x4305,
        DEC        = 0x6000,
        DNA_DL     = 0x6001,
        DNA_RC     = 0x6002,
        DNA_RT     = 0x6003,
        LAT        = 0x6004,
        DIAG       = 0x6005,
        CUST       = 0x6006,
        SCA        = 0x6007,
        TEB        = 0x6558,
        RARP       = 0x8035,
        ATALK      = 0x809B,
        AARP       = 0x80F3,
        _8021Q     = 0x8100,
        ERSPAN     = 0x88BE,
        IPX        = 0x8137,
        IPV6       = 0x86DD,
        PAUSE      = 0x8808,
        SLOW       = 0x8809,
        WCCP       = 0x883E,
        MPLS_UC    = 0x8847,
        MPLS_MC    = 0x8848,
        ATMMPOA    = 0x884c,
        PPP_DISC   = 0x8863,
        PPP_SES    = 0x8864,
        LINK_CTL   = 0x886c,
        ATMFATE    = 0x8884,
        PAE        = 0x888E,
        PROFINET   = 0x8892,
        REALTEK    = 0x8899,
        AOE        = 0x88A2,
        ETHERCAT   = 0x88A4,
        _8021AD    = 0x88A8,
        _802_EX1   = 0x88B5,
        PREAUTH    = 0x88C7,
        TIPC       = 0x88CA,
        LLDP       = 0x88CC,
        MRP        = 0x88E3,
        MACSEC     = 0x88E5,
        _8021AH    = 0x88E7,
        MVRP       = 0x88F5,
        _1588      = 0x88F7,
        NCSI       = 0x88F8,
        PRP        = 0x88FB,
        CFM        = 0x8902,
        FCOE       = 0x8906,
        IBOE       = 0x8915,
        TDLS       = 0x890D,
        FIP        = 0x8914,
        _80221     = 0x8917,
        HSR        = 0x892F,
        NSH        = 0x894F,
        LOOPBACK   = 0x9000,
        QINQ1      = 0x9100,
        QINQ2      = 0x9200,
        QINQ3      = 0x9300,
        EDSA       = 0xDADA,
        DSA_8021Q  = 0xDADB,
        DSA_A5PSW  = 0xE001,
        IFE        = 0xED3E,
        IUCV       = 0xFBFB,
        _802_3_MIN = 0x0600,
        _802_3     = 0x0001,
        AX25       = 0x0002,
        ALL        = 0x0003,
        _802_2     = 0x0004,
        SNAP       = 0x0005,
        DDCMP      = 0x0006,
        WAN_PPP    = 0x0007,
        PPP_MP     = 0x0008,
        LOCALTALK  = 0x0009,
        CAN        = 0x000C,
        CANFD      = 0x000D,
        CANXL      = 0x000E,
        PPPTALK    = 0x0010,
        TR_802_2   = 0x0011,
        MOBITEX    = 0x0015,
        CONTROL    = 0x0016,
        IRDA       = 0x0017,
        ECONET     = 0x0018,
        HDLC       = 0x0019,
        ARCNET     = 0x001A,
        DSA        = 0x001B,
        TRAILER    = 0x001C,
        PHONET     = 0x00F5,
        IEEE802154 = 0x00F6,
        CAIF       = 0x00F7,
        XDSA       = 0x00F8,
        MAP        = 0x00F9,
        MCTP       = 0x00FA,
    };

    class __packed Ethernet
    {
        MAC    m_Destination;
        MAC    m_Source;
        be16_t m_Protocol;

    public:
        Ethernet(MAC destination, MAC source, EthernetProtocol protocol);

        [[nodiscard]]
        MAC __GetDestination() const;

        [[nodiscard]]
        MAC __GetSource() const;

        [[nodiscard]]
        EthernetProtocol __GetProtocol() const;

        __declspec(property(get=__GetDestination))
        MAC Destination;

        __declspec(property(get=__GetSource))
        MAC Source;

        __declspec(property(get=__GetProtocol))
        EthernetProtocol Protocol;
    };
}
