#pragma once

namespace Simply2D
{
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(const Sprite& other) = default;
		Sprite(Sprite&& other) = default;
		virtual ~Sprite() = default;
	};
}