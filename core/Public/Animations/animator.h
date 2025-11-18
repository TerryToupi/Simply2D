#pragma once

#include <set>
#include <functional>

#include "Base/resources.h"

namespace Simply2D
{
	struct Animation 
	{
		using FrameRects = std::array<std::array<int, 4>, 30>;

		// animation atlas
		THandle<Texture> texture;
		FrameRects frames;
		uint16_t totalFrames = 0;

		// animation data
		uint16_t dx = 0;
		uint16_t dy = 0;
		uint16_t delay = 0;
		uint16_t reps = 1;

		// frame range animation data
		uint16_t startFrame = 0;
		uint16_t endFrame = 0;
	};

	enum animatorstate_t
	{
		ANIMATOR_FINISHED = 0,
		ANIMATOR_RUNNING = 1,
		ANIMATOR_STOPPED = 2
	};

	class Animator
	{
	public:
		using OnFinish = std::function<void(Animator*)>;
		using OnStart = std::function<void(Animator*)>;
		using OnAction = std::function<void(Animator*, const Animation&)>;

	public:
		Animator();
		Animator(const Animator&) = delete;
		Animator(Animator&&) = delete;
		virtual ~Animator();

		void	Stop(void);
		bool	HasFinished(void) const { return m_state != ANIMATOR_RUNNING; }
		void	TimeShift(double offset);
		void	Progress(double currTime);
		void	Start(Animation* a, double t, uint16_t _currFrame);

		void SetOnFinish(OnFinish f) { m_onFinish = f; }
		void SetOnStart(OnStart f) { m_onStart = f; }
		void SetOnAction(OnAction f) { m_onAction = f; }

	private:
		double				m_lastTime = 0;
		animatorstate_t		m_state = ANIMATOR_FINISHED;
		OnFinish			m_onFinish;
		OnStart				m_onStart;
		OnAction			m_onAction;
		void				NotifyStopped(void);
		void				NotifyStarted(void);
		void				NotifyAction(const Animation&);
		void				Finish(bool isForced = false);

		Animation*			m_anim = nullptr;
		uint32_t			m_currFrame = 0;
		uint32_t			m_currRep = 0;
	};
}