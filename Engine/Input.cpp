#include "Input.h"


void Input::ResetInput()
{
	for (int i = 0; i < 255; i++)
	{
		if (i < 3)
		{
			MouseButtonDown[i] = false;
			MouseButtonUp[i] = false;
		}
		KeyDown[i] = false;
		KeyUp[i] = false;
	}

	controller = {};
}