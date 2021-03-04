#include "Hack.h"
typedef void* (*tCreateInterface)(const char* name, int* returnCode);

void* GetInterface(const char* dllName, const char* interfaceName) {
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllName), "CreateInterface");
	int returnCode = 0;
	void* Interface = CreateInterface(interfaceName, &returnCode);
	return Interface;
}