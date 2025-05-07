#ifndef XB_IMGUILAYER_H
#define XB_IMGUILAYER_H

#include "XBai/Core/Layer.h"
#include "XBai/Events/KeyEvent.h"
#include "XBai/Events/MouseEvent.h"
#include "XBai/Events/ApplicationEvent.h"

namespace XBai
{
	class XB_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin() override;
		void End() override;

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}

#endif
