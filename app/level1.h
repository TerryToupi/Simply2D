#include <Simly2D.h>

class level1 final : public Simply2D::Scene
{
public:
	level1(std::string level);

	virtual void load()		       override;
	virtual void update(float ts)  override;
	virtual void render()          override;
	virtual void unload()          override;

private:
	const float m_timer1 = 0.5f;
	float m_counter1 = 0.0f;
};