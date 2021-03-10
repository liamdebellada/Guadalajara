#pragma once
#include <iostream>
#include <Windows.h>
#include "detours.h"
#include "InterfaceDefinitions.h"

#include "CUserCmd.h"

#include "Menu.h"
#include "bhop.h"

#include <cstdlib>

namespace Hooks {
	typedef bool(__stdcall* CreateMoveFn)(float, CUserCmd*);
	void init();
}
