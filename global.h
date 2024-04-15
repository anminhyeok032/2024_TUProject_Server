#pragma once

#include <iostream>
#include <array>
#include <thread>
#include <vector>
#include <WS2tcpip.h>
#include <MSWSock.h>


#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

constexpr short PORT = 4000;
constexpr int BUFSIZE = 256;
constexpr int MAX_USER = 4;

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND };
enum class S_STATE {ST_FREE, ST_ALLOC, ST_INGAME};
