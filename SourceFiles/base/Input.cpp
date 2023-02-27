#include "Input.h"
#include "WindowsAPI.h"
#include "D3D12Common.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <imgui.h>

Microsoft::WRL::ComPtr<IDirectInput8> Input::directInput;

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* gamePadDevice;
	int findCount;
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
#pragma region ゲームパッド
	DeviceEnumParameter parameter{};

	parameter.findCount = 0;
	parameter.gamePadDevice = &joystick;

	// GAMEPADを調べる
	directInput->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	directInput->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.findCount == 0) { return; }

	// 制御開始
	StartGamePadControl();
#pragma endregion
}

bool SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph))) { return false; }

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }

	return true;
}

int CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	IDirectInputDevice8* device = nullptr;

	// 既に発見しているなら終了
	if (parameter->findCount >= 1) { return DIENUM_STOP; }

	// デバイス生成
	HRESULT hr = directInput->CreateDevice(
		lpddi->guidInstance,
		parameter->gamePadDevice,
		NULL);

	if (FAILED(hr)) { return DIENUM_STOP; }

	// 入力フォーマットの指定
	device = *parameter->gamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr)) { return DIENUM_STOP; }

	// プロパティの設定
	if (!SetUpGamePadProperty(device)) { return DIENUM_STOP; }

	// 協調レベルの設定
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	device->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	// 発見数をカウント
	parameter->findCount++;

	return DIENUM_CONTINUE;
}

bool Input::StartGamePadControl()
{
	// デバイスが生成されてない
	if (!joystick) { return false; }

	// 制御開始
	if (FAILED(joystick->Acquire())) { return false; }

	DIDEVCAPS cap;
	joystick->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		if (FAILED(joystick->Poll())) { return false; }
	}

	return true;
}

void Input::Update()
{
	keyboard->Acquire();
	oldkey = key;
	keyboard->GetDeviceState((DWORD)key.size(), (LPVOID)key.data());

	mouse->Acquire();
	mouseStatePre = mouseState;
	mouse->GetDeviceState(sizeof(mouseState), &mouseState);

	if (!joystick) { return; }
	joyStatePre = joyState;
	joystick->GetDeviceState(sizeof(joyState), &joyState);
	
	ImGui::Text("joyState.lRx = %d", joyState.lRx);
	ImGui::Text("joyState.lRy = %d", joyState.lRy);
	ImGui::Text("joyState.lx = %d", joyState.lX);
	ImGui::Text("joyState.ly = %d", joyState.lY);
	ImGui::Text("joyState.lz = %d", joyState.lZ);
	ImGui::Text("joyState.rgdwPOV[0] = %d", joyState.rgdwPOV[0]);
	for (size_t i = 0; i < 16; i++)
	{
		ImGui::Text("joyState.rgbButtons[%d] = %d", i, joyState.rgbButtons[i]);
	}
}

bool Input::IsTrigger(Mouse KEY)
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

Input::PadState Input::GetPadState()
{
	return PadState(joyState.lX,joyState.lY, joyState.lRx, joyState.lRy, joyState.lZ, joyState.rgdwPOV[0]);
}