#ifndef XBAI_LAYER_H
#define XBAI_LAYER_H

#include "XBai/Core.h"
#include "XBai/Events/Event.h"

namespace XBai
{
	class XB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

#endif