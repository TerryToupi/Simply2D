#pragma once

#include <string>

namespace Simply2D
{
	struct Tag
	{
		std::string tag;

		Tag() = default;
		Tag(const Tag& other) = default;
		Tag(std::string& tag)
			:	tag(tag) { }
	};

	template<typename... Components>
	struct ComponentGroup
	{
	};

	using AllCopmonents = ComponentGroup<Tag>;
}