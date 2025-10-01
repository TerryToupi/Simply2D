#pragma once

#include <Simly2D.h>

#include <level1.h>

class SampleApp final : public Simply2D::Layer
{
public:
	SampleApp();
	~SampleApp();

	void event() override;
	void update(float ts) override;
	void render() override;

private:
	const float m_timer1 = 3.0f;
	float m_counter1 = 0.0f;
};