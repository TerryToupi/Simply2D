#pragma once

#include "Math/mathUtils.h"
#include "Types/Function.h"

namespace Simply2D
{
	class Clipper
	{
	public:
		using View = TFunction<const Rect& (void)>;

		Clipper(void) = default;
		Clipper(const Clipper& other) = default;

		Clipper& SetView(const View& f);
		bool Clip(Rect& r, const Rect& dpyArea, Rect* dpyPos, Rect* clippedBox) const;

	private:
		View m_view;
	};
}