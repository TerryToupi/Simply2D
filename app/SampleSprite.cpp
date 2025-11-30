#include "SampleSprite.h"

SampleSprite::SampleSprite(const char* name, int _x, int _y)
	:	Sprite(name, _x, _y)
{
}

void SampleSprite::begin(float ts)
{

}

void SampleSprite::update(float ts)
{
	m_animator.Start(&m_anim1, 0.0f, 0);
}

void SampleSprite::end(float ts)
{

}

Simply2D::BoundingArea* SampleSprite::OnGetCollider()
{
	m_box1 = Simply2D::BoundingBox(1, 1, 1, 1);

	return &m_box1;
}

void SampleSprite::OnCollision(Sprite& other)
{

}

