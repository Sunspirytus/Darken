#pragma once
#include "TypeDefine.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

class InputManager
{
public:
	~InputManager();

	static InputManager& getInstance()
	{
		static InputManager m_instance;
		return m_instance;
	}

	void CheckInput();

private:

};