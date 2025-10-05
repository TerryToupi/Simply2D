#include <level1.h>
#include <level2.h>

Level1::Level1(std::string level, Simply2D::SceneManager* manager)
	:	Scene(level, manager)
{
	Simply2D::Sprite s1 = createSprite("antonis");
	Simply2D::Sprite s2 = createSprite("kwstas");
	Simply2D::Sprite s3 = createSprite("giwrgis");
}

Level1::~Level1()
{
}

void Level1::event()
{
}

void Level1::update(float ts)
{
	m_counter1 += ts;
	if (m_counter1 >= m_timer1)
	{
		m_counter1 = 0.0f;
	}

	Simply2D::Sprite antonis = getSprite("antonis");
}

void Level1::render()
{
	for (const auto& layer : m_layers)
	{
		int twidth = 0, theight = 0;
		Simply2D::gfx().textureSize(layer->texture(), twidth, theight);

		int swidth = 0, sheight = 0;
		Simply2D::gfx().textureSize(SURFACE, swidth, sheight);

		Simply2D::gfx().draw(
			{
				.target = SURFACE,
				.loadOp = Simply2D::LoadOp::LOAD,
				.storeOp = Simply2D::StoreOp::STORE,
				.clearColor = {0, 0, 0, 255}
			}, 
			{
				{ layer->texture(), Simply2D::Blend::BLEND, 255, {0, 0, twidth, theight}, {0, 0, swidth, sheight} } 
			});
	}
}

