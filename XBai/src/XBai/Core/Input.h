#ifndef XB_INPUT_H
#define XB_INPUT_H

#include "XBai/Core/Core.h"

namespace XBai
{
	class XB_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

#endif
