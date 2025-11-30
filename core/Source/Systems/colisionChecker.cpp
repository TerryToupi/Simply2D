#include "Source/Systems/colisionChecker.h"

namespace Simply2D
{
	namespace ColisionChecker
	{
		void check(Ref<Scene> scene)
		{
			//for (auto sprite : scene->GetSprites())
			//{
			//	sprite->GetBox();
				//if (!sprites[i]->GetBoundingArea().has_value())
				//	continue;

				//ThreadPool::Execute([i, &sprites]() {
				//	for (unsigned j = i + 1; i < sprites.size(); ++j)
				//	{
				//		if (!sprites[j]->GetBoundingArea().has_value())
				//			continue;

				//		if (auto* b1 = std::get_if<BoundingBox>(&sprites[i]->GetBoundingArea().value()))
				//		{
				//			if (auto* b2 = std::get_if<BoundingBox>(&sprites[j]->GetBoundingArea().value()))
				//			{
				//				if (b1->Intersects(*b2))
				//				{
				//					auto b1CallBack = sprites[i]->GetCollisionCallback();
				//					if (b1CallBack)
				//						b1CallBack(sprites[j]);
				//					
				//					auto b2CallBack = sprites[j]->GetCollisionCallback();
				//					if (b2CallBack)
				//						b2CallBack(sprites[i]);
				//				}
				//			}
				//		}
				//	}
				//});
			//}
		}
	}
}

