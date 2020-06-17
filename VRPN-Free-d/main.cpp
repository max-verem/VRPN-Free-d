#include <windows.h>
#include <memory>
#include <iostream>
#include <string>
#include "vrpn_Server_FreeD.h"

static volatile int done = 0;
std::unique_ptr<vrpn_Server_FreeD> server{};

// install a signal handler to shut down the devices
// On Windows, the signal handler is run in a different thread from
// the main application.  We don't want to go destroying things in
// here while they are being used there, so we set a flag telling the
// main program it is time to exit.
#if defined(_WIN32) && !defined(__CYGWIN__)
/**
* Handle exiting cleanly when we get ^C or other signals.
*/
BOOL WINAPI handleConsoleSignalsWin(DWORD signaltype)
{
    switch (signaltype) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        done = 1;
        return TRUE;
        // Don't exit, but return FALSE so default handler
        // gets called. The default handler, ExitProcess, will exit.
    default:
        return FALSE;
    }
}
#endif

int main(int argc, char *argv[])
{
    int vrpn_listen = 0, free_d_listen = 0;

    if (argc != 3 && argc != 2)
    {
        std::cerr << "Error! Not enough argumements!" << std::endl;
        std::cerr << "Usage:" << std::endl;
        std::cerr << "    " << argv[0] << " [<vrpn tcp port>] <free-d udp listen port>" << std::endl;
        return 1;
    }

    if (argc == 2)
    {
        vrpn_listen = vrpn_DEFAULT_LISTEN_PORT_NO;
        free_d_listen = atoi(argv[1]);
    }
    else
    {
        vrpn_listen = atoi(argv[1]);;
        free_d_listen = atoi(argv[2]);
    }

    server = std::make_unique<vrpn_Server_FreeD>(vrpn_listen, free_d_listen);
    while (!done && server->s) {
        server->mainloop();
    }
    server.reset(nullptr);
    return 0;
}