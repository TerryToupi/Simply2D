#pragma once

#include "Types/SmartPointers.h"
#include "Scene/scene.h"

#include "Types/Arrays.h"
#include "Types/Function.h"

namespace Simply2D
{
	class ColisionSystem final
	{
	public:
		void Check(Ref<Scene>) const;
	};
}
