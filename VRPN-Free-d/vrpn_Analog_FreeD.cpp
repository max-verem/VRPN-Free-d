#include "vrpn_Analog_FreeD.h"
#include <quat.h>
#include <iostream>

vrpn_Analog_FreeD::vrpn_Analog_FreeD(const std::string& name, vrpn_Connection* connection/*, vr::IVRSystem * vr*/) :
    vrpn_Analog(name.c_str(), connection)
{
    vrpn_Analog::num_channel = 2;
}

void vrpn_Analog_FreeD::update(FreeD_D1_t* src)
{
    vrpn_Analog::channel[0] = src->Zoom;
    vrpn_Analog::channel[1] = src->Focus;
};

void vrpn_Analog_FreeD::mainloop() {
    vrpn_gettimeofday(&(vrpn_Analog::timestamp), NULL);
    vrpn_Analog::report_changes();
}