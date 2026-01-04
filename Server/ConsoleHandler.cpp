#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <iostream>
#include "ServerCore.h"
#include "ConsoleHandler.h"

static Server* g_server = nullptr;

void RegisterConsoleHandler(Server* server) {
    g_server = server;
    SetConsoleCtrlHandler([](DWORD ctrlType) -> BOOL {
        if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_CLOSE_EVENT) {
            std::cout << "\n[INFO] Ctrl+C detected, shutting down...\n";
            if (g_server) g_server->stop();
            return TRUE;
        }
        return FALSE;
        }, TRUE);
}