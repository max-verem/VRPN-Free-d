#include <iostream>
#include <string>
#include <quat.h>
#include <vrpn_Connection.h>
#include "vrpn_Server_FreeD.h"

vrpn_Server_FreeD::vrpn_Server_FreeD() {
    // Initialize OpenVR

    // Initialize VRPN Connection
    std::string connectionName = ":" + std::to_string(vrpn_DEFAULT_LISTEN_PORT_NO);
    connection = vrpn_create_server_connection(connectionName.c_str());
}


vrpn_Server_FreeD::~vrpn_Server_FreeD() {
    // Initialize OpenVR
    // vr::VR_Shutdown();

    if (connection) {
        connection->removeReference();
        connection = NULL;
    }
}

void vrpn_Server_FreeD::mainloop() {

    // Send and receive all messages.
    connection->mainloop();

    // Bail if the connection is in trouble.
    if (!connection->doing_okay()) {
        std::cerr << "Connection is not doing ok. Should we bail?" << std::endl;
    }
}

