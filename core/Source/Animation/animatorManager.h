#pragma once

#include "Animations/animator.h"
#include "Types/Set.h"

namespace Simply2D
{
	class AnimatorManager
	{
	public:
		AnimatorManager(void) = default;
		AnimatorManager(const AnimatorManager&) = delete;
		AnimatorManager(AnimatorManager&&) = delete;

		void Reginster(Animator* a);
		void Cancel(Animator* a);
		void MarkAsRunning(Animator* a);
		void MarkAsSuspended(Animator* a);
		void Progress(double currTime);
		void TimeShift(double dt);

		static AnimatorManager& GetInstance(void)
		{
			static AnimatorManager instacne;
			return instacne;
		}

	private:
		TSet<Animator*>	m_running, m_suspended;
	};
}