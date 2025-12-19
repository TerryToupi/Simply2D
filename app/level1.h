#pragma once

#include <Simply2D.h>

class Level1 final : public Simply2D::Scene
{
public:
	Level1(const std::string& scene);
	~Level1();

	virtual void event(Simply2D::Event& e)	override;
	virtual void update(float ts)			override;
	virtual void render()					override;
	virtual void end(float ts)				override;

protected:
	// Mark brown ground tiles as empty/passable
	virtual TSet<uint16_t> getEmptyTileIndices() const override;

private:
	bool m_showGridDebug = false;
};