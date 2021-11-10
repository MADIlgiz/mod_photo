#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <slp/slp.h>
#include <slp/slp_link_dbc.h>
#include "retk2/core/Application.hpp"
#include "retk2/core/StringUtils.hpp"
#include "retk2/core/Exception.hpp"
#include "retk2/io/Poller.hpp"
#include "retk2/ipc/DBCClient.hpp"
#include "tools/dbc_common.hpp"
#include "retk2/proto/PacketMultiLayer.hpp"

class mod_photo_DBCclient: public slp::SLP_DBCClient {
    public:
    mod_photo_DBCclient();
    private:

};