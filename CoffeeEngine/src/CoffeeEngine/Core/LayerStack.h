/**
 * @defgroup core Core
 * @brief Core components of the CoffeeEngine.
 * @{
 */

#pragma once

#include <vector>

namespace Coffee {

    class Layer;

}

namespace Coffee {

    /**
     * @class LayerStack
     * @brief Manages a stack of layers and overlays.
     */
    class LayerStack
    {
    public:
        /**
         * @brief Default constructor.
         */
        LayerStack() = default;

        /**
         * @brief Destructor.
         */
        ~LayerStack();

        /**
         * @brief Adds a layer to the stack.
         * @param layer Pointer to the layer to be added.
         */
        void PushLayer(Scope<Layer> layer);

        /**
         * @brief Adds an overlay to the stack.
         * @param overlay Pointer to the overlay to be added.
         */
        void PushOverlay(Scope<Layer> overlay);

        /**
         * @brief Removes a layer from the stack.
         * @param layer Pointer to the layer to be removed.
         */
        void PopLayer(Scope<Layer> layer);

        /**
         * @brief Removes an overlay from the stack.
         * @param overlay Pointer to the overlay to be removed.
         */
        void PopOverlay(Scope<Layer> overlay);

        /**
         * @brief Returns an iterator to the beginning of the layer stack.
         * @return Iterator to the beginning of the layer stack.
         */
        std::vector<Scope<Layer>>::iterator begin() { return m_Layers.begin(); }

        /**
         * @brief Returns an iterator to the end of the layer stack.
         * @return Iterator to the end of the layer stack.
         */
        std::vector<Scope<Layer>>::iterator end() { return m_Layers.end(); }

        /**
         * @brief Returns a reverse iterator to the beginning of the layer stack.
         * @return Reverse iterator to the beginning of the layer stack.
         */
        std::vector<Scope<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }

        /**
         * @brief Returns a reverse iterator to the end of the layer stack.
         * @return Reverse iterator to the end of the layer stack.
         */
        std::vector<Scope<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

        /**
         * @brief Returns a constant iterator to the beginning of the layer stack.
         * @return Constant iterator to the beginning of the layer stack.
         */
        std::vector<Scope<Layer>>::const_iterator begin() const { return m_Layers.begin(); }

        /**
         * @brief Returns a constant iterator to the end of the layer stack.
         * @return Constant iterator to the end of the layer stack.
         */
        std::vector<Scope<Layer>>::const_iterator end() const { return m_Layers.end(); }

        /**
         * @brief Returns a constant reverse iterator to the beginning of the layer stack.
         * @return Constant reverse iterator to the beginning of the layer stack.
         */
        std::vector<Scope<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }

        /**
         * @brief Returns a constant reverse iterator to the end of the layer stack.
         * @return Constant reverse iterator to the end of the layer stack.
         */
        std::vector<Scope<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    private:
        std::vector<Scope<Layer>> m_Layers; ///< Vector of layer pointers.
        unsigned int m_LayerInsertIndex = 0; ///< Index at which to insert new layers.
    };

}

/** @} */