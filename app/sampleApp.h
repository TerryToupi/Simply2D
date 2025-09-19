#pragma once

#include <app.h>

class SampleApp : public core::Layer
{
public:
	void start() override;
	void destroy() override;
	void render() override;

private:
	Handle<Texture> m_texture;
};