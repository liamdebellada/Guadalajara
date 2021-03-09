#pragma once
#include "Entity.h"
#include "EngineClient.h"
#include "InputSystem.h"
#include "IBaseClient.h"
#include "IClientMode.h"

class InterfaceCollection {
public:
	InterfaceCollection();
public:
	IClientEntityList* ClientEntityList = nullptr;
	IEngineClient* EngineClient = nullptr;
	IInputSystem* InputSystem = nullptr;
	IBaseClientDLL* Client = nullptr;
	IClientMode* ClientMode = nullptr;
};

extern InterfaceCollection* Interfaces;