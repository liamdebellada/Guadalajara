#include <Windows.h>
#include <windowsx.h>
#include "detours.h"
#include "PatternScanning.h"
#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

#include "offsets.h"
#include "Menu.h"
#include "Entity.h"
#include "EngineClient.h"
#include "InputSystem.h"
#include "InterfaceDefinitions.h"
#include "bhop.h"

extern Ent* g_localplayer;
extern DWORD g_clientModule;