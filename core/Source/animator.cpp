#include <pch.h>

#include <animator.h>

#include <Source/animatorManager.h>

namespace Simply2D
{
	Animator::Animator() 
	{
		AnimatorManager::GetInstance().Reginster(this);
	}
	Animator::~Animator()
	{
		AnimatorManager::GetInstance().Cancel(this);
	}
	void Animator::Stop(void)
	{
		Finish(true);
	}
	void Animator::TimeShift(double offset)
	{
		m_lastTime += offset;
	}
	void Animator::Progress(double currTime)
	{
		while (currTime > m_lastTime && (currTime - m_lastTime) >= m_anim->delay)
		{
			if (m_currFrame == m_anim->endFrame)
			{
				m_currFrame = m_anim->startFrame;
			}
			else
			{
				++m_currFrame;
			}

			m_lastTime += m_anim->delay;
			NotifyAction(*m_anim);

			if (m_currFrame == m_anim->endFrame)
			{
				if (!(!m_anim->reps) && ++m_currRep == m_anim->reps)
				{
					m_state = ANIMATOR_FINISHED;
					NotifyStopped();
					return;
				}
			}
		}
	}
	void Animator::Start(Animation* a, double t, uint16_t _currFrame)
	{
		m_anim = a;
		m_lastTime = t;
		m_state = ANIMATOR_RUNNING;
		m_currFrame = _currFrame;
		m_currRep = 0;
		NotifyStarted();
		NotifyAction(*m_anim);
	}
	void Animator::NotifyStopped(void)
	{
		AnimatorManager::GetInstance().MarkAsSuspended(this);
		if (m_onFinish)
			(m_onFinish)(this);
	}
	void Animator::NotifyStarted(void)
	{
		AnimatorManager::GetInstance().MarkAsRunning(this);
		if (m_onStart)
			(m_onStart)(this);
	}
	void Animator::NotifyAction(const Animation& anim)
	{
		if (m_onAction)
			(m_onAction)(this, anim);
	}
	void Animator::Finish(bool isForced)
	{
		if (!HasFinished())
		{
			m_state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
			NotifyStopped();
		}
	}
}