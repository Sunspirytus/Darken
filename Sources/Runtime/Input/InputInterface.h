#pragma once
#include "InputBase.h"

class InputInterface : private InputManager
{
public:
	InputInterface();
	~InputInterface();

	virtual void OnKeyPress()

private:

};

InputInterface::InputInterface()
{
}

InputInterface::~InputInterface()
{
}
