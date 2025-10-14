#include <sampleApp.h>
#include <sampleMenu.h>
#include <iostream>

SampleApp::SampleApp()
{
	Simply2D::assetDatabase().load(Simply2D::AssetType::SERIALIZABLE, m_scenes[0]);

	m_sceneManager.push<Level2>(Simply2D::assetDatabase().get(m_scenes[0]));
	m_sceneManager.push<Level1>(Simply2D::assetDatabase().get(m_scenes[0]));
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
