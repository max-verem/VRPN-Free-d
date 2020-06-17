#include <iostream>
#include <string>
#include <quat.h>
#include <vrpn_Connection.h>
#include "vrpn_Server_FreeD.h"

#define WS_VER_MAJOR 2
#define WS_VER_MINOR 2
//#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib, "quatlib.lib")
#pragma comment(lib, "vrpn.lib")

#include "FreeD.h"

vrpn_Server_FreeD::vrpn_Server_FreeD(int vrpn_listen, int free_d_listen): s(0), connection(NULL), anl(NULL), trk(NULL)
{
    int r;

    // init winsock
    WSADATA wsaData;
    r = WSAStartup
    (
        ((unsigned long)WS_VER_MAJOR) |
        (((unsigned long)WS_VER_MINOR) << 8),
        &wsaData
    );
    if (r)
        return;
    else
    {
        // Init udp listener
        struct sockaddr_in s_local;

        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (INVALID_SOCKET == s)
        {
            std::cerr << "Error! socket() failed" << std::endl;
            return;
        }

        /* socket addr struct */
        s_local.sin_family = AF_INET;
        s_local.sin_addr.s_addr = htonl(INADDR_ANY);				// any interface
        s_local.sin_port = htons((unsigned short)free_d_listen);	// port defined

        /* try to bind addr struct to socket */
        if (SOCKET_ERROR == bind(s, (struct sockaddr*)&s_local, sizeof(struct sockaddr_in)))
        {
            std::cerr << "Error! bind() failed" << std::endl;
            closesocket(s);
            return;
        };
    }

    // Initialize VRPN Connection
    std::string connectionName = ":" + std::to_string(vrpn_listen);
    connection = vrpn_create_server_connection(connectionName.c_str());

    // Init devices
    anl = new vrpn_Analog_FreeD("FreeD", connection);
    trk = new vrpn_Tracker_FreeD("FreeD", connection);
}


vrpn_Server_FreeD::~vrpn_Server_FreeD() {
    if (anl)
        delete anl;
    if (trk)
        delete trk;

    if (s)
        closesocket(s);

    if (connection) {
        connection->removeReference();
        connection = NULL;
    }
}

void vrpn_Server_FreeD::mainloop()
{
    int l, r;
    FreeD_D1_t d1;
    unsigned char buf[1024];
    struct sockaddr_in s_remote;

    // Send and receive all messages.
    connection->mainloop();

    /* clear buffer */
    memset(buf, 0, sizeof(buf));

    /* wait for packet */
    l = sizeof(s_remote);
    r = recvfrom(s, (char*)buf, sizeof(buf), 0, (struct sockaddr*)&s_remote, &l);
    if (SOCKET_ERROR == r)
    {
        std::cerr << "Error! recvfrom() failed" << std::endl;
        return;
    }

    /* parse buffer */
    r = FreeD_D1_unpack(buf, l = r, &d1);
    if (r)
    {
        std::cerr << "Error! FreeD_D2_parse() failed" << std::endl;
        return;
    }

    std::string pos_msg;
    {
        char pos_msg_buf[128];
        const char *syms_array = "+|/-\\";
        static int syms_idx = 0;
        snprintf(pos_msg_buf, sizeof(pos_msg_buf), " [%c] ID=%.3d|Pan=%9.4f|Tilt=%9.4f|Roll=%9.4f|X=%9.4f|Y=%9.4f|Z=%9.4f|Zoom=%5d|Focus=%5d\r",
            syms_array[++syms_idx % 5], d1.ID, d1.Pan, d1.Tilt, d1.Roll, d1.X, d1.Y, d1.Z, d1.Zoom, d1.Focus);
        pos_msg = pos_msg_buf;
    }
    std::cerr << pos_msg;


    // Send and receive all messages.
    connection->mainloop();
    trk->update(&d1);
    trk->mainloop();
    anl->update(&d1);
    anl->mainloop();

    // Bail if the connection is in trouble.
    if (!connection->doing_okay()) {
        std::cerr << "Connection is not doing ok. Should we bail?" << std::endl;
    }
}

