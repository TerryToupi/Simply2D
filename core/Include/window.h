#pragma once

#include <string>
#include <memory>

namespace core
{
	struct WindowSpecifications
	{
		std::string name = "giorgos";
		int width = 640;
		int height = 480;
		bool borderless = false;
		bool resizable = true;
		bool vSync = true;
	};

	class Window
	{
	public:
		static std::unique_ptr<Window> Create(const WindowSpecifications& specs);

		Window(const WindowSpecifications& specs = WindowSpecifications())
			: m_specifications(specs) { }
		virtual ~Window() = default;

		virtual void update() = 0;
	
	protected:
		WindowSpecifications m_specifications;
	};
}