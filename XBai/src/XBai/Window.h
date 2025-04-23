#ifndef XB_WINDOW_H
#define XB_WINDOW_H

#include "xbpch.h"

#include "XBai/Core.h"
#include "XBai/Events/Event.h"

namespace XBai
{
	struct  WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "XBai Engine", 
			unsigned int width = 1200,
			unsigned int height = 720) : Title(title), Width(width), Height(height) {}
	};

	class XB_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}

#endif

