#pragma once

#pragma comment(lib, "wsock32.lib")

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <tchar.h>
#include <process.h>

#include "../raknet/RakClient.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/BitStream.h"
#include "../raknet/PacketEnumerations.h"

#include "rconclient.h"

extern bool bQuitApp;

void logprintf(char* format, ...);