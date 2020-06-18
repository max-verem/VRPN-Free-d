#include "vrpn_Tracker_FreeD.h"
#include <quat.h>
#include <iostream>

vrpn_Tracker_FreeD::vrpn_Tracker_FreeD(const std::string& name, vrpn_Connection* connection/*, vr::IVRSystem * vr*/) :
	vrpn_Tracker(name.c_str(), connection)
{
	// Initialize the vrpn_Tracker
    // We track each device separately so this will only ever have one sensor
	vrpn_Tracker::num_sensors = 1;
}

#ifndef PI
#define PI 3.1415926
#endif

void vrpn_Tracker_FreeD::update(FreeD_D1_t* src)
{
    // position
    vrpn_Tracker::pos[0] = src->X;
    vrpn_Tracker::pos[1] = src->Y;
    vrpn_Tracker::pos[2] = src->Z;

    // rotation
    q_from_euler(d_quat,
        PI * src->Pan / 180.0,      // double yaw,
        PI * src->Roll / 180.0,      // double roll
        PI * src->Tilt / 180.0     // double pitch,
        );

    // Pack message
    vrpn_gettimeofday(&timestamp, NULL);
    char msgbuf[1000];
    vrpn_int32 len = vrpn_Tracker::encode_to(msgbuf);
    if (d_connection->pack_message(len, timestamp, position_m_id, d_sender_id, msgbuf, vrpn_CONNECTION_LOW_LATENCY)) {
        std::cerr << " Can't write message";
    };
};

void vrpn_Tracker_FreeD::mainloop() {
    vrpn_gettimeofday( &(vrpn_Tracker_FreeD::timestamp), NULL );
	vrpn_Tracker::server_mainloop();
}
