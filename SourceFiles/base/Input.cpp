#include "Input.h"
#include "WindowsAPI.h"
#include "D3D12Common.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

Input* Input::GetInstance()
{
	static Input input;
	return &input;
}

void Input::Initialize()
{
	Result result;
	WindowsAPI* wAPI = WindowsAPI::GetInstance();

	result = DirectInput8Create(wAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	// キーボード
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = keyboard->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	// マウス
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	result = mouse->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	// ゲームパッド
	//result = directInput->CreateDevice(GUID_Joystick, &joystick, NULL);
	//result = joystick->SetDataFormat(&c_dfDIJoystick);
	//result = joystick->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	keyboard->Acquire();
	oldkey = key;
	keyboard->GetDeviceState((DWORD)key.size(), (LPVOID)key.data());

	mouse->Acquire();
	mouseStatePre = mouseState;
	mouse->GetDeviceState(sizeof(mouseState), &mouseState);

	//joystick->Acquire();
	//joyStatePre = joyState;
	//joystick->GetDeviceState(sizeof(joyState), &joyState);

}

bool Input::IsTriggerMouse(Mouse KEY)
{
	return !mouseStatePre.rgbButtons[(int)KEY] && mouseState.rgbButtons[(int)KEY]; 
}

Input::MouseMove Input::GetMouseMove() 
{
	MouseMove tmp{};
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

float Input::Move(Key KEY1, Key KEY2, const float spd) { return (IsInput(KEY1) - IsInput(KEY2)) * spd; }