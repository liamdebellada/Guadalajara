#pragma once
#include "Entity.h"
#include "EngineClient.h"
#include "InputSystem.h"

class InterfaceCollection {
public:
	InterfaceCollection();
public:
	IClientEntityList* ClientEntityList = nullptr;
	IEngineClient* EngineClient = nullptr;
	IInputSystem* InputSystem = nullptr;
};

extern InterfaceCollection* Interfaces;