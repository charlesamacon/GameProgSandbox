#include <Windows.h>
#include <windowsx.h>
#include <Xinput.h>
#include <iostream>

#pragma comment(lib, "Xinput.lib")

void vibrateTest(XINPUT_STATE state);
void outputInput(XINPUT_STATE state);
bool isPressed(XINPUT_STATE state, WORD button);

using namespace std;

int main()
{
	int controllerId = -1;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			controllerId = i;
		}
	}

	if (controllerId == -1)
	{
		cout << "No controller detected!" << endl;
		cout << "Exiting" << endl;
		system("PAUSE");
	}
	else
	{
		cout << "Controller detected on port: " << controllerId << endl;
		//system("PAUSE");
		//vibrateTest(state);
		system("PAUSE");
		outputInput(state);
	}

	

	return 0;
}

void vibrateTest(XINPUT_STATE state)
{
	for (int i = 0; i < 10; i++)
	{
		int c = i + 1; 
		system("CLS");
		cout << "Vibrate test: " << c << endl;

		XINPUT_VIBRATION vib;
		if (c % 2 == 0)
		{
			cout << "Testing Left" << endl;
			vib.wLeftMotorSpeed = 65535 * c;
		}
		else
		{
			cout << "Testing Right" << endl;
			vib.wRightMotorSpeed = 65535 * c;
		}
		XInputSetState(JOYSTICKID1, &vib);

		Sleep(1000);

		vib.wLeftMotorSpeed = 0;
		vib.wRightMotorSpeed = 0;
		XInputSetState(JOYSTICKID1, &vib);

		Sleep(1000);
	}

	XINPUT_VIBRATION vib;
	vib.wLeftMotorSpeed = 0;
	vib.wRightMotorSpeed = 0;
	XInputSetState(JOYSTICKID1, &vib);

	for (int i = 0; i < 65535; i = i + 100)
	{
		system("CLS");
		cout << "Testing Vibration Fade: " << i << endl;
		vib.wLeftMotorSpeed = i;
		vib.wRightMotorSpeed = i;
		XInputSetState(JOYSTICKID1, &vib);

		// So we are able to see all vibration levels before the end of time
		Sleep(1);
	}

	for (int i = 65535; i > 0; i = i - 100)
	{
		system("CLS");
		cout << "Testing Vibration Fade: " << i << endl;
		vib.wLeftMotorSpeed = i;
		vib.wRightMotorSpeed = i;
		XInputSetState(JOYSTICKID1, &vib);

		// So we are able to see all vibration levels before the end of time
		Sleep(1);
	}
}

void outputInput(XINPUT_STATE state)
{
	int quit = 0;
	
	XINPUT_VIBRATION vib;
	vib.wLeftMotorSpeed = 0;
	vib.wRightMotorSpeed = 0;
	XInputSetState(JOYSTICKID1, &vib);


	while (quit == 0)
	{
		
		Sleep(50);
		//system("CLS");
		vib.wLeftMotorSpeed = 0;
		vib.wRightMotorSpeed = 0;
		XInputSetState(JOYSTICKID1, &vib);
		XInputGetState(JOYSTICKID1, &state);

		
		if (isPressed(state, XINPUT_GAMEPAD_A))
		{
			cout << "A pressed" << endl;
			vib.wLeftMotorSpeed = 60000;
			vib.wRightMotorSpeed = 65535;
			XInputSetState(JOYSTICKID1, &vib);
		}
		else if (isPressed(state, XINPUT_GAMEPAD_B))
		{
			cout << "B pressed" << endl;
			vib.wLeftMotorSpeed = 0;
			vib.wRightMotorSpeed = 65500;	// Kinda stronger seems like the biggest difference is between 65000 and 65535
			XInputSetState(JOYSTICKID1, &vib);
		}
		else if (isPressed(state, XINPUT_GAMEPAD_B) && isPressed(state, XINPUT_GAMEPAD_A))
		{
			cout << "A and B pressed" << endl;

		}
		else if (isPressed(state, XINPUT_GAMEPAD_X))
		{
			cout << "X pressed" << endl;
			vib.wLeftMotorSpeed = 65535;	// Really Strong
			vib.wRightMotorSpeed = 0;
			XInputSetState(JOYSTICKID1, &vib);
		}
		else if (isPressed(state, XINPUT_GAMEPAD_Y))
		{
			cout << "Y pressed" << endl;
			vib.wLeftMotorSpeed = 15000;
			vib.wRightMotorSpeed = 45000;
			XInputSetState(JOYSTICKID1, &vib);
		}
		else if (isPressed(state, XINPUT_GAMEPAD_DPAD_UP))
		{
			cout << "D-PAD UP pressed" << endl;
		}
		else if (isPressed(state, XINPUT_GAMEPAD_DPAD_DOWN))
		{
			cout << "D-PAD DOWN pressed" << endl;
		}
		else if (isPressed(state, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			cout << "D-PAD LEFT pressed" << endl;
		}
		else if (isPressed(state, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			cout << "D-PAD RIGHT pressed" << endl;
		}
		else if (state.Gamepad.bLeftTrigger > 0)
		{
			cout << "Left Trigger" << endl;
			vib.wLeftMotorSpeed = (((state.Gamepad.bLeftTrigger + 1) * 256) - 1);
			XInputSetState(JOYSTICKID1, &vib);
		}
		else if (state.Gamepad.bRightTrigger > 0)
		{
			cout << "Right Trigger" << endl;
			vib.wRightMotorSpeed = (((state.Gamepad.bRightTrigger + 1) * 256) - 1);
			XInputSetState(JOYSTICKID1, &vib);
		}
		else
		{
			cout << "No current input" << endl;
		}

	}
	return;
}

bool isPressed(XINPUT_STATE state, WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;
}