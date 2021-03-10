#include "hooks.h"

Hooks::CreateMoveFn originalFn;

bool __stdcall hkCreateMove(float sampleTime, CUserCmd* cmd)
{
    bhop();
    originalFn(sampleTime, cmd);
    if (menu::breakAngles) {
        cmd->viewangles.y = (float)(rand() % 359);
        return true;
    }
    else {
        return true;
    }
}

void Hooks::init() {
    uintptr_t createMoveAddress = (uintptr_t)((*(void***)Interfaces->ClientMode)[24]);
    originalFn = (Hooks::CreateMoveFn)DetourFunction((PBYTE)createMoveAddress, (PBYTE)hkCreateMove);
}

