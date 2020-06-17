#pragma once

#include <string>
#include <vrpn_Analog.h>
#include <quat.h>

#include "FreeD.h"

class vrpn_Analog_FreeD :
    public vrpn_Analog
{
public:
    vrpn_Analog_FreeD(const std::string& name, vrpn_Connection* connection/*, vr::IVRSystem * vr*/);
    void mainloop();
    void update(FreeD_D1_t* src);
protected:
    //vr::IVRSystem * vr;
private:
};

