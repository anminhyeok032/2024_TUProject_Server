#pragma once

#include <iostream>
#include <array>
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

constexpr short PORT = 4000;
constexpr int BUFSIZE = 256;

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND };
