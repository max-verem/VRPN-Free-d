#pragma once

#include <map>
#include <array>
#include <memory>

#include <quat.h>
#include <vrpn_Connection.h>


class vrpn_Server_FreeD {
public:
    vrpn_Server_FreeD();
	~vrpn_Server_FreeD();
	void mainloop();

private:
	std::unique_ptr<vr::IVRSystem> vr{ nullptr };
	vrpn_Connection *connection;
};

