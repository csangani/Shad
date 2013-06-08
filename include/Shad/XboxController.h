#ifndef XBOX_CONTROLLER_H
#define XBOX_CONTROLLER_H

#include <windows.h>
#include <XInput.h>

#pragma comment(lib, "XInput.lib")

class XboxController {
public:
	XboxController(int playerNumber);
	XINPUT_STATE GetState();
	float GetDirectionAngle();
	int GetXCameraCoefficient();
	int GetYCameraCoefficient();
	bool isConnected();
	bool LeftStickMoved();
	bool RightStickMoved();
	
	void Vibrate(int leftVal = 0, int rightVal = 0);

private:
	XINPUT_STATE controllerState_;
	int controllerNum_;
	float GetRightStickX();
	float GetRightStickY();
	float GetLeftStickX();
	float GetLeftStickY();
};

#endif