#pragma once

#include <Simply2D.h>

class Level1 final : public Simply2D::Scene
{
public:
	Level1(Simply2D::Asset level, Simply2D::SceneManager* manager);
	~Level1();

	virtual void event()		   override;
	virtual void update(float ts)  override;
	virtual void render()          override;
	virtual void end(float ts)	   override;

private:
	Simply2D::Animation m_anim1;
	Simply2D::Animation m_anim2;
	Simply2D::Animation m_anim3;
	Simply2D::Animator	m_animator;

	const float m_timer1 = 3.0f;
	float m_counter1 = 0.0f;
};