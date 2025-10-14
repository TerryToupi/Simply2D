#include <Level2.h>
#include <Level1.h>

Level2::Level2(Simply2D::Asset level, Simply2D::SceneManager* manager)
	:	Scene(level, manager)
{
}

Level2::~Level2()
{
}

void Level2::event()
{
}

void Level2::update(float ts)
{ 
	m_counter1 += ts;
	if (m_counter1 >= m_timer1)
	{
		transition<Level1>();
		m_counter1 = 0.0f;
	}
}

void Level2::render()
{
}
