#pragma once

#include <span.h>
#include <resources.h>
#include <renderer.h>
#include <pool.h>
#include <layer.h>
#include <handle.h>
#include <assets.h>
#include <app.h>

namespace Simply2D
{
	static inline core::Application& app()
	{
		return core::Application::GetInstance();
	}

	static inline core::Renderer& gfx()
	{
		return *core::Application::GetInstance().GetRenderer();
	}

	static inline core::AssetDatabase& assetDatabase()
	{
		return *core::Application::GetInstance().GetAssetDatabase();
	}
}
