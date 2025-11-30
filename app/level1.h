#pragma once

#include <Simply2D.h>

class Level1 final : public Simply2D::Scene
{
public:
	Level1(Simply2D::Asset level);
	~Level1();

	virtual void event()		   override;
	virtual void update(float ts)  override;
	virtual void render()          override;
	virtual void end(float ts)	   override;
};