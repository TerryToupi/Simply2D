#pragma once

#include "Base/app.h"
#include "Base/assets.h"
#include "Base/resources.h"

#include "Rendering/renderer.h"

#include "Scene/scene.h"
#include "Scene/boundingArea.h"
#include "Scene/gravity.h"
#include "Scene/motionQuantizer.h"
#include "Scene/sprite.h"
#include "Scene/tileLayers.h"
#include "Scene/tileSet.h"

#include "Animations/animator.h"

#include "Math/mathUtils.h"

#include "Memory/bumpAllocator.h"
#include "Memory/offsetAllocator.h"
#include "Memory/memoryAllocator.h"

#include "Types/handle.h"
#include "Types/pool.h"
#include "Types/span.h"
#include "Types/mtRingBuffer.h"

namespace Simply2D
{
	static inline Simply2D::Application& app()
	{
		return *Simply2D::Application::GetInstance();
	}

	static inline Simply2D::Renderer& gfx()
	{
		return *Simply2D::app().GetRenderer();
	}

	static inline Simply2D::AssetDatabase& assetDatabase()
	{
		return *Simply2D::app().GetAssetDatabase();
	}
}
