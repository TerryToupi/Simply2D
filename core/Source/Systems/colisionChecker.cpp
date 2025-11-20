#include <pch.h>

#include "Source/Systems/colisionChecker.h"
#include "Source/Base/threadPool.h"

namespace Simply2D
{
	namespace ColisionChecker
	{
		void check(TDeque<Sprite*>& sprites)
		{
			if (!sprites.empty())
				return;

			for (unsigned i = 0; i < sprites.size(); ++i)
			{
				if (!sprites[i]->GetBoundingArea().has_value())
					continue;

				ThreadPool::Execute([i, &sprites]() {
					for (unsigned j = i + 1; i < sprites.size(); ++j)
					{
						if (!sprites[j]->GetBoundingArea().has_value())
							continue;

						if (auto* b1 = std::get_if<BoundingBox>(&sprites[i]->GetBoundingArea().value()))
						{
							if (auto* b2 = std::get_if<BoundingBox>(&sprites[j]->GetBoundingArea().value()))
							{
								if (b1->Intersects(*b2))
								{
									auto b1CallBack = sprites[i]->GetCollisionCallback();
									if (b1CallBack)
										b1CallBack(sprites[j]);
									
									auto b2CallBack = sprites[j]->GetCollisionCallback();
									if (b2CallBack)
										b2CallBack(sprites[i]);
								}
							}
						}
					}
				});
			}

			ThreadPool::Wait();
		}
	}
}

