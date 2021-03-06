#include "InterfaceDefinitions.h"
#include <Windows.h>

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
}