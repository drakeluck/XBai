#ifndef XB_WINDOWS_INPUT_H
#define XB_WINDOWS_INPUT_H

#include "XBai/Core/Input.h"

namespace XBai
{
	class WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMousePressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	private:

	};
}
#endif
