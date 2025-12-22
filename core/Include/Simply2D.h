#pragma once

#include "Base/app.h"
#include "Base/assets.h"
#include "Base/resources.h"
#include "Rendering/renderer.h"
#include "Audio/audio.h"
#include "Scene/scene.h"
#include "Scene/boundingArea.h"
#include "Scene/gravity.h"
#include "Scene/motionQuantizer.h"
#include "Scene/sprite.h"
#include "Scene/tileLayers.h"
#include "Scene/tileSet.h"
#include "Scene/Camera.h"
#include "Animations/animator.h"
#include "Math/mathUtils.h"
#include "Memory/memory.h"
#include "Types/Arrays.h"
#include "Types/Deque.h"
#include "Types/Function.h"
#include "Types/Handle.h"
#include "Types/HashMap.h"
#include "Types/IDVector.h"
#include "Types/Pool.h"
#include "Types/Set.h"
#include "Types/SparceSet.h"
#include "Types/SmartPointers.h"
#include "Types/Span.h"
#include "Types/String.h"
#include "Types/Json.h"
#include "Events/event.h"
#include "Events/inputEvents.h"
#include "Events/windowEvent.h"

namespace Simply2D
{
	static inline Simply2D::Application& app()
	{
		return *Simply2D::Application::GetInstance();
	}

	static inline Simply2D::Renderer& gfx()
	{
		return *Simply2D::Application::GetRenderer();
	}

	static inline Simply2D::AssetDatabase& assets()
	{
		return *Simply2D::Application::GetAssetDatabase();
	}
	
	static inline Simply2D::AudioSystem& audio()
	{
		return *Simply2D::Application::GetAudioSystem();
	}
}
