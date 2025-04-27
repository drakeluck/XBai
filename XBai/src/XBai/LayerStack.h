#ifndef XB_LAYERSTACK_H
#define XB_LAYERSTACK_H

#include "XBai/Core.h"
#include "Layer.h"

namespace XBai
{
	class XB_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int  m_LayerInsertIndex = 0;
	};
}


#endif
