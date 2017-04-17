#include "InputControl.h"

#pragma region Constructors and Deconstructor
InputControl_Singleton::InputControl_Singleton()
{

}

InputControl_Singleton::~InputControl_Singleton()
{

}

#pragma endregion

#pragma region Initialize

void InputControl_Singleton::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		_keys[i] = false;
	}
	_mouseDown = false;
	_wasMouseDown = false;
}

#pragma endregion

#pragma region KeyFunctions

void InputControl_Singleton::KeyDown(unsigned int &keyNumber)
{
	_keys[keyNumber] = true;
}

void InputControl_Singleton::KeyUp(unsigned int &keyNumber)
{
	_keys[keyNumber] = false;
}

bool InputControl_Singleton::IsKeyDown(unsigned int &keyNumber)
{
	return _keys[keyNumber];
}

void InputControl_Singleton::MouseDown()
{
	if (!_wasMouseDown)
	{
		_mouseDown = true;
		_wasMouseDown = true;
	}
	else
	{
		_mouseDown = false;
	}
}

void InputControl_Singleton::MouseUp()
{
	_mouseDown = false;
	_wasMouseDown = false;
}

bool InputControl_Singleton::IsMouseDown()
{
	return _mouseDown;
}

#pragma endregion