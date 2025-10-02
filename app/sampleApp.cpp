#include <sampleApp.h>
#include <sampleMenu.h>
#include <iostream>

SampleApp::SampleApp()
{
	m_sceneManager.push<Level1>("scenes/level1/Level1.json");
	m_sceneManager.push<Level2>("scenes/level1/Level1.json");
}

SampleApp::~SampleApp()
{
}

void SampleApp::event()
{
}

void SampleApp::update(float ts)
{
	m_sceneManager.update(ts);
}

void SampleApp::render()
{
	m_sceneManager.render(); 
}
