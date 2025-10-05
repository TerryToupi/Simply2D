#pragma once

#include <string>
#include <array>

#include <resources.h>

namespace Simply2D
{
	struct Component {};

	struct Tag final : Component
	{
		std::string tag;

		Tag() = default;
		Tag(const Tag& other) = default;
		Tag(std::string& tag)
			:	tag(tag) { }
	};

	struct BoundingBox final : Component
	{ 
		std::array<int, 4> box = { 0, 0, 0, 0 };

		BoundingBox() = default;
		BoundingBox(const BoundingBox& other) = default;
		BoundingBox(std::array<int, 4>& box)
			: box(box) { }
	};

	struct Animation final : Component
	{
		enum animatorstate_t
		{
			ANIMATOR_FINISHED = 0,
			ANIMATOR_RUNNING = 1,
			ANIMATOR_STOPPED = 2
		};

		using FrameRects = std::array<std::array<int, 4>, 30>;
		
		// animation atlas
		Handle<Texture> texture;
		FrameRects frames;
		uint16_t totalFrames = 0;
		
		// animation data
		uint16_t dx = 0;
		uint16_t dy = 0;
		uint16_t delay = 0;
		uint16_t reps = 1;
		
		// frame range animation data
		uint16_t startFrame = 0;
		uint16_t endFrame = 0;

		// animator data
		double lastTime = 0;
		uint16_t currFrame = 0;
		uint16_t currRep = 0;
		animatorstate_t state = ANIMATOR_FINISHED;

		void Stop(void) 
		{ 
			if (!HasFinished()) 
				state = ANIMATOR_STOPPED; 
		}
		void Start(double time, uint16_t _currFrame)
		{
			state = ANIMATOR_RUNNING;
			currFrame = _currFrame;
			currRep = 0;
		} 
		bool HasFinished(void) const 
		{ 
			return state != ANIMATOR_RUNNING; 
		}
		void TimeShift(double offset) 
		{ 
			lastTime += offset; 
		}
		void Progress(double currtime)
		{
			while (currtime > lastTime && (currtime - lastTime) >= delay)
			{
				if (currFrame == endFrame)
				{
					currFrame = startFrame;
				}
				else
				{
					++currFrame;
				}

				lastTime += delay;

				if (currFrame == endFrame)
				{
					if (!(!reps) && ++currRep == reps)
					{
						state = ANIMATOR_FINISHED;
						return;
					}
				}
			}
		}
	};
}