#include <pch.h>

#include "Base/app.h"
#include "Base/layer.h"

namespace Simply2D
{
    void Layer::enqueTransition(std::shared_ptr<Layer> newLayer)
    {
        Application::GetInstance().m_mainThreadQueue.push_back(
            [this, newLayer]() mutable
            {
                auto& layerStack = Application::GetInstance().m_layers;
                for (auto& layer : layerStack)
                {
                    if (layer.get() == this)
                    {
                        layer = newLayer;
                        return;
                    }
                }
            }
        );
    }
}