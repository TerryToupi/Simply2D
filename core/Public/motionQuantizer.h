#pragma once 

#include <functional>
#include <mathUtils.h>

namespace Simply2D 
{
	class MotionQuantizer
	{
	public:
		using Mover = std::function<void(Rect& r, int* dx, int* dy)>;

		MotionQuantizer() = default;
		MotionQuantizer(const MotionQuantizer& other) = default;

		MotionQuantizer& SetUsed(bool val);
		MotionQuantizer& SetRange(int h, int v);
		MotionQuantizer& SetMover(const Mover& f);

		void Move(Rect& r, int* dx, int* dy);

	protected:
		int		m_Hmax = 0, m_Vmax = 0;
		Mover	m_Mover;
		bool	m_Used = false;
	};
}