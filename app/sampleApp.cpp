#include <sampleApp.h>

void SampleApp::start()
{
	for (auto& [path, type] : m_assets)
		Simply2D::assetDatabase().load(type, path);

	Simply2D::sceneManager().pushScene<level1>("scenes/level1/Level1.json");
	Simply2D::sceneManager().setActive(0);
}

void SampleApp::destroy()
{
}

void SampleApp::render()
{
	Simply2D::sceneManager().get()->render(); 
}
