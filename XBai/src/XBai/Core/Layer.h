#ifndef XBAI_LAYER_H
#define XBAI_LAYER_H

#include "XBai/Core/Core.h"
#include "XBai/Events/Event.h"
#include "XBai/Core/TimeStep.h"

namespace XBai
{
	class XB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep timestep) {}
		virtual void OnImGuiRender(){}
		virtual void OnEvent(Event& e) {}
		virtual void Begin(){}
		virtual void End(){}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

#endif