#include "bhop.h"
#include "Menu.h"
#include <iostream>
#include "Offsets.h"
#include <Windows.h>
#include "Utils.h"

void bhop() {
	if (menu::isBhop) {
		if (GetAsyncKeyState(VK_SPACE) && g_localplayer->m_fFlags == 257) {
			*(DWORD*)(g_clientModule + hazedumper::signatures::dwForceJump) = 6;
		}
	}
}
