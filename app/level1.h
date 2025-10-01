#include <Simly2D.h>

class level1 final : public Simply2D::Scene
{
public:
	level1(std::string level);
	~level1();

	virtual void event()		   override;
	virtual void update(float ts)  override;
	virtual void render()          override;
};