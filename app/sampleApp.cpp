#include <sampleApp.h>
#include <sampleMenu.h>
#include <iostream>

SampleApp::SampleApp()
{
	Simply2D::sceneManager().pushScene<level1>("scenes/level1/Level1.json");
	Simply2D::sceneManager().setActive(0);
}

SampleApp::~SampleApp()
{
}

void SampleApp::event()
{
}

void SampleApp::update(float ts)
{
	//m_counter1 += ts;
	//if (m_counter1 >= m_timer1)
	//{
	//	std::cout << "[Time step] " << m_counter1 << std::endl;
	//	m_counter1 = 0.0f;
	//	transition<Menu>();
	//}

	Simply2D::sceneManager().get()->update(ts);
}

void SampleApp::render()
{
	Simply2D::sceneManager().get()->render(); 
}
