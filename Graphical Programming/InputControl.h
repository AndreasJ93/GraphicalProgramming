#ifndef INPUT_CONTROL_H
#define INPUT_CONTROL_H

class InputControl_Singleton
{
private:
	InputControl_Singleton();
	~InputControl_Singleton();
	InputControl_Singleton &operator=(const InputControl_Singleton &original) = delete;

	bool _keys[256];
	bool _mouseDown;
	bool _wasMouseDown;

public:
	inline static InputControl_Singleton &Instance() { static InputControl_Singleton singleton; return singleton; };

	void Initialize();
	void KeyDown(unsigned int &keyNumber);
	void KeyUp(unsigned int &keyNumber);
	void MouseDown();
	void MouseUp();
	bool IsMouseDown();

	bool IsKeyDown(unsigned int &keyNumber);

};





#endif