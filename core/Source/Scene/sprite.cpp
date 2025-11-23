#include "Scene/sprite.h"

namespace Simply2D
{
	Sprite::Sprite(const TString& _name, int _x, int _y)
		:	m_name(_name), m_x(_x), m_y(_y)
	{
	}

	void Sprite::SetBox(int _width, int _height)
	{ 
		m_width = _width;
		m_height = _height;
	}

	Rect Sprite::GetBox()
	{
		return { m_x, m_y, m_width, m_height };
	}

	void Sprite::SetMover(const Mover& move)
	{
		m_mover = move;
	}

	Sprite& Sprite::Move(int dx, int dy)
	{
		if (m_directMotion)
		{
			m_x += dx;
			m_y += dy;
		}
		else
		{
			Rect r = GetBox();
			m_quantizer.Move(r, &dx, &dy);
			m_gravity.Check(r);
		}

		return *this;
	}

	void Sprite::SetPos(int _x, int _y)
	{
	}

	int Sprite::GetPosX() const
	{
		return 0;
	}

	int Sprite::GetPosY() const
	{
		return 0;
	}

	void Sprite::SetBoundingArea(const BoundingVariant& area)
	{
	}

	void Sprite::SetColiderBox(unsigned _w, unsigned _h)
	{
	}

	void Sprite::SetMotionQuantizerUse(bool v)
	{
	}

	void Sprite::SetQuanntizerVertHorz(int v, int h)
	{
	}

	void Sprite::SetCollisionCallback(const CollisionCallback& callback)
	{
		m_collisionCallback = callback;
	}

	GravityHandler& Sprite::GetGravityHandler(void)
	{
		return m_gravity;
	}

	Sprite& Sprite::SetHasDirectMotion(bool v)
	{
		m_directMotion = v; return *this;
	}

	bool Sprite::GetHasDirectMotion(void) const
	{
		return m_directMotion;
	}
}