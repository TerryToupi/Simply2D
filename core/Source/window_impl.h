#pragma once

#include <Include/window.h>
#include <SDL3/SDL.h>

namespace core
{
	class WindowImpl : public Window
	{
	public:
		WindowImpl(const WindowSpecifications& specs);
		~WindowImpl();

		void update() override;

	private:
		SDL_Window* m_handle{};
	};
}