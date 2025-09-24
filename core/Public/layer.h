#pragma once

namespace Simply2D
{
	class Layer
	{
	public: 
		virtual ~Layer() = default;

		virtual void start() {}
		
		virtual void update() {}

		virtual void render() {}

		virtual void destroy() {}
	};
}