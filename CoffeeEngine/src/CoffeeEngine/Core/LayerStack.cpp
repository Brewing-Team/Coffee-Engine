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

	void LayerStack::PopLayer(Layer* layer)
	{
	    auto end = m_Layers.begin() + m_LayerInsertIndex;
	    auto it = std::find_if(m_Layers.begin(), end,
            [layer](const Scope<Layer>& current) { return current.get() == layer; });

	    if (it != end)
	    {
	        layer->OnDetach();
	        m_Layers.erase(it);
	        m_LayerInsertIndex--;
	    }
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
	    auto begin = m_Layers.begin() + m_LayerInsertIndex;
	    auto it = std::find_if(begin, m_Layers.end(),
            [overlay](const Scope<Layer>& current) { return current.get() == overlay; });

	    if (it != m_Layers.end())
	    {
	        overlay->OnDetach();
	        m_Layers.erase(it);
	    }
	}

}