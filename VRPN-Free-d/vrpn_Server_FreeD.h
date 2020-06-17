#pragma once

#include <map>
#include <array>
#include <memory>

#include <quat.h>
#include <vrpn_Connection.h>

#include "vrpn_Analog_FreeD.h"
#include "vrpn_Tracker_FreeD.h"

class vrpn_Server_FreeD {
public:
    SOCKET s;
    vrpn_Server_FreeD(int _vrpn_listen, int _free_d_listen);
    ~vrpn_Server_FreeD();
    void mainloop();

private:
    vrpn_Connection *connection;
    vrpn_Analog_FreeD* anl;
    vrpn_Tracker_FreeD* trk;

};

