#ifndef XB_WINDOW_H
#define XB_WINDOW_H

#include "xbpch.h"

#include "XBai/Core/Core.h"
#include "XBai/Events/Event.h"

namespace XBai
{
	struct  WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "XBai Engine", 
			uint32_t width = 1600,
			uint32_t height = 900) : Title(title), Width(width), Height(height) {}
	};

	class XB_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}

#endif

