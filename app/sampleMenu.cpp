#include <sampleMenu.h>
#include <sampleApp.h>

#include <iostream>

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::event()
{
}

void Menu::update(float ts)
{
	m_counter1 += ts;
	if (m_counter1 >= m_timer1)
	{
		transition<SampleApp>();
		std::cout << "[Time step] " << m_counter1 << std::endl;
		m_counter1 = 0.0f;
	}
}

void Menu::render()
{
}
