#include <pch.h>

#include "Source/Animation/animatorManager.h"

namespace Simply2D
{
	AnimatorManager AnimatorManager::s_Instance;

	void AnimatorManager::Reginster(Animator* a)
	{
		m_suspended.insert(a);
	}
	void AnimatorManager::Cancel(Animator* a)
	{
		m_suspended.erase(a);
	}
	void AnimatorManager::MarkAsRunning(Animator* a)
	{
		m_suspended.erase(a);
		m_running.insert(a);
	}
	void AnimatorManager::MarkAsSuspended(Animator* a)
	{
		m_running.erase(a);
		m_suspended.insert(a);
	}
	void AnimatorManager::TimeShift(double offset)
	{
		auto copied(m_running);
		for (auto* a : copied)
		{
			a->TimeShift(offset);
		}
	}
	void AnimatorManager::Progress(double currTime)
	{
		auto copied(m_running);
		for (auto* a : copied)
		{
			a->Progress(currTime);
		}
	}
}
