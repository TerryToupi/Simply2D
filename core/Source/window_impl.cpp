#include <pch.h>
#include <Source/window_impl.h>

namespace core
{
	std::unique_ptr<Window> Window::Create(const WindowSpecifications& specs)
	{
		return std::make_unique<WindowImpl>(specs);
	}

	WindowImpl::WindowImpl(const WindowSpecifications& specs)
		:	Window(specs)
	{
		SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
		if (m_specifications.borderless)
			flags |= SDL_WINDOW_BORDERLESS;
		else if (m_specifications.resizable)
			flags |= SDL_WINDOW_RESIZABLE;

		if (m_specifications.vSync)
			flags |= SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE;

		m_handle = SDL_CreateWindow(m_specifications.name.c_str(),
									m_specifications.width,
									m_specifications.height,
									flags);
	}

	WindowImpl::~WindowImpl()
	{
		SDL_DestroyWindow(m_handle);
	}

	void WindowImpl::update()
	{
	}
}