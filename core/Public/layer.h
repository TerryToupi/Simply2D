#pragma once

#include <memory>

namespace Simply2D
{
	class Layer
	{
	public: 
		Layer() = default;
		virtual ~Layer() = default;

		virtual void begin(float ts)	{}
		virtual void event()			{}
		virtual void update(float ts)	{}
		virtual void render()			{}
		virtual void end(float ts)		{}

	public:
		template<std::derived_from<Layer> TLayer, typename... Args>
		void transition(Args&&... args)
		{
			enqueTransition(std::move(std::make_shared<TLayer>(std::forward<Args>(args)...)));
		}

	private:
		void enqueTransition(std::shared_ptr<Layer> newLayer);
	};
}