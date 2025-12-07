#pragma once

#include <Simply2D.h>

class Level1 final : public Simply2D::Scene
{
public:
	Level1(const std::string& scene);
	~Level1();

	virtual void event(Simply2D::Event& e)	override;
	virtual void start()					override;
	virtual void update(float ts)			override;
	virtual void render()					override;
	virtual void end(float ts)				override;

private:
	THandle<Simply2D::Audio> m_kompra;
	THandle<Simply2D::Audio> m_kippo;
};