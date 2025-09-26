#include <Simly2D.h>

class level1 : public Simply2D::Scene
{
public:
	virtual void load() override;
	virtual void update() override;
	virtual void render() override;
	virtual void unload() override;
};