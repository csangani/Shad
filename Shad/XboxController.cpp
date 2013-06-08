#include <Shad\XboxController.h>

#include <math.h>
#include <iostream>

XboxController::XboxController(int playerNumber)
{
	controllerNum_ = playerNumber - 1;
}

XINPUT_STATE XboxController::GetState()
{
	ZeroMemory(&controllerState_, sizeof(XINPUT_STATE));

	XInputGetState(controllerNum_, &controllerState_);

	return controllerState_;
}

float XboxController::GetLeftStickX()
{
	float x = (float)GetState().Gamepad.sThumbLX;
	if (abs(x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) x = 0;
	return x;
}

float XboxController::GetLeftStickY()
{
	float y = (float)GetState().Gamepad.sThumbLY;
	if (abs(y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) y = 0;
	return y;
}

float XboxController::GetDirectionAngle()
{
	//in degrees
	return atan2(GetLeftStickX(),GetLeftStickY())* 180.f / (float)M_PI;;
}

bool XboxController::LeftStickMoved()
{
	return (abs(GetLeftStickX()) > 0 || abs(GetLeftStickY()) > 0);
}

float XboxController::GetRightStickX()
{
	float x = (float)GetState().Gamepad.sThumbRX;
	if (abs(x) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) x = 0;
	return x;
}

float XboxController::GetRightStickY()
{
	float y = (float)GetState().Gamepad.sThumbRY;
	if (abs(y) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) y = 0;
	return y;
}

int XboxController::GetXCameraCoefficient()
{
	float x = GetRightStickX();

	int xCameraCoef;
	if (x > 0) {
		xCameraCoef = 1;
	} else if (x < 0) {
		xCameraCoef = -1;
	} else {
		xCameraCoef = 0;
	}

	return xCameraCoef;
}

int XboxController::GetYCameraCoefficient()
{
	float y = GetRightStickY();

	int yCameraCoef;
	if (y > 0) {
		yCameraCoef = 1;
	} else if (y < 0) {
		yCameraCoef = -1;
	} else {
		yCameraCoef = 0;
	}

	return yCameraCoef;
}

bool XboxController::RightStickMoved()
{
	return (abs(GetRightStickX()) > 0 || abs(GetRightStickY()) > 0);
}

bool XboxController::isConnected()
{
	ZeroMemory(&controllerState_, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(controllerNum_, &controllerState_);

	if (result == ERROR_SUCCESS) {
		return true;
	} else {
		return false;
	}
}

void XboxController::Vibrate(int leftVal, int rightVal)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = leftVal;
	vibration.wRightMotorSpeed = rightVal;

	XInputSetState(controllerNum_, &vibration);
}