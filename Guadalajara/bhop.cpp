#include "bhop.h"
#include "Menu.h"
#include <iostream>
#include "Offsets.h"
#include <Windows.h>
#include "Utils.h"

void bhop() {
	if (menu::isBhop) {

		//std::cout << "Add" << std::hex << (g_clientModule + hazedumper::signatures::dwForceJump) << "\n";
		//std::cout << "\n";
		//std::cout << std::hex << utils::FindPattern("client.dll", "8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02");

		if (g_localplayer != nullptr) {
			if (GetAsyncKeyState(VK_SPACE) && g_localplayer->m_fFlags == 257) {
				//(DWORD)0x3935ce94
				
				//std::cout << (g_clientModule + hazedumper::signatures::dwForceJump) << "\n";
				//std::cout << utils::FindPattern("client.dll", "8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02") << "\n";

				
				//if (g_clientModule != NULL) {
					*(DWORD*)(g_clientModule + hazedumper::signatures::dwForceJump) = 6;
				//}
			}
		}
	}
}
