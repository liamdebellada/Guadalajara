#include "InterfaceDefinitions.h"
#include <Windows.h>
#include <iostream>

InterfaceCollection* Interfaces = nullptr;

typedef void* (*tCreateInterface)(const char* name, int* returnCode);
void* GetInterface(const char* dllName, const char* interfaceName) {
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllName), "CreateInterface");
	int returnCode = 0;
	void* Interface = CreateInterface(interfaceName, &returnCode);
	return Interface;
}

InterfaceCollection::InterfaceCollection() {
	ClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
	EngineClient = (IEngineClient*)GetInterface("engine.dll", "VEngineClient014");
	InputSystem = (IInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion001");
	Client = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
	ClientMode = **(IClientMode***)((*(DWORD**)Client)[10] + 0x5);
}