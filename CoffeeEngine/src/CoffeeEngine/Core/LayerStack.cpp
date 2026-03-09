#include "CoffeeEngine/Core/LayerStack.h"
#include "CoffeeEngine/Core/Layer.h"

#include <algorithm>

namespace Coffee {

	LayerStack::~LayerStack()
	{
		for (Scope<Layer>& layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerStack::PushLayer(Scope<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Scope<Layer> overlay)
	{
		m_Layers.emplace_back(std::move(overlay));
	}

	void LayerStack::PopLayer(Scope<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Scope<Layer> overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}