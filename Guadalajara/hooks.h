#pragma once
#include <iostream>
#include <Windows.h>
#include "detours.h"
#include "InterfaceDefinitions.h"

#include "CUserCmd.h"
#include "Vector.h"
#include "QAngle.h"


#include "Menu.h"
#include "bhop.h"

#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
extern int counter;
namespace Hooks {
	typedef bool(__stdcall* CreateMoveFn)(float, CUserCmd*);
	void init();
}

