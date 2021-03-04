#include "InputSystem.h"

void IInputSystem::DisableAllInput(bool disable)
{
	static void* mouseAddress = *(void**)((char*)this + *(int*)((*(char***)this)[10] + 5));
	*(void**)((char*)this + *(int*)((*(char***)this)[10] + 5)) = disable ? 0 : mouseAddress;
}

void IInputSystem::EnableInput(bool bEnable)
{
	typedef void(__thiscall* OriginalFn)(void*, bool);
	return callvfunc<OriginalFn>(this, 11)(this, bEnable);
}

void IInputSystem::ResetInputState()
{
	typedef void(__thiscall* OriginalFn)(void*);
	return callvfunc<OriginalFn>(this, 39)(this);
}

void IInputSystem::GetCursorPosition(int* m_pX, int* m_pY)
{
	typedef void(__thiscall* OriginalFn)(void*, int*, int*);
	return callvfunc<OriginalFn>(this, 56)(this, m_pX, m_pY);
}