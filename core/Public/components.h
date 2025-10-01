#pragma once

#include <string>

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
}