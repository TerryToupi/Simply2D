#pragma once

#include "Simply2D.h"

class SampleSprite : public Simply2D::Sprite
{
public:
	SampleSprite(const char* name, int _x, int _y);
	virtual ~SampleSprite() override {}

	virtual void begin(float ts) override;
	virtual void update(float ts) override;
	virtual void end(float ts) override;

	virtual Simply2D::BoundingArea* OnGetCollider() override;
	virtual	void					OnCollision(Sprite& other) override;

public:
	Simply2D::Animation m_anim1{};
	Simply2D::Animator m_animator{};

	Simply2D::BoundingBox m_box1{ 0, 0, 0, 0 };
};
