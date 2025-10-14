#pragma once

#include <Simply2D.h>

#include <level1.h>
#include <level2.h>

class SampleApp final : public Simply2D::Layer
{
public:
	SampleApp();
	~SampleApp();

	void event() override;
	void update(float ts) override;
	void render() override;

private:
	std::array<std::string, 1> m_scenes = {
		"scenes/level1/Level1.json",
	};

	Simply2D::SceneManager m_sceneManager;
};