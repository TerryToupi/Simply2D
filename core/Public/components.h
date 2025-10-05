#pragma once

#include <string>
#include <array>

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
}