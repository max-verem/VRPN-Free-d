#pragma once

#include <string>
#include <vrpn_Tracker.h>
#include <quat.h>
#include "FreeD.h"

class vrpn_Tracker_FreeD :
    public vrpn_Tracker
{
public:
    vrpn_Tracker_FreeD(const std::string& name, vrpn_Connection* connection);
    void mainloop();
    void update(FreeD_D1_t* src);
protected:
    //vr::IVRSystem * vr;
private:
};

