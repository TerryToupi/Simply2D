#include <Simly2D.h>

class level1 final : public Simply2D::Scene
{
public:
	level1(std::string level);

	virtual void load() override;
	virtual void update() override;
	virtual void render() override;
	virtual void unload() override;
};