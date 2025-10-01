#include<Simly2D.h>

class Menu final : public Simply2D::Layer
{
public:
	Menu();
	~Menu();

	virtual void event() override;
	virtual void update(float ts) override;
	virtual void render() override;

private:
	const float m_timer1 = 3.0f;
	float m_counter1 = 0.0f;
};