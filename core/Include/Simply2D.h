#pragma once

#include <span.h>
#include <resources.h>
#include <renderer.h>
#include <pool.h>
#include <layer.h>
#include <handle.h>
#include <assets.h>
#include <tileSet.h>
#include <tileLayers.h>
#include <components.h>
#include <sprites.h>
#include <scene.h>
#include <app.h>

namespace Simply2D
{
	static inline Simply2D::Application& app()
	{
		return Simply2D::Application::GetInstance();
	}

	static inline Simply2D::Renderer& gfx()
	{
		return *Simply2D::Application::GetInstance().GetRenderer();
	}

	static inline Simply2D::AssetDatabase& assetDatabase()
	{
		return *Simply2D::Application::GetInstance().GetAssetDatabase();
	}
}
