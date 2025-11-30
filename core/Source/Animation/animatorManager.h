#pragma once

#include "Animations/animator.h"
#include "Memory/memory.h"
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

		static AnimatorManager* GetInstance(void)
		{
			return s_pInstance;
		}

	private:
		static void Create()
		{
			assert(s_pInstance == nullptr);
			s_pInstance = MM::New<AnimatorManager>();
		}

		static void Destroy()
		{
			assert(s_pInstance != nullptr);
			MM::Delete<AnimatorManager>(s_pInstance);
		}

	private:
		TSet<Animator*>	m_running, m_suspended;

		static inline AnimatorManager* s_pInstance = nullptr;

		friend class Application;
	};
}