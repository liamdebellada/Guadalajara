#pragma once
class IInputSystem
{
	template<typename FuncType>
	inline FuncType callvfunc(void* ppClass, unsigned int index) {
		unsigned int* pVTable = *(unsigned int**)ppClass;
		unsigned int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}

public:
	void DisableAllInput(bool bEnable);

	void EnableInput(bool bEnable);

	void ResetInputState();

	//bool IsButtonDown(ButtonCode_t code);
	void val();


	void GetCursorPosition(int* m_pX, int* m_pY);

	//const char* ButtonCodeToString(ButtonCode_t ButtonCode);
	void val1();

	//ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey);
	void va2();

	//int ButtonCodeToVirtualKey(ButtonCode_t code);
	void val3();
};