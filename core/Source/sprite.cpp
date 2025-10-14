#include <pch.h>

#include <sprite.h>

namespace Simply2D
{
	Sprite::Sprite(const std::string& _name, int _x, int _y)
		:	m_name(_name), m_x(_x), m_y(_y)
	{
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
}