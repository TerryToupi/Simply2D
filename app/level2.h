#pragma once

#include <Simply2D.h>

class Level2 final : public Simply2D::Scene
{
public:
	Level2(Simply2D::Asset level, Simply2D::SceneManager* manager);
	~Level2();

	virtual void event()		   override;
	virtual void update(float ts)  override;
	virtual void render()          override;

private:
	const float m_timer1 = 3.0f;
	float m_counter1 = 0.0f;
};